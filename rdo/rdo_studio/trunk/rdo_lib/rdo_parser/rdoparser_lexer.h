/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_lexer.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_LEXER_H_
#define _RDOPARSER_LEXER_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdogramma.h"
#include "thirdparty/bison_flex/FlexLexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
class RDOParser;

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(PTR(RDOParser) parser, PTR(std::istream) yyin, PTR(std::ostream) yyout);

	void loc_init     ();
	void loc_action   ();
	void loc_delta_pos(int value);
	void setvalue     (int value);

	PTR(RDOParser)   m_parser;
	PTR(int)         m_lpval;
	PTR(YYLTYPE)     m_lploc;
	int              m_enum_param_cnt;

protected:
	virtual int  LexerInput (PTR(char)  buf, int max_size);
	virtual void LexerOutput(CPTR(char) buf, int size    );
	virtual void LexerError (CPTR(char) msg);

private:
	PTR(std::istream) m_yyin;
	PTR(std::ostream) m_yyout;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdoparser_lexer.inl"

#endif //! _RDOPARSER_LEXER_H_
