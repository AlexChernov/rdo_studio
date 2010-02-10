#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"

namespace rdoParse 
{

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}

void frmerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDOFUNLogic* logic ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_frame( NULL )
{
	m_frame = new rdoRuntime::RDOFRMFrame( parser()->runtime(), _src_info, logic ? logic->getCalc() : NULL );
	parser()->insertFRMFrame( this );
}

} // namespace rdoParse
