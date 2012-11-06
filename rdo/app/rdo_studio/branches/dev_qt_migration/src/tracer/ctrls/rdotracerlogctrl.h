/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerlogctrl.h
  \author    ������� �����
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologctrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "app/rdo_studio_mfc/src/help_context_i.h"
// --------------------------------------------------------------------------------

namespace rdoTracer {
class RDOTracerBase;
}

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrlView
// --------------------------------------------------------------------------------
class RDOTracerLogCtrlView
	: public RDOLogCtrl
	, public IHelpContext
{
protected:
	typedef std::map< int, RDOLogColorPair* > RDOColorMap;
	RDOColorMap subitemColors;
	rbool addingSubitems;
	RDOLogColorPair* itemColor;
	virtual rbool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	void showFindError( tstring& findStr );

	CMenu popupMenu;
	rbool bShowMenu;

private:
	DECLARE_IHelpContext;

	afx_msg void OnFind();
	afx_msg void OnFindNext();
	afx_msg void OnFindPrev();
	afx_msg void OnCopy();
	afx_msg LRESULT OnFindReplaceMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnCanCopy( CCmdUI* pCmdUI );
	afx_msg void OnUpdateFindNextPrev( CCmdUI* pCmdUI );
	afx_msg void OnUpdateFind( CCmdUI* pCmdUI );
	//! todo qt
	//afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	//afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	
public:
	RDOTracerLogCtrlView(PTR(QAbstractScrollArea) pParent);
	virtual ~RDOTracerLogCtrlView();

	virtual void addStringToLog( const tstring logStr );

	virtual void setStyle( RDOTracerLogStyle* style, const rbool needRedraw = true );

	virtual void clear();

	rbool getShowMenu() const              { return bShowMenu;  };
	void  setShowMenu( const rbool value ) { bShowMenu = value; };

};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
class RDOTracerLogCtrl: public QAbstractScrollArea
{
public:
	RDOTracerLogCtrl(PTR(QWidget) pParent);

	REF(RDOTracerLogCtrlView) view()
	{
		return *static_cast<PTR(RDOTracerLogCtrlView)>(viewport());
	}

private:
	rbool viewportEvent(PTR(QEvent) pEvent);
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_
