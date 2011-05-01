#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_method_proc2rdo_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_create_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_process_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_terminate_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_generation_type_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/res/method_big.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/method_small.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/generate.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/generate_setup.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_create.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_decide.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_terminate.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_process.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_resource.xpm"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_object_chart.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/rdostudiomodel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RPMethodProc2RDO_MJ* proc2rdo = NULL;

RPMethodProc2RDO_MJ::RPMethodProc2RDO_MJ( RPObject* _parent ):
	rpMethod::RPMethod( _parent, "���-�������" ),
	RPObject_MJ( get_this() ),
	btn_generate( -1 ),
	btn_generate_setup( -1 ),
	RDOfiles( NULL ) // �������������
{
	proc2rdo = this;
	generate_time_MJ = 100;
	RDOfiles = new RPCreationRDOFilesMJ();
	pixmap_big   = rpMethod::project->createBitmap( method_big_xpm );
	pixmap_small = rpMethod::project->createBitmap( method_small_xpm );
}

RPMethodProc2RDO_MJ::~RPMethodProc2RDO_MJ()
{
	delete RDOfiles;
	proc2rdo = NULL;
}

void RPMethodProc2RDO_MJ::registerObject()
{
	// ������� �����
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPObject_MJ", NULL ) );

	// ������� RPMethod
	RPObjectClassInfo* class_info = new RPObjectClassInfo( "RPMethodProc2RDO_MJ", "RPMethod" );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// ������� RPObjectFlowChart
	class_info = new RPObjectClassInfo( "RPObjectFlowChart_MJ", "RPObjectFlowChart", RPObjectFlowChart_MJ::newObject, this, _T("���-�������") );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// ������� RPShape
	class_info = new RPObjectClassInfo( "RPShape_MJ", "RPShape" );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// ������� RPShape_MJ
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeCreateMJ", "RPShape_MJ", RPShapeCreateMJ::newObject, this, _T("���������"), block_create_xpm, 0 ) );

	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeTerminateMJ", "RPShape_MJ", RPShapeTerminateMJ::newObject, this, _T("����������"), block_terminate_xpm, 1 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeProcessMJ", "RPShape_MJ", RPShapeProcessMJ::newObject, this, _T("�������"), block_process_xpm, 2 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeResource_MJ", "RPShape_MJ", RPShapeResource_MJ::newObject, this, _T("������"), block_resource_xpm, 3 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeDecide", "RPShape_MJ", RPShapeDecide::newObject, this, _T("������"), block_decide_xpm, 4 ) );

	RPCtrlToolbar* toolbar = rpMethod::project->createToolBar( _T("���-�������") );
	btn_generate       = toolbar->insertButton( this, generate_xpm, _T("������� ������") );
	btn_generate_setup = toolbar->insertButton( this, generate_setup_xpm, _T("���������") );
}

rpMethod::RPMethod* RPMethodProc2RDO_MJ::registerMethod()
{
	new RPMethodProc2RDO_MJ( NULL );
	proc2rdo->registerObject();

	return proc2rdo;
}

RPObjectFlowChart* RPMethodProc2RDO_MJ::makeFlowChart( RPObject* parent )
{
	return new RPObjectFlowChart_MJ( parent );
}

void RPMethodProc2RDO_MJ::buttonCommand( int button_id )
{
	if ( button_id == btn_generate ) {
		generate();
//		model->closeModel();
//		model->openModel();
	} else if ( button_id == btn_generate_setup ) {
		RP_GENERATION_TYPE_MJ dlg( AfxGetMainWnd() );
		dlg.DoModal();	
	}
}

void RPMethodProc2RDO_MJ::buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update )
{
	button_update.enable = true;
}

void RPMethodProc2RDO_MJ::generate()
{
	std::list< RPObject* > all_child;
	// ������� ��������� � ��������
	all_child.clear();
	rpMethod::project->getAllChildByClass( all_child, "RPShape_MJ", true );
	std::list< RPObject* >::iterator shape_it = all_child.begin();
	while ( shape_it != all_child.end() ) {
		dynamic_cast<RPObject_MJ*>(static_cast<RPShape_MJ*>(*shape_it))->generate();
		shape_it++;
	}
}

bool RPMethodProc2RDO_MJ::checkModelStructure()
{
// ��������� ID
	std::list< RPObject* > all_child;
	std::list< rp::string > class_names;
	class_names.push_back( "RPShapeCreateMJ" );
	class_names.push_back( "RPShapeProcessMJ" );
	class_names.push_back( "RPShapeTerminateMJ" );
	class_names.push_back( "RPShapeDecide" );
	rpMethod::project->getAllChildByClasses( all_child, class_names, true );
	if(all_child.size()>0)
	{
		std::list< RPObject* >::const_iterator block_it = all_child.begin();
		while ( block_it != all_child.end() )
		{
			std::list< RPShape* > list = static_cast<RPShape*>(*block_it)->getNextBlock();
			if(block_it == all_child.begin() && (*block_it)->getClassName()!="RPShapeCreateMJ")
			{
				TRACE1( "%s\n", "������ ������ ��������� ���� Create" );
				return false;
			}
			else
			{
				if ( !list.empty() )
				{
					dynamic_cast<RPObject_MJ*>(*block_it)->id_next = list.front()->getName();
					rp::string name1 = dynamic_cast<RPObject_MJ*>(*block_it)->id_next;
					if((*block_it)->getClassName()=="RPShapeDecide"){
						if(list.size()==2)
						{
							dynamic_cast<RPObject_MJ*>(*block_it)->id_next2 = list.back()->getName();
							rp::string name2 = dynamic_cast<RPObject_MJ*>(*block_it)->id_next2;
						}
						else
						{
							rp::string error="���� "+(*block_it)->getName()+" ������ ����� 2 �����!!\n� �� ����!!";
							TRACE1( "%s\n", error.c_str() );
							return false;
						}
					}
				}else{
					if((*block_it)->getClassName()!="RPShapeTerminateMJ")
					{
						rp::string error="���� "+(*block_it)->getName()+" ������ ����� ���������";
						TRACE1( "%s\n", error.c_str() );
						return false;
					}
				}
				block_it++;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
