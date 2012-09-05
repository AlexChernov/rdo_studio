/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      workspace.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_WORKSPACE_H_
#define _RDO_STUDIO_MFC_WORKSPACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdotabctrl.h"
#include "app/rdo_studio_mfc/src/dock_wnd.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_pagectrl.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioWorkspace
// --------------------------------------------------------------------------------
class RDOStudioWorkspace: public QWidget
{
friend class RDOStudioFrameManager;
public:
	RDOStudioWorkspace(PTR(QWidget) pParent);
	virtual ~RDOStudioWorkspace();

	RPPageCtrl* pagectrl;
	RPPageCtrlItem* prepareNewPage()                                 { return pagectrl->prepareNewPage();         }
	RPPageCtrlItem* insertPage( CWnd* wnd, const rp::string& label ) { return pagectrl->insertPage( wnd, label ); }
	void removePage( RPPageCtrlItem* page )                          { pagectrl->removePage( page );              }
	void selectFirst()                                               { pagectrl->selectFirst();                   }

private:
	typedef  QWidget  parent_type;

	CWnd                    m_thisCWnd;
	RDOTabCtrl              tab;
	RDOStudioFrameTreeCtrl* frames;

	void resizeEvent(PTR(QResizeEvent) event);
};

#endif // _RDO_STUDIO_MFC_WORKSPACE_H_
