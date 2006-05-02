#ifndef RDOPARSER_BASE_H
#define RDOPARSER_BASE_H

#include <rdocommon.h>

namespace rdoParse
{

typedef int  (*t_bison_parse_fun)( void* lexer );
typedef void (*t_bison_error_fun)( char* mes );
typedef int  (*t_flex_lexer_fun)( int* lpval, void* lexer );

// ----------------------------------------------------------------------------
// ---------- RDOParserBase
// ----------------------------------------------------------------------------
class RDOParserBase
{
public:
	rdoModelObjects::RDOFileType type;

	t_bison_parse_fun parser_fun;
	t_bison_error_fun error_fun;
	t_flex_lexer_fun  lexer_fun;

	RDOParserBase(): type(rdoModelObjects::PAT), parser_fun(NULL), error_fun(NULL), lexer_fun(NULL) {};
	RDOParserBase( rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun ): type( _type ), parser_fun( _parser_fun ), error_fun( _error_fun ), lexer_fun( _lexer_fun ) {};
	virtual ~RDOParserBase() {};

	virtual void parse()                          {            }
	virtual void parse( std::istream& in_stream ) {            }
	virtual void set_lexer_value( int value )     {            }
	virtual int lineno()                          { return -1; }
};

// ----------------------------------------------------------------------------
// ---------- RDOParserList
// ----------------------------------------------------------------------------
class RDOParserList
{
private:
	std::map< int, RDOParserBase* > list;

public:
	RDOParserList();
	~RDOParserList();

	void clear();
	void reset();

	int insertParser( rdoModelObjects::RDOParseType type, RDOParserBase* parser );
	std::map< int, RDOParserBase* >::const_iterator begin() const { return list.begin(); }
	std::map< int, RDOParserBase* >::const_iterator end()   const { return list.end();   }
	std::map< int, RDOParserBase* >::const_iterator find( int index ) const { return list.find( index ); }

	static void getParserMinMax( rdoModelObjects::RDOParseType type, int& min, int& max );
	static std::list< rdoModelObjects::RDOFileType > getParserFiles( int files );
};


} // namespace rdoParse

#endif // RDOPARSER_BASE_H
