#include "stdafx.h"
#include "rdostudioplugins.h"
#include "rdostudioapp.h"
#include "rdostudiomodel.h"

#include <rdokernel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugin
// ----------------------------------------------------------------------------
RDOStudioPlugin::RDOStudioPlugin( const std::string& _modulName ):
	modulName( _modulName ),
	lib( NULL ),
	name( "" ),
	version_major( 0 ),
	version_minor( 0 ),
	version_build( 0 ),
	version_info( "" ),
	description( "" ),
	state( rdoPlugin::psStoped ),
	restoreState( true ),
	defaultRunMode( rdoPlugin::prmNoAuto ),
	runMode( rdoPlugin::prmNoAuto )
{
	lib = ::LoadLibrary( modulName.c_str() );
	if ( lib ) {
		rdoPlugin::PFunGetPluginInfo getPluginInfo = reinterpret_cast<rdoPlugin::PFunGetPluginInfo>(::GetProcAddress( lib, "getPluginInfo" ));
		if ( getPluginInfo ) {
			rdoPlugin::PluginInfo info;
			getPluginInfo( &info );
			name           = info.name;
			version_major  = info.version_major;
			version_minor  = info.version_minor;
			version_build  = info.version_build;
			version_info   = info.version_info;
			description    = info.description;
			defaultRunMode = info.defaultRunMode;
		}
		runMode = getDefaultRunMode();
	}
	runMode      = static_cast<rdoPlugin::PluginRunMode>(AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "runMode", runMode ));
	restoreState = AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "restoreState", restoreState ) ? true : false;
	if ( restoreState ) {
		setState( static_cast<rdoPlugin::PluginState>(AfxGetApp()->GetProfileInt( getProfilePath().c_str(), "state", state )) );
	}
	if ( runMode == rdoPlugin::prmStudioStartUp ) {
		setState( rdoPlugin::psActive );
	} else if ( getState() == rdoPlugin::psStoped && lib ) {
		::FreeLibrary( lib );
		lib = NULL;
	}
}

RDOStudioPlugin::~RDOStudioPlugin()
{
	restoreState = false;
	setState( rdoPlugin::psStoped );
}

bool RDOStudioPlugin::isRDOStudioPlugin( const std::string& modulName )
{
	bool res = false;
	HMODULE local_lib = ::LoadLibrary( modulName.c_str() );
	if ( local_lib ) {
		rdoPlugin::PFunGetPluginInfo getPluginInfo = reinterpret_cast<rdoPlugin::PFunGetPluginInfo>(::GetProcAddress( local_lib, "getPluginInfo" ));
		if ( getPluginInfo ) {
			res = true;
		}
		::FreeLibrary( local_lib );
	}
	return res;
}

std::string RDOStudioPlugin::getProfilePath() const
{
	return format( "plugins\\%s_%d.%d.%d", name.c_str(), version_major, version_minor, version_build );
}

void RDOStudioPlugin::setState( const rdoPlugin::PluginState value )
{
	if ( state != value ) {
		state = value;
		if ( state == rdoPlugin::psActive ) {
			if ( !lib ) {
				lib = ::LoadLibrary( modulName.c_str() );
				if ( !lib ) {
					state = rdoPlugin::psStoped;
				}
			}
			if ( state == rdoPlugin::psActive ) {
				rdoPlugin::PFunStartPlugin startPlugin = reinterpret_cast<rdoPlugin::PFunStartPlugin>(::GetProcAddress( lib, "startPlugin" ));
				if ( !startPlugin || !startPlugin( plugins->getStudio() ) ) {
					state = rdoPlugin::psStoped;
				}
			}
		}
		if ( state == rdoPlugin::psStoped ) {
			if ( lib ) {
				rdoPlugin::PFunStopPlugin stopPlugin = reinterpret_cast<rdoPlugin::PFunStopPlugin>(::GetProcAddress( lib, "stopPlugin" ));
				if ( stopPlugin ) {
					stopPlugin();
				}
				::FreeLibrary( lib );
				lib = NULL;
			}
		}
		if ( getRestoreState() ) {
			AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "state", state );
		}
	}
}

void RDOStudioPlugin::setRestoreState( const bool value )
{
	if ( restoreState != value ) {
		restoreState = value;
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "restoreState", restoreState );
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "state", state );
	}
}

void RDOStudioPlugin::setRunMode( const rdoPlugin::PluginRunMode value )
{
	if ( runMode != value ) {
		runMode = value;
		if ( runMode == rdoPlugin::prmModelStartUp && !model->isRunning() ) {
			setState( rdoPlugin::psStoped );
		}
		AfxGetApp()->WriteProfileInt( getProfilePath().c_str(), "runMode", runMode );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioPlugins
// ----------------------------------------------------------------------------
RDOStudioPlugins* plugins;

RDOStudioPlugins::RDOStudioPlugins()
{
	plugins = this;

	studio.model.hasModel         = RDOStudioPlugins::hasModel;
	studio.model.build            = RDOStudioPlugins::buildModel;
	studio.model.run              = RDOStudioPlugins::runModel;
	studio.model.stop             = RDOStudioPlugins::stopModel;
	studio.model.isRunning        = RDOStudioPlugins::isModelRunning;
	studio.model.getShowMode      = RDOStudioPlugins::getModelShowMode;
	studio.model.setShowMode      = RDOStudioPlugins::setModelShowMode;
	studio.model.isFrmDescribed   = RDOStudioPlugins::isModelFrmDescribed;
	studio.model.getShowRate      = RDOStudioPlugins::getModelShowRate;
	studio.model.setShowRate      = RDOStudioPlugins::setModelShowRate;
	studio.model.showNextFrame    = RDOStudioPlugins::showModelNextFrame;
	studio.model.showPrevFrame    = RDOStudioPlugins::showModelPrevFrame;
	studio.model.canShowNextFrame = RDOStudioPlugins::canShowModelNextFrame;
	studio.model.canShowPrevFrame = RDOStudioPlugins::canShowModelPrevFrame;
	studio.model.getFrameCount    = RDOStudioPlugins::modelGetFrameCount;
	studio.model.getFrameName     = RDOStudioPlugins::getModelFrameName;
	studio.model.showFrame        = RDOStudioPlugins::modelShowFrame;
	studio.model.closeAllFrame    = RDOStudioPlugins::modelCloseAllFrame;

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, modelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, modelStopNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, modelStopNotify );
	kernel.setNotifyReflect( RDOKernel::executeError, modelStopNotify );

	init();
}

RDOStudioPlugins::~RDOStudioPlugins()
{
	std::vector< RDOStudioPlugin* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();

	plugins = NULL;
}

void RDOStudioPlugins::enumPlugins( const std::string& mask )
{
	CFileFind finder;
	if ( finder.FindFile( mask.c_str() ) ) {
		BOOL flag = TRUE;
		while ( flag ) {
			flag = finder.FindNextFile();
			if ( finder.IsDirectory() && !finder.IsDots() ) {
				std::string subDir = finder.GetFilePath();
				subDir += "\\*.*";
				enumPlugins( subDir );
			} else if ( !finder.IsDots() ) {
				std::string fullname = finder.GetFilePath();
				if ( fullname.rfind( ".rsp" ) == fullname.length() - 4 ) {
					std::string modulName = finder.GetFilePath();
					if ( RDOStudioPlugin::isRDOStudioPlugin( modulName ) ) {
						RDOStudioPlugin* plugin = new RDOStudioPlugin( modulName );
						list.push_back( plugin );
					}
				}
			}
		}
	}
	finder.Close();
}

void RDOStudioPlugins::init()
{
	std::string path = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		path = RDOStudioApp::extractFilePath( szExeName );
	}
	if ( !path.empty() ) {
		enumPlugins( path + "\\plugins\\*.*" );
	}
/*
	int i = 1;
	std::vector< RDOStudioPlugin* >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		RDOStudioPlugin* plugin = *it;
		TRACE( "%d. plugin name = %s, version %d.%d, build %d, version info = %s, description = %s\r\n", i++, plugin->name.c_str(), plugin->version_major, plugin->version_minor, plugin->version_build, plugin->version_info.c_str(), plugin->description.c_str() );
		it++;
	}
*/
}

bool RDOStudioPlugins::hasModel()
{
	return model->hasModel();
}

void RDOStudioPlugins::buildModel()
{
	model->buildModel();
}

void RDOStudioPlugins::runModel()
{
	model->runModel();
}

void RDOStudioPlugins::stopModel()
{
	model->stopModel();
}

bool RDOStudioPlugins::isModelRunning()
{
	return model->isRunning();
}

rdoPlugin::ModelShowMode RDOStudioPlugins::getModelShowMode()
{
	switch ( model->getShowMode() ) {
		case RDOSimulatorNS::SM_NoShow   : return rdoPlugin::NoShow;
		case RDOSimulatorNS::SM_Animation: return rdoPlugin::Animation;
		case RDOSimulatorNS::SM_Monitor  : return rdoPlugin::Monitor;
	}
	return rdoPlugin::NoShow;
}

void RDOStudioPlugins::setModelShowMode( rdoPlugin::ModelShowMode showMode )
{
	switch ( showMode ) {
		case rdoPlugin::NoShow   : model->setShowMode( RDOSimulatorNS::SM_NoShow ); break;
		case rdoPlugin::Animation: model->setShowMode( RDOSimulatorNS::SM_Animation ); break;
		case rdoPlugin::Monitor  : model->setShowMode( RDOSimulatorNS::SM_Monitor ); break;
	}
}

bool RDOStudioPlugins::isModelFrmDescribed()
{
	return model->isFrmDescribed();
}

double RDOStudioPlugins::getModelShowRate()
{
	return model->getShowRate();
}

void RDOStudioPlugins::setModelShowRate( double value )
{
	model->setShowRate( value );
}

void RDOStudioPlugins::showModelNextFrame()
{
	model->showNextFrame();
}

void RDOStudioPlugins::showModelPrevFrame()
{
	model->showPrevFrame();
}

bool RDOStudioPlugins::canShowModelNextFrame()
{
	return model->canShowNextFrame();
}

bool RDOStudioPlugins::canShowModelPrevFrame()
{
	return model->canShowPrevFrame();
}

int RDOStudioPlugins::modelGetFrameCount()
{
	return model->getFrameCount();
}

const char* RDOStudioPlugins::getModelFrameName( int index )
{
	return model->getFrameName( index );
}

void RDOStudioPlugins::modelShowFrame( int index )
{
	model->showFrame( index );
}

void RDOStudioPlugins::modelCloseAllFrame()
{
	model->closeAllFrame();
}

void RDOStudioPlugins::modelStartNotify()
{
	AfxGetApp()->PostThreadMessage( PLUGIN_STARTMODEL_MESSAGE, 0, 0 );
}

void RDOStudioPlugins::modelStopNotify()
{
	AfxGetApp()->PostThreadMessage( PLUGIN_STOPMODEL_MESSAGE, 0, 0 );
}

void RDOStudioPlugins::modelStart()
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psActive );
			}
			it++;
		}
	}
}

void RDOStudioPlugins::modelStop()
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->getRunMode() == rdoPlugin::prmModelStartUp ) {
				plugin->setState( rdoPlugin::psStoped );
			}
			it++;
		}
	}
}

void RDOStudioPlugins::stopPlugin( const HMODULE lib ) const
{
	if ( plugins ) {
		std::vector< RDOStudioPlugin* >::const_iterator it = list.begin();
		while ( it != list.end() ) {
			RDOStudioPlugin* plugin = *it;
			if ( plugin->lib == lib ) {
				plugin->setState( rdoPlugin::psStoped );
				break;
			}
			it++;
		}
	}
}

int RDOStudioPlugins::comparePluginsByName( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		return strcmp( plugin1->getName().c_str(), plugin2->getName().c_str() );
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByVersion( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getVersionMajor() > plugin2->getVersionMajor() ) {
			return 1;;
		} else if ( plugin1->getVersionMajor() < plugin2->getVersionMajor() ) {
			return -1;
		} else if ( plugin1->getVersionMinor() > plugin2->getVersionMinor() ) {
			return 1;
		} else if ( plugin1->getVersionMinor() < plugin2->getVersionMinor() ) {
			return -1;
		} else if ( plugin1->getVersionBuild() > plugin2->getVersionBuild() ) {
			return 1;
		} else if ( plugin1->getVersionBuild() < plugin2->getVersionBuild() ) {
			return -1;
		} else {
			return strcmp( plugin1->getVersionInfo().c_str(), plugin2->getVersionInfo().c_str() );
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByRunMode( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getRunMode() > plugin2->getRunMode() ) {
			return 1;;
		} else if ( plugin1->getRunMode() < plugin2->getRunMode() ) {
			return -1;
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByState( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		if ( plugin1->getState() > plugin2->getState() ) {
			return 1;;
		} else if ( plugin1->getState() < plugin2->getState() ) {
			return -1;
		}
	}
	return 0;
}

int RDOStudioPlugins::comparePluginsByDescription( const RDOStudioPlugin* plugin1, const RDOStudioPlugin* plugin2 )
{
	if ( plugin1 && plugin2 ) {
		return strcmp( plugin1->getDescription().c_str(), plugin2->getDescription().c_str() );
	}
	return 0;
}
