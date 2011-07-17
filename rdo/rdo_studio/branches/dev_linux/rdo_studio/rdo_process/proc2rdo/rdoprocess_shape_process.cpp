// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include <list>
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_process.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "rdo_studio/rdostudioapp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RPShapeProcessMJ::RPShapeProcessMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Process") )
{
	
	gname=_T("Process"); // ���
	
	gtype = 0;
	base_gen = 1234567;
	
	//�������� �������
	gexp=0;
	gdisp=0;
	gmax=0;
	gmin=0;

	action=0; // ��� �������� �� ��������� � �������
	prior=0;
	queue=0;
	parameter=888; // ���� �� ������ ���� ��������
		
		// ������������� ���� ����� 
	type ="block";
		
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point( -50, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point(  0, 25), 270, "resource" ) );
}

RPShapeProcessMJ::~RPShapeProcessMJ()
{
}

RPObject* RPShapeProcessMJ::newObject( RPObject* parent )
{
	return new RPShapeProcessMJ( parent );
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeProcessDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeProcessMJ::generate()
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

	m_pParams = rdo::Factory<RPShapeDataBlockProcess>::create(zakon, gname);
	ASSERT(m_pParams);
	m_pParams->setBase(base_gen);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);

	switch(action)
	{
		case 0://advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 1://sieze,advance,release
			m_pParams->addAction(RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(RPShapeDataBlockProcess::A_RELEASE);
			break;
		case 2://seize,advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 3://seize,advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(RPShapeDataBlockProcess::A_RELEASE);
			break;
	}

	std::list<CString>::iterator it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
		m_pParams->addRes(static_cast<tstring>(*it));
		it++;
	}

	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_PROCESS, m_pParams.get());
	m_pParams = NULL;
}