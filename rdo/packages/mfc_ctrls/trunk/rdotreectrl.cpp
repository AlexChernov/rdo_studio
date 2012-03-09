/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotreectrl.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "ui/mfc_ctrls/rdo_ctrls_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdotreectrl.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTreeCtrl
// --------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( RDOTreeCtrl, CTreeCtrl )

BEGIN_MESSAGE_MAP( RDOTreeCtrl, CTreeCtrl )
END_MESSAGE_MAP()

RDOTreeCtrl::RDOTreeCtrl(): CTreeCtrl()
{
}

RDOTreeCtrl::~RDOTreeCtrl()
{
}

BOOL RDOTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	return CTreeCtrl::PreCreateWindow(cs);
}

void RDOTreeCtrl::deleteChildren( const HTREEITEM parent )
{
	HTREEITEM nextItem;
	HTREEITEM childItem = GetChildItem( parent );

	while ( childItem )	{
		nextItem = GetNextItem( childItem, TVGN_NEXT );
		DeleteItem( childItem );
		childItem = nextItem;
	}

}
