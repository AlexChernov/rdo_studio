// Scintilla source code edit control
/** @file LexDebug.cxx
 ** Lexer for debug output.
 ** Written by Andrey Urusov.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "SString.h"

static void ColouriseBuildDoc( unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler )
{
	WordList& keywords = *keywordlists[ 0 ];
	if ( !keywords ) return;
	const char* findKeyword = keywords[0];
	const int findKeywordLen = strlen( findKeyword );
	bool matchCase = styler.GetPropertyInt( "find_matchcase", 0 ) ? true : false;

	styler.StartAt( startPos );
	styler.StartSegment( startPos );
	int state = initStyle;
	unsigned int lengthDoc = startPos + length;
	for ( unsigned int i = startPos; i < lengthDoc; i++ ) {

		char ch = styler.SafeGetCharAt( i );

		if ( styler.IsLeadByte( ch ) ) {
			i += 1;
			continue;
		}

		if ( state == SCE_FIND_DEFAULT || state == SCE_FIND_IDENTIFIER ) {
			if ( (matchCase && ch == findKeyword[0]) /*|| (!matchCase && RDOMakeLowerCase(ch) == RDOMakeLowerCase(findKeyword[0]))*/ ) {
				bool flag = true;
				for ( int j = 0; j < findKeywordLen; j++ ) {
					char c1 = styler.SafeGetCharAt( i + j );
					char c2 = findKeyword[j];
					if ( (matchCase && c1 != c2) /*|| (!matchCase && RDOMakeLowerCase(c1) != RDOMakeLowerCase(c2))*/ ) {
						flag = false;
						break;
					}
				}
				if ( flag ) {
					int lineCurrent = styler.GetLine( i );
					int posInLine   = i - styler.LineStart( lineCurrent );
					if ( posInLine < 3 ) {
						flag = false;
					}
				}

				styler.ColourTo( i - 1, state );
				if ( flag ) {
					styler.ColourTo( i + findKeywordLen - 1, SCE_FIND_KEYWORD );
					i += findKeywordLen - 1;
					state = SCE_FIND_DEFAULT;
				} else {
					if ( ch != ' ' && ch != '\t' ) {
						state = SCE_FIND_IDENTIFIER;
					} else {
						state = SCE_FIND_DEFAULT;
					}
				}
			} else if ( ch != ' ' && ch != '\t' ) {
				styler.ColourTo( i - 1, state );
				state = SCE_FIND_IDENTIFIER;
			} else {
				styler.ColourTo( i - 1, state );
				state = SCE_FIND_DEFAULT;
			}
		}

	}
	styler.ColourTo( lengthDoc - 1, state );
}

LexerModule lmBuild( SCLEX_BUILD, ColouriseBuildDoc, "build" );
