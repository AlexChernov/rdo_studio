/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_childfrm.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ������� ������ (dronbas@gmail.com)
  \date      10.01.2006
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_childfrm.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_docview.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_flowchart.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPChildFrame
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RPChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(RPChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

RPChildFrame::RPChildFrame():
	RDOStudioChildFrame(),
	view( NULL )
{
}

RPChildFrame::~RPChildFrame()
{
}

BOOL RPChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	/// @todo Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

int RPChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( CMDIChildWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	return 0;
}

void RPChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus( pOldWnd );
//	if ( flowchart ) flowchart->SetFocus();
}

#ifdef _DEBUG
void RPChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void RPChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void RPChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

void RPChildFrame::OnMDIActivate( BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd )
{
	if ( view ) view->getFlowchart()->flowobj->setActive( bActivate ? true : false );
	CMDIChildWnd::OnMDIActivate( bActivate, pActivateWnd, pDeactivateWnd );
}
