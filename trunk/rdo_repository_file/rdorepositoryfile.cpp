#include "stdafx.h"
#include "rdorepositoryfile.h"
#include "../rdo_studio/resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>

#include <fstream>
#include <afxdlgs.h>

using namespace std;
using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDORepositoryFile
// ----------------------------------------------------------------------------
namespace rdoRepository {
std::string format( UINT resource, ... )
{
	CString str;
	if ( str.LoadString( resource ) ) {
		std::vector< char > s;
		s.resize( 256 );
		va_list paramList;
		int size = -1;
		while ( size == -1 ) {
			va_start( paramList, resource );
			size = _vsnprintf( s.begin(), s.size(), static_cast<LPCTSTR>(str), paramList );
			va_end( paramList );
			if ( size == -1 ) {
				s.resize( s.size() + 256 );
			}
		}
		s.resize( size );
		return std::string( s.begin(), s.end() );
	}
	return "";
}
} // namespace rdoRepository

RDORepositoryFile::RDORepositoryFile():
	modelName( "" ),
	modelPath( "" ),
	lastModelPath( "" ),
	patFileName( "" ),
	rtpFileName( "" ),
	rssFileName( "" ),
	oprFileName( "" ),
	frmFileName( "" ),
	funFileName( "" ),
	dptFileName( "" ),
	smrFileName( "" ),
	pmdFileName( "" ),
	pmvFileName( "" ),
	trcFileName( "" ),
	readOnly( false )
{
}

RDORepositoryFile::~RDORepositoryFile()
{
}

void RDORepositoryFile::newModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
		modelName   = "noname";
		modelPath   = "";
		patFileName = modelName;
		rtpFileName = modelName;
		rssFileName = modelName;
		oprFileName = modelName;
		frmFileName = modelName;
		funFileName = modelName;
		dptFileName = modelName;
		smrFileName = modelName;
		pmdFileName = modelName;
		pmvFileName = modelName;
		trcFileName = modelName;
		changeLastModelPath();
		kernel.notify( RDOKernel::newModel );
	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}
}

bool RDORepositoryFile::openModel( const string& modelFileName )
{
	if ( canCloseModel() ) {

		realCloseModel();

		bool flag = true;
		readOnly = false;
		if ( modelFileName.empty() ) {
			CString s;
			s.LoadString( ID_MODEL_FILETYPE );
			CFileDialog dlg( true, "smr", lastModelPath.c_str(), 0, s, AfxGetMainWnd() );
			flag = dlg.DoModal() == IDOK;
			readOnly = dlg.GetReadOnlyPref() == TRUE;
			if ( flag ) {
				extractName( &dlg );
			}
		} else {
			extractName( modelFileName );
			flag = !modelName.empty();
		}

		if ( flag ) {
			if ( isFileExists( modelPath + modelName + ".smr" ) ) {

				smrFileName = modelName;
				stringstream smrStream;
				loadFile( modelPath + smrFileName + ".smr", smrStream );
				rdoModelObjects::RDOSMRFileInfo fileInfo;
				kernel.getSimulator()->parseSMRFileInfo( smrStream, fileInfo );

				patFileName = fileInfo.model_name.empty()     ? smrFileName : fileInfo.model_name;
 				rtpFileName = fileInfo.model_name.empty()     ? smrFileName : fileInfo.model_name;
 				rssFileName = fileInfo.resource_file.empty()  ? smrFileName : fileInfo.resource_file;
 				oprFileName = fileInfo.oprIev_file.empty()    ? smrFileName : fileInfo.oprIev_file;
 				frmFileName = fileInfo.frame_file.empty()     ? smrFileName : fileInfo.frame_file;
 				funFileName = fileInfo.model_name.empty()     ? smrFileName : fileInfo.model_name;
 				dptFileName = fileInfo.model_name.empty()     ? smrFileName : fileInfo.model_name;
 				pmdFileName = fileInfo.statistic_file.empty() ? smrFileName : fileInfo.statistic_file;
 				pmvFileName = fileInfo.results_file.empty()   ? smrFileName : fileInfo.results_file;
 				trcFileName = fileInfo.trace_file.empty()     ? smrFileName : fileInfo.trace_file;

				kernel.notify( RDOKernel::openModel );
				return true;

			} else {
				AfxMessageBox( format( ID_MSG_MODELOPEN_ERROR, string(modelPath + modelName + ".smr").c_str() ).c_str(), MB_ICONSTOP | MB_OK );
				setName( "" );
			}
		}

	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}

	return false;
}

bool RDORepositoryFile::saveModel()
{
	bool flag = true;
	if ( modelPath.empty() ) {
		flag = saveAsDlg();
	}
	if ( flag ) {
		kernel.notify( RDOKernel::saveModel );
	}
	return flag;
}

void RDORepositoryFile::saveAsModel()
{
	if ( saveAsDlg() ) {
		kernel.notify( RDOKernel::saveModel );
	}
}

bool RDORepositoryFile::saveAsDlg()
{
	CString s;
	s.LoadString( ID_MODEL_FILETYPE );
	CFileDialog dlg( false, "smr", lastModelPath.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, s );

	if ( dlg.DoModal() == IDOK ) {

		extractName( &dlg );

		if ( modelName.empty() ) return false;

		patFileName = modelName;
 		rtpFileName = modelName;
 		rssFileName = modelName;
 		oprFileName = modelName;
 		frmFileName = modelName;
 		funFileName = modelName;
 		dptFileName = modelName;
		smrFileName = modelName;
 		pmdFileName = modelName;
 		pmvFileName = modelName;
 		trcFileName = modelName;

		return true;

	}
	return false;
}

bool RDORepositoryFile::canCloseModel() const
{
	return kernel.boolNotifyAnd( RDOKernel::canCloseModel );
}

void RDORepositoryFile::realCloseModel()
{
	kernel.notify( RDOKernel::closeModel );
	modelName   = "";
	modelPath   = "";
	patFileName = "";
	rtpFileName = "";
	rssFileName = "";
	oprFileName = "";
	frmFileName = "";
	funFileName = "";
	dptFileName = "";
	smrFileName = "";
	pmdFileName = "";
	pmvFileName = "";
	trcFileName = "";
	changeLastModelPath();
}

void RDORepositoryFile::closeModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}
}

void RDORepositoryFile::extractName( const CFileDialog* const dlg )
{
	if ( !dlg ) return;

	string fullname  = dlg->GetPathName();
	string filename  = dlg->GetFileName();
	string extention = dlg->GetFileExt();

	modelPath = extractFilePath( fullname );

	if ( extention.empty() ) {
		setName( filename );
	} else {
		string name( filename.begin(), filename.length() - extention.length() - 1 );
		setName( name );
	}
}

void RDORepositoryFile::extractName( const string& fullname )
{
	modelPath = extractFilePath( fullname );

	string name = fullname;
	string::size_type pos = name.find_last_of( '.' );
	if ( pos != string::npos ) {
		string s;
		s.assign( name.begin(), pos );
		name = s;
	}
	static char szDelims[] = " \t\n\r";
	name.erase( 0, name.find_first_not_of( szDelims ) );
	name.erase( name.find_last_not_of( szDelims ) + 1, string::npos );
	pos = name.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = name.find_last_of( '/' );
	}
	if ( pos != string::npos ) {
		string s( name, pos + 1, name.length() - pos );
		setName( s );
	} else {
		setName( "" );
	}
}

string RDORepositoryFile::extractFilePath( const string& fileName )
{
	string s;
	string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos != string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool RDORepositoryFile::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

void RDORepositoryFile::changeLastModelPath()
{
	string name = !modelName.empty() ? modelName + ".smr" : "*.smr";
	if ( !modelPath.empty() ) {
		lastModelPath = modelPath + name;
	} else {
		TCHAR buff[ MAX_PATH ];
		::GetCurrentDirectory( MAX_PATH, buff );
		lastModelPath = buff;
		if ( !lastModelPath.empty() ) {
			if ( lastModelPath[ lastModelPath.length()-1 ] != '\\' && lastModelPath[ lastModelPath.length()-1 ] != '/' ) {
				lastModelPath += "\\";
			}
		}
		lastModelPath += name;
	}
}

string RDORepositoryFile::getName() const
{
	return modelName;
}

string RDORepositoryFile::getFullName() const
{
	return modelPath + smrFileName + ".smr";
}

void RDORepositoryFile::setName( const string& str )
{
	modelName = str;
	static char szDelims[] = " \t\n\r";
	modelName.erase( 0, modelName.find_first_not_of( szDelims ) );
	modelName.erase( modelName.find_last_not_of( szDelims ) + 1, string::npos );
	if ( modelName.empty() ) {
		modelPath   = "";
		patFileName = "";
		rtpFileName = "";
		rssFileName = "";
		oprFileName = "";
		frmFileName = "";
		funFileName = "";
		dptFileName = "";
		smrFileName = "";
		pmdFileName = "";
		pmvFileName = "";
		trcFileName = "";
	}
	changeLastModelPath();
}

bool RDORepositoryFile::isReadOnly() const
{
	return readOnly;
}

void RDORepositoryFile::loadFile( const string& filename, stringstream& stream ) const
{
	if ( isFileExists( filename ) ) {
		ifstream file( filename.c_str() );
		stream << file.rdbuf();
		file.close();
	} else {
		stream.setstate( ios_base::badbit );
	}
}

void RDORepositoryFile::saveFile( const string& filename, stringstream& stream, const bool deleteIfEmpty ) const
{
	if ( !filename.empty() ) {
		bool file_exist = isFileExists( filename );
		if ( stream.str().length() || ( file_exist && !deleteIfEmpty ) ) {
			ofstream file( filename.c_str() );
			file << stream.str();
			file.close();
		} else {
			if ( file_exist && deleteIfEmpty ) {
				CFile::Remove( filename.c_str() );
			}
		}
	}
}

void RDORepositoryFile::loadPAT( stringstream& stream ) const
{
	loadFile( modelPath + patFileName + ".pat", stream );
}

void RDORepositoryFile::loadRTP( stringstream& stream ) const
{
	loadFile( modelPath + rtpFileName + ".rtp", stream );
}

void RDORepositoryFile::loadRSS( stringstream& stream ) const
{
	loadFile( modelPath + rssFileName + ".rss", stream );
}

void RDORepositoryFile::loadOPR( stringstream& stream ) const
{
	loadFile( modelPath + oprFileName + ".opr", stream );
}

void RDORepositoryFile::loadFRM( stringstream& stream ) const
{
	loadFile( modelPath + frmFileName + ".frm", stream );
}

void RDORepositoryFile::loadFUN( stringstream& stream ) const
{
	loadFile( modelPath + funFileName + ".fun", stream );
}

void RDORepositoryFile::loadDPT( stringstream& stream ) const
{
	loadFile( modelPath + dptFileName + ".dpt", stream );
}

void RDORepositoryFile::loadSMR( stringstream& stream ) const
{
	loadFile( modelPath + smrFileName + ".smr", stream );
}

void RDORepositoryFile::loadPMD( stringstream& stream ) const
{
	loadFile( modelPath + pmdFileName + ".pmd", stream );
}

void RDORepositoryFile::loadPMV( stringstream& stream ) const
{
	loadFile( modelPath + pmvFileName + ".pmv", stream );
}

void RDORepositoryFile::loadTRC( stringstream& stream ) const
{
	loadFile( modelPath + trcFileName + ".trc", stream );
}

void RDORepositoryFile::savePAT( stringstream& stream ) const
{
	saveFile( modelPath + patFileName + ".pat", stream );
}

void RDORepositoryFile::saveRTP( stringstream& stream ) const
{
	saveFile( modelPath + rtpFileName + ".rtp", stream );
}

void RDORepositoryFile::saveRSS( stringstream& stream ) const
{
	saveFile( modelPath + rssFileName + ".rss", stream );
}

void RDORepositoryFile::saveOPR( stringstream& stream ) const
{
	saveFile( modelPath + oprFileName + ".opr", stream, true );
}

void RDORepositoryFile::saveFRM( stringstream& stream ) const
{
	saveFile( modelPath + frmFileName + ".frm", stream );
}

void RDORepositoryFile::saveFUN( stringstream& stream ) const
{
	saveFile( modelPath + funFileName + ".fun", stream );
}

void RDORepositoryFile::saveDPT( stringstream& stream ) const
{
	saveFile( modelPath + dptFileName + ".dpt", stream, true );
}

void RDORepositoryFile::saveSMR( stringstream& stream ) const
{
	saveFile( modelPath + smrFileName + ".smr", stream );
}

void RDORepositoryFile::savePMD( stringstream& stream ) const
{
	saveFile( modelPath + pmdFileName + ".pmd", stream );
}

void RDORepositoryFile::savePMV( stringstream& stream ) const
{
	saveFile( modelPath + pmvFileName + ".pmv", stream );
}

void RDORepositoryFile::saveTRC( stringstream& stream ) const
{
	saveFile( modelPath + trcFileName + ".trc", stream );
}

void RDORepositoryFile::loadBMP( const std::string& name, std::stringstream& stream ) const
{
	string file_name = modelPath + name + ".bmp";
	if ( isFileExists( file_name ) ) {
		ifstream file( file_name.c_str() );
		stream << file.rdbuf();
		file.close();
	} else {
		stream.setstate( ios_base::badbit );
	}
}
