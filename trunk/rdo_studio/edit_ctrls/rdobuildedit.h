#ifndef RDOBUILDEDIT_H
#define RDOBUILDEDIT_H
#pragma once

#include "rdologedit.h"

namespace rdoEditCtrl {

// ----------------------------------------------------------------------------
// ---------- RDOBuildEditLineInfo
// ----------------------------------------------------------------------------
class RDOBuildEditLineInfo: public RDOLogEditLineInfo
{
protected:
	bool error;

public:
	RDOBuildEditLineInfo( const std::string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1, bool _error = true );
	~RDOBuildEditLineInfo();

	virtual std::string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOBuildEdit
// ----------------------------------------------------------------------------
class RDOBuildEdit: public RDOLogEdit
{
protected:
	virtual void updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo );

protected:
	//{{AFX_MSG(RDOBuildEdit)
	afx_msg void OnHelpKeyword();
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOBuildEdit)
	//}}AFX_VIRTUAL

public:
	RDOBuildEdit();
	virtual ~RDOBuildEdit();
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDOBUILDEDIT_H
