#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdoopr.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdo_logic_opr.h"

namespace rdoParse 
{

int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void oprerror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( parent, _src_info, _pattern_src_info )
{
	parser()->insertOPROperation( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>( _parser, _src_info )
{
	m_rt_logic = F(rdoRuntime::RDOOperations)::create(parser()->runtime());
	m_rt_logic->init(parser()->runtime());
	parser()->insertOperations( this );
}

} // namespace rdoParse 
