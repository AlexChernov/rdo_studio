// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////
#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
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

RPShapeCreateMJ::RPShapeCreateMJ(PTR(RPObject) _parent)
	: RPShape_MJ(_parent, _T("Create"))
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// ������������� ���������� ��� �������������
	gname=_T("Create"); // ���
	gfirst=0; // ����� �������
	gamount=100; // ���-�� �����������
	gtype=0; // ����� ��������
	base_gen=1234567890;
	//�������� �������
	gexp=10;
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

void RPShapeCreateMJ::generate()
{
	RPShapeDataBlock::zakonRaspr zakon;
	switch(gtype)
	{
		case 0: // ���������
			zakon = RPShapeDataBlock::Const;
			break;	
		case 1: // ����������
			zakon = RPShapeDataBlock::Normal;
			break;
		case 2: // ����������� �����
			zakon = RPShapeDataBlock::Uniform;
			break;
		case 3: // ����������������
			zakon = RPShapeDataBlock::Exp;
			break;
	}

	m_pParams = rdo::Factory<RPShapeDataBlockCreate>::create(zakon, gname);
	m_pParams->setBase(base_gen);
	m_pParams->setAmount(gamount);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);

	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_CREATE, m_pParams.get());

	m_pParams = NULL;
}
