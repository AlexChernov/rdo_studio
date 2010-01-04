#include "rdo_studio/stdafx.h"
#include "rdo_studio/edit_ctrls/rdofindedit.h"
#include "rdo_studio/edit_ctrls/sci/SciLexer.h"
#include "rdo_studio/edit_ctrls/sci/LexFind.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/resource.h"
#include "rdo_studio/htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;
using namespace rdoStyle;

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ�����������娸����������������������������������������������������";

// ----------------------------------------------------------------------------
// ---------- RDOFindEdit
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI �������

BEGIN_MESSAGE_MAP( RDOFindEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOFindEdit)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR, OnUpdateModifyStatusBar )
END_MESSAGE_MAP()

RDOFindEdit::RDOFindEdit(): RDOLogEdit()
{
}

RDOFindEdit::~RDOFindEdit()
{
}

int RDOFindEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOLogEdit::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETLEXER, SCLEX_FIND );
//	int lexLanguage = sendEditor( SCI_GETLEXER );
	sendEditor( SCI_SETSTYLEBITS, 5 );
	sendEditorString( SCI_SETWORDCHARS, 0, wordCharacters );

	return 0;
}

void RDOFindEdit::setEditorStyle( RDOFindEditStyle* _style )
{
	RDOLogEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Colors
	RDOFindEditTheme* theme = static_cast<RDOFindEditTheme*>(style->theme);
	sendEditor( SCI_STYLESETFORE, SCE_FIND_DEFAULT, theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_FIND_DEFAULT, theme->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_FIND_KEYWORD, theme->keywordColor );
	sendEditor( SCI_STYLESETBACK, SCE_FIND_KEYWORD, theme->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::UNDERLINE );

	// ----------
	// Font Name
	sendEditorString( SCI_STYLESETFONT, SCE_FIND_DEFAULT, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_FIND_KEYWORD, style->font->name.c_str() );

	// ----------
	// Font Size
	sendEditor( SCI_STYLESETSIZE, SCE_FIND_DEFAULT, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_FIND_KEYWORD, style->font->size );

	// ----------
	// Codepage and Characterset
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_FIND_DEFAULT, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_FIND_KEYWORD, style->font->characterSet );
}

void RDOFindEdit::setKeyword( const std::string& keyword, const bool matchCase ) const
{
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("find_matchcase"), matchCase ? "1" : "0" );
	sendEditorString( SCI_SETKEYWORDS, SCI_RDO_ENDOFLINEONLY_KEYWORDSINDEX, keyword.c_str() );
}

void RDOFindEdit::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( "%d: %d", getCurrentColumnNumber() + 1, getCurrentLineNumber() + 1 ).c_str() );
}

void RDOFindEdit::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
}

void RDOFindEdit::OnHelpKeyword()
{
	std::string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_run.htm#output_find";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
