#include "stdafx.h"
#include "rdofindedit.h"
#include "sci/SciLexer.h"
#include "sci/LexFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditCtrl;

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ�����������娸����������������������������������������������������";

// ----------------------------------------------------------------------------
// ---------- RDOFindEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOFindEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOFindEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
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
	int lexLanguage = sendEditor( SCI_GETLEXER );
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
	sendEditor( SCI_STYLESETBOLD     , SCE_FIND_DEFAULT, theme->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_FIND_DEFAULT, theme->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_FIND_DEFAULT, theme->defaultStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_FIND_KEYWORD, theme->keywordStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_FIND_KEYWORD, theme->keywordStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_FIND_KEYWORD, theme->keywordStyle & RDOFS_UNDERLINE );

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

void RDOFindEdit::setKeyword( const string& keyword, const bool matchCase ) const
{
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("find_matchcase"), matchCase ? "1" : "0" );
	sendEditorString( SCI_SETKEYWORDS, SCI_RDO_ENDOFLINEONLY_KEYWORDSINDEX, keyword.c_str() );
}
