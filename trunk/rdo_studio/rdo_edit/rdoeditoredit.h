#ifndef RDOEDITOREDIT_H
#define RDOEDITOREDIT_H
#pragma once

#include "rdoeditorbaseedit.h"
#include "rdoeditoreditstyle.h"

class RDOStudioEditBaseView;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorEdit
// ----------------------------------------------------------------------------
class RDOEditorEdit: public RDOEditorBaseEdit
{
private:
	int sci_MARKER_ERROR;

protected:
	int bufSelStart;
	RDOStudioEditBaseView* view;

	void commentSelection() const;
	void completeWord();

	void onBufferAppend( const int bufIndex );
	void onBufferEdit( const int bufIndex );

	void setErrorLine( int line = -1 );
	void clearErrorLine();
	bool hasErrorLine() const;

	//{{AFX_MSG(RDOEditorEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnEditCommentSelection();
	afx_msg void OnEditCompleteWord();
	afx_msg void OnInsertBuffer1Paste();
	afx_msg void OnInsertBuffer2Paste();
	afx_msg void OnInsertBuffer3Paste();
	afx_msg void OnInsertBuffer4Paste();
	afx_msg void OnInsertBuffer1Append();
	afx_msg void OnInsertBuffer2Append();
	afx_msg void OnInsertBuffer3Append();
	afx_msg void OnInsertBuffer4Append();
	afx_msg void OnInsertBuffer1Edit();
	afx_msg void OnInsertBuffer2Edit();
	afx_msg void OnInsertBuffer3Edit();
	afx_msg void OnInsertBuffer4Edit();
	afx_msg void OnInsertBuffer1Clear();
	afx_msg void OnInsertBuffer2Clear();
	afx_msg void OnInsertBuffer3Clear();
	afx_msg void OnInsertBuffer4Clear();
	afx_msg void OnUndateBuffer1Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer2Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer3Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer4Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBufferAppend( CCmdUI* pCmdUI );
	afx_msg void OnUpdateInsertBufferEdit(CCmdUI* pCmdUI);
	afx_msg void OnUndateBuffer1Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer2Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer3Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer4Clear( CCmdUI* pCmdUI );
	//}}AFX_MSG
	afx_msg void OnInsertCommand( UINT nID );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorEdit)
	protected:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	//}}AFX_VIRTUAL

public:
	RDOEditorEdit( RDOStudioEditBaseView* _view = NULL );
	virtual ~RDOEditorEdit();

	void setEditorStyle( RDOEditorEditStyle* _style );

	void replaceCurrent( const std::string str, const int changePosValue = -1 ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITOREDIT_H
