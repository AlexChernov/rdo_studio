#ifndef RDOEDITORBASEEDIT_H
#define RDOEDITORBASEEDIT_H
#pragma once

#include "../edit_ctrls/rdobaseedit.h"
#include "rdoeditoreditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseEdit
// ----------------------------------------------------------------------------
class RDOEditorBaseEdit: public rdoEditCtrl::RDOBaseEdit
{
protected:
	static char* kw0;
	static char* kw1;
	static char* kw2;

	//{{AFX_MSG(RDOEditorBaseEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorBaseEdit)
	//}}AFX_VIRTUAL

public:
	RDOEditorBaseEdit();
	virtual ~RDOEditorBaseEdit();

	void setEditorStyle( RDOEditorBaseEditStyle* _style );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORBASEEDIT_H
