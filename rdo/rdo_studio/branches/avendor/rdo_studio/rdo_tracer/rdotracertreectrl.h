#ifndef RDOTRACERTREECTRL_H
#define RDOTRACERTREECTRL_H
#pragma once

#include <afxole.h>

#include <rdotreectrl.h>
#include "rdotracerrdoclasses.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerTreeCtrl
// ----------------------------------------------------------------------------
class RDODropSource : public COleDropSource
{
protected:

	//{{AFX_VIRTUAL(RDODropSource)
	virtual SCODE GiveFeedback( DROPEFFECT dropEffect );
	//}}AFX_VIRTUAL
public:
	RDODropSource() : COleDropSource() {};
	virtual ~RDODropSource() {};
};

class RDOTracerTreeCtrl: public RDOTreeCtrl  
{
DECLARE_DYNCREATE( RDOTracerTreeCtrl )
private:
	CImageList imageList;

	HTREEITEM rootItem;
	HTREEITEM rtpItem;
	HTREEITEM patItem;
	HTREEITEM pmvItem;

	void setHasChildren( const HTREEITEM item );

protected:
	COleDataSource source;
	RDODropSource  dropsource;
	void doDragDrop( RDOTracerTreeItem* item, CPoint point );
	void addToNewChart( RDOTracerTreeItem* const item );

	//{{AFX_VIRTUAL(RDOTracerTreeCtrl)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerTreeCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnAddToNewChart();
	afx_msg void OnUpdateAddToNewChart( CCmdUI* pCmdUI );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOTracerTreeCtrl();
	virtual ~RDOTracerTreeCtrl();

	BOOL setModelName( const CString& modelName );
	void addResourceType( RDOTracerResType* resType );
	void addResource( RDOTracerResource* res );
	void updateResource( RDOTracerResource* const res );
	void addPattern( RDOTracerPattern* pat );
	void addOperation( RDOTracerOperation* opr );
	void addIrregularEvent( RDOTracerOperation* opr );
	void addResult( RDOTracerResult* res );
	void clear();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOTRACERTREECTRL_H
