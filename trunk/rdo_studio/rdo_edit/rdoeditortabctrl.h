#ifndef RDOEDITORTABCTRL_H
#define RDOEDITORTABCTRL_H
#pragma once

#include <rdotabctrl.h>

#include "rdoeditoredit.h"

class RDOStudioEditBaseView;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
class RDOEditorTabCtrl: public RDOTabCtrl
{
private:
	RDOStudioEditBaseView*       view;
	rdoEditCtrl::RDOBaseEditList group;

protected:
	//{{AFX_MSG(RDOEditorTabCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorTabCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	RDOEditorTabCtrl( RDOStudioEditBaseView* _view );
	virtual ~RDOEditorTabCtrl();

	rdoModelObjects::RDOFileType indexToType( const int index ) const;
	int typeToIndex( const rdoModelObjects::RDOFileType type ) const;
	bool typeSupported( const rdoModelObjects::RDOFileType type ) const { return typeToIndex( type ) != -1;        }

	rdoModelObjects::RDOFileType getCurrentRDOItem() const              { return indexToType( getCurrentIndex() ); }
	void setCurrentRDOItem( const rdoModelObjects::RDOFileType type );

	RDOEditorEdit* getCurrentEdit() const               { return static_cast<RDOEditorEdit*>(getItemCurrent()); }
	RDOEditorEdit* getItemEdit( const int index ) const { return static_cast<RDOEditorEdit*>(getItem( index )); }
	RDOEditorEdit* getItemEdit( const rdoModelObjects::RDOFileType type ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORTABCTRL_H
