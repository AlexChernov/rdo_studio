#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdostudioframetreectrl.h"
#include "rdo_studio/rdostudiomodel.h"
#include "rdo_studio/rdostudioframemanager.h"
#include "rdo_studio/rdostudioframedoc.h"
#include "rdo_studio/rdostudioframeview.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/rdostudiomainfrm.h"
#include "rdo_studio/resource.h"
#include "rdo_studio/htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTreeCtrl
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFrameTreeCtrl, RDOTreeCtrl)
	//{{AFX_MSG_MAP(RDOStudioFrameTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioFrameTreeCtrl::RDOStudioFrameTreeCtrl()
{
}

RDOStudioFrameTreeCtrl::~RDOStudioFrameTreeCtrl()
{
}

BOOL RDOStudioFrameTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int RDOStudioFrameTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOTreeCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 5, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_FRAMES_TREECTRL );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );
	InsertItem( rdo::format( IDS_FRAMES ).c_str(), 0, 0 );

	return 0;
}

void RDOStudioFrameTreeCtrl::expand()
{
	Expand( GetRootItem(), TVE_EXPAND );
}

void RDOStudioFrameTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);

	if ( model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed ) {

		UINT uFlags;
		HTREEITEM hitem = HitTest( point, &uFlags );

		if ( hitem && ( TVHT_ONITEM & uFlags ) && hitem != GetRootItem() ) {
			int index = model->m_frameManager.findFrameIndex( hitem );
			if ( index != -1 ) {
				RDOStudioFrameDoc* doc = model->m_frameManager.getFrameDoc( index );
				if ( !doc ) {
					model->m_frameManager.connectFrameDoc( index );
				} else {
					studioApp.mainFrame->MDIActivate( doc->getView()->GetParentFrame() );
				}
			}
		}
	}
}

void RDOStudioFrameTreeCtrl::OnHelpKeyword()
{
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_model_frame.htm#frame";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
