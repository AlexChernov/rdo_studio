// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////
#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_create.h"
#include "rdoprocess_shape_create_dlg1.h"
#include "rdoprocess_method_proc2rdo.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"
#include "rdo_studio/rdostudioapp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeCreateMJ::RPShapeCreateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Create") )
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// ������������� ���������� ��� �������������
	gname; // ���
	gfirst=0; // ����� �������
	gamount=100; // ���-�� �����������
	gtype=0; // ����� ��������
	base_gen=1234567890;
	//�������� �������
	gexp=0;
	gdisp=0;

	//������ ����
	inf=1000000; // ������������
	gID=1; // ID ������
	gpar1=0;
	gpar2=0;
	gpar3=0;

}

RPShapeCreateMJ::~RPShapeCreateMJ()
{
}

rp::RPXMLNode* RPShapeCreateMJ::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

/*void RPShapeCreateMJ::list_name()
{
	RPShape_MJ::list_name();
	TRACE( "*** Create ***\n" );
}
*/


void RPShapeCreateMJ::generate()
{
	std::vector <double>  params;
	params.push_back(static_cast<double>(gtype));
	params.push_back(static_cast<double>(base_gen));
	params.push_back(gfirst);
	params.push_back(gamount);
	params.push_back(gexp);
	params.push_back(gdisp);

	studioApp.broadcastMessage(RDOThread::RT_PROCGUI_BLOCK_CREATE,&params);

	/*
	RDOfiles->function <<"��� ���������� ����� - "<<id_next
	<<std::endl<<"��� - "<<getName().c_str()
	<<std::endl<<"����� ������� - "<<gfirst
	<<std::endl<< "���-�� ����������� - "<<gamount
	<<std::endl<<"����� �������� - "<<gtype
	<<std::endl<<"���� ���������� - "<<base_gen
	<<std::endl<<"�������� ������ - "<<"exp - "<<gexp<<" disp"<<gdisp<<" max - " <<gmax<<" min - "<<gmin
	<<std::endl<<"������ ����! ������������ - "<<inf
	<<std::endl<<"ID ������ - "<<gID
	<<std::endl<<"��������� - "<<"1-  "<<gpar1<< " 2 - "<<gpar2<<" 3 - "<<gpar3;
	*/

	/*
	int cur = gtype;
	switch(cur) // ���������� �������� ���� ������ �� ������
	{
		case 0: // ��������� 
			break;	
		case 1: // ����������
			break;
		case 2: // ����������� �����
			break;
		case 3: // ����������������
			break;
	}*/


}