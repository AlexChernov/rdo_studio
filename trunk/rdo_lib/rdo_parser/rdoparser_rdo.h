#ifndef RDOPARSER_RDO_H
#define RDOPARSER_RDO_H

#include "rdoparser_base.h"
#include "rdoparser_lexer.h"
#include "rdoparser_object.h"
#include <rdoruntime_object.h>
#include <iostream.h>

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
class RDOParserRDOItem: public RDOParserItem
{
public:
	RDOParserRDOItem( RDOParser* _parser, rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun );
	virtual ~RDOParserRDOItem();

	virtual void parse( StreamFrom streamFrom = sf_repository );
	virtual void parse( std::istream& in_stream );
	virtual RDOLexer* getLexer( std::istream& in_stream, std::ostream& out_stream );

	virtual int  lexer_loc_line();
	virtual int  lexer_loc_pos();

protected:
	RDOLexer*  m_lexer;
	YYLTYPE    m_loc;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
public:
	RDOParserRSS( RDOParser* _parser );

	virtual void parse( std::istream& in_stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
public:
	RDOParserRSSPost( RDOParser* _parser ): RDOParserItem( _parser, rdoModelObjects::RSS, NULL, NULL, NULL ) {};

	virtual void parse( StreamFrom streamFrom = sf_repository );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
public:
	RDOParserSTDFUN( RDOParser* _parser ): RDOParserItem( _parser, rdoModelObjects::FUN, NULL, NULL, NULL ) {};

	virtual void parse( StreamFrom streamFrom = sf_repository );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
class RDOSMR1OkException: public rdoRuntime::RDOException
{
public:
	virtual std::string getType() const { return ""; }
	RDOSMR1OkException( const char* str ): RDOException( str ) {};
};

class RDOParserSMRFile: public RDOParserRDOItem
{
public:
	RDOParserSMRFile( RDOParser* _parser );

	virtual void parse( std::istream& in_stream );
};

} // namespace rdoParse

#endif // RDOPARSER_RDO_H
