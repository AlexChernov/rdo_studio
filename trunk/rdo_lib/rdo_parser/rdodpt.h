#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdoparser_object.h"
#include "rdofun.h"
#include "rdopat.h"
#include <rdodptrtime.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOPatternRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
}

namespace rdoParse 
{

int dptparse( void* lexer );
int dptlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpterror( char* mes );

int dpt_rss_parse( void* lexer );
int dpt_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpt_rss_error( char* mes );

int dpt_opr_parse( void* lexer );
int dpt_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpt_opr_error( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
class RDODPTActivity: public RDOParserObject, public RDOParserSrcInfo
{
private:
	void init( const RDOParserSrcInfo& _pattern_src_info );

protected:
	const RDOPATPattern*            pattern;
	int                             currParam;
	rdoRuntime::RDOActivityRuntime* activity;

public:
	RDODPTActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
	RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	const std::string&              getName() const     { return src_info().src_text(); }
	const RDOPATPattern*            getType() const     { return pattern;               }
	rdoRuntime::RDOActivityRuntime* getActivity() const { return activity;              }

	void addParam( const std::string& _param, const YYLTYPE& _param_pos );
	void addParam( int _param, const YYLTYPE& _param_pos );
	void addParam( double _param, const YYLTYPE& _param_pos );
	void addParam( const YYLTYPE& _param_pos );
	void endParam( const YYLTYPE& _param_pos );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOParserObject, public RDOParserSrcInfo
{
private:
	rdoRuntime::RDODecisionPointTrace::DPT_TraceFlag trace;
	RDOFUNLogic*   conditon;
	RDOFUNLogic*   termConditon;
	RDOFUNArithm*  evalBy;
	bool compTops;
	std::vector< RDODPTSearchActivity* > activities;

public:
	RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODecisionPointTrace::DPT_TraceFlag _trace = rdoRuntime::RDODecisionPointTrace::DPT_no_trace );
	const std::string& getName() const                         { return src_info().src_text(); }
	void setCondition( RDOFUNLogic* _conditon = NULL )         { conditon     = _conditon;     }
	void setTermCondition( RDOFUNLogic* _termConditon = NULL ) { termConditon = _termConditon; }
	void setEvaluateBy( RDOFUNArithm* _evalBy )                { evalBy       = _evalBy;       }
	void setCompareTops( bool _compTops )                      { compTops     = _compTops;     }

	RDODPTSearchActivity* addNewActivity( const RDOParserSrcInfo& _src_info, const std::string& _pattern );
	RDODPTSearchActivity* getLastActivity() const {
		return !activities.empty() ? activities.back() : NULL;
	}
	const std::vector< RDODPTSearchActivity* >& getActivities() const { return activities; }

	void end();
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
public:
	enum DPTSearchValue {
		DPT_value_before,
		DPT_value_after
	};

private:
	DPTSearchValue value;
	RDOFUNArithm* ruleCost;

public:
	RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	DPTSearchValue getValue() const { return value; }
	void setValue( DPTSearchValue _value, RDOFUNArithm* _ruleCost, const YYLTYPE& _param_pos );

	RDOFUNArithm* getRuleCost() const { return ruleCost; }
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOParserObject, public RDOParserSrcInfo
{
private:
	RDOFUNLogic* conditon;
	std::vector< RDODPTSomeActivity* > activities;

public:
	RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info );

	const std::string& getName() const                 { return src_info().src_text(); }

	RDOFUNLogic* getConditon() const                   { return conditon;              }
	void setCondition( RDOFUNLogic* _conditon = NULL ) { conditon = _conditon;         }

	RDODPTSomeActivity* addNewActivity( const RDOParserSrcInfo& _src_info, const std::string& _pattern );
	RDODPTSomeActivity* getLastActivity() const {
		return !activities.empty() ? activities.back() : NULL;
	}
	const std::vector< RDODPTSomeActivity* >& getActivities() const { return activities; }
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivity
{
public:
	RDODPTSomeActivity( const RDODPTSome* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	RDODPTActivityHotKey( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
	void addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
class RDODPTFreeActivity: public RDODPTActivityHotKey
{
public:
	RDODPTFreeActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCProcess: public RDOParserObject
{
friend class RDOPROCOperator;

protected:
	std::string                   name;
	bool                          m_end;
	RDOPROCProcess*               parent;
	std::list< RDOPROCProcess* >  child;
	std::list< RDOPROCOperator* > operations;
	rdoRuntime::RDOPROCProcess*   runtime;

public:
	static std::string name_prefix;
	static std::string name_sufix;

	RDOPROCProcess( RDOParser* _parser, const std::string& _name );

	void end();
	bool isend() const { return m_end; }

	void insertChild( RDOPROCProcess* value );

	rdoRuntime::RDOPROCProcess* getRunTime() const { return runtime; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCTransact: public RDORTPResType
{
protected:
	static bool created;
	RDOPROCTransact( RDOParser* _parser );

public:
	static RDOPROCTransact* makeRTP( RDOParser* _parser );
	virtual ~RDOPROCTransact();
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
class RDOPROCOperator: public RDOParserObject
{
protected:
	std::string     name;
	RDOPROCProcess* process;

	RDOPROCOperator( RDOPROCProcess* _process, const std::string& _name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCGenerate* runtime;

public:
	RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCSeize* runtime;

public:
	RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCRelease* runtime;

public:
	RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name );
};


// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCAdvance* runtime;

public:
	RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCTerminate* runtime;

public:
	RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name );
};

} // namespace rdoParse

#endif // RDODPT_DPT
