#include "stdafx.h"
#include "rdobuildedit.h"
#include "../rdo_edit/rdoeditoredit.h"
#include "../rdostudioapp.h"
#include "../resource.h"
#include "../htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOBuildEditLineInfo
// ----------------------------------------------------------------------------
RDOBuildEditLineInfo::RDOBuildEditLineInfo( const std::string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, bool _error ):
	RDOLogEditLineInfo( _message, _fileType, _lineNumber ),
	error( _error )
{
}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{
}

std::string RDOBuildEditLineInfo::getMessage() const
{
	std::string file;
	switch ( fileType ) {
		case rdoModelObjects::PAT: file = "PAT"; break;
		case rdoModelObjects::RTP: file = "RTP"; break;
		case rdoModelObjects::RSS: file = "RSS"; break;
		case rdoModelObjects::OPR: file = "OPR"; break;
		case rdoModelObjects::FRM: file = "FRM"; break;
		case rdoModelObjects::FUN: file = "FUN"; break;
		case rdoModelObjects::DPT: file = "DPT"; break;
		case rdoModelObjects::SMR: file = "SMR"; break;
		case rdoModelObjects::PMD: file = "PMD"; break;
		default: file = "";
	}
	if ( lineNumber < 0 || file.empty() ) {
		return message;
	} else {
		std::string s = error ? "error" : "warning";
		return rdo::format( "%s (%d): %s: %s", file.c_str(), lineNumber + 1, s.c_str(), message.c_str() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOBuildEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOBuildEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOBuildEdit)
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOBuildEdit::RDOBuildEdit(): RDOLogEdit()
{
}

RDOBuildEdit::~RDOBuildEdit()
{
}

void RDOBuildEdit::updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo )
{
	RDOLogEdit::updateEdit( edit, lineInfo );
	edit->setErrorLine( lineInfo->lineNumber );
}

void RDOBuildEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOBuildEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
}

void RDOBuildEdit::OnHelpKeyword()
{
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_windows_output.htm";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
