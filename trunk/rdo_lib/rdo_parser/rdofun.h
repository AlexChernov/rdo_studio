#ifndef RDOFUN_H
#define RDOFUN_H

#include "rdoparser_object.h"
#include "rdortp.h"
#include <rdoruntime_object.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOCalcConst;
class RDOCalcIsEqual;
class RDOCalcFuncParam;
class RDOCalcDoubleToIntByResult;
class RDOFunCalc;
class RDOCalcSeqInit;
class RDOCalcSeqNext;
class RDOFunCalcSelect;
}

namespace rdoParse 
{

int funparse( void* lexer );
int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void funerror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
private:
	std::vector< rdoRuntime::RDOCalcDoubleToIntByResult* > int_or_double;

public:
	void initCalc( bool round );
	void push_back( rdoRuntime::RDOCalcDoubleToIntByResult* calc ) {
		int_or_double.push_back( calc );
	}
	void insert( const RDOFUNDoubleToIntByResult& first ) {
		int_or_double.insert( int_or_double.end(), first.int_or_double.begin(), first.int_or_double.end() );
	}
	void insert( const RDOFUNDoubleToIntByResult& first, const RDOFUNDoubleToIntByResult& second ) {
		int_or_double.insert( int_or_double.end(), first.int_or_double.begin(), first.int_or_double.end() );
		int_or_double.insert( int_or_double.end(), second.int_or_double.begin(), second.int_or_double.end() );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
class RDOFUNLogic: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOFUNArithm;

private:
	rdoRuntime::RDOCalc* calc;
	RDOFUNDoubleToIntByResult int_or_double;

public:
	rdoRuntime::RDOCalc* createCalc( rdoRuntime::RDOValue::ParamType _type = rdoRuntime::RDOValue::pt_real );

	RDOFUNLogic( const RDOParserObject* _parent, rdoRuntime::RDOCalc* _calc, bool hide_warning = false );

	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcPos( const YYLTYPE& _error_pos ) {
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end ) {
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
class RDOFUNArithm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPEnum* enu; // for type == enum
	std::string str; // for type == unknow

private:
	rdoRuntime::RDOValue::ParamType type;
	rdoRuntime::RDOCalc* calc;
	RDOFUNDoubleToIntByResult int_or_double;

	void init( const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info );
	void init( const std::string& value, const YYLTYPE& _pos );

public:
	RDOFUNArithm( RDOParser* _parser, rdoRuntime::RDOValue::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info );
	RDOFUNArithm( RDOParser* _parser, int value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, double* value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, const std::string& value, const YYLTYPE& _pos );

	RDOFUNArithm( const RDOParserObject* _parent, rdoRuntime::RDOValue::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOFUNArithm* _parent, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info );
	RDOFUNArithm( const RDOFUNArithm* _parent, int value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOFUNArithm* _parent, double* value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOFUNArithm* _parent, const std::string& value, const YYLTYPE& _pos );

	RDOFUNArithm* operator +( RDOFUNArithm& second );
	RDOFUNArithm* operator -( RDOFUNArithm& second );
	RDOFUNArithm* operator *( RDOFUNArithm& second );
	RDOFUNArithm* operator /( RDOFUNArithm& second );

	RDOFUNLogic* operator ==( RDOFUNArithm& second );
	RDOFUNLogic* operator !=( RDOFUNArithm& second );
	RDOFUNLogic* operator < ( RDOFUNArithm& second );
	RDOFUNLogic* operator > ( RDOFUNArithm& second );
	RDOFUNLogic* operator <=( RDOFUNArithm& second );
	RDOFUNLogic* operator >=( RDOFUNArithm& second );

	rdoRuntime::RDOCalc* createCalc( const RDORTPParamType* const forType = NULL );
	rdoRuntime::RDOCalc* getCalc() const            { return calc; }
	rdoRuntime::RDOValue::ParamType getType() const { return type; }

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end ) {
		RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	}
	void setSrcPos( const YYLTYPE& _error_pos ) {
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end ) {
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
class RDOFUNConstant: public RDOParserObject, public RDOParserSrcInfo
{
private:
	RDORTPParam* param;

public:
	int number;
	RDOFUNConstant( RDOParser* _parser, RDORTPParam* _param );
	const std::string&           getName() const  { return param->getName(); }
	const RDORTPParamType* const getType() const  { return param->getType(); }
	const RDORTPParam* const     getDescr() const { return param;            }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// ���������, � �������� ������� ����������
// ----------------------------------------------------------------------------
class RDOFUNParams: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOParserSrcInfo funseq_name;
	std::vector< RDOFUNArithm* > params;

	RDOFUNParams( RDOParser* _parser ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo()
	{
	}
	RDOFUNParams( const RDOParserObject* _parent ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo()
	{
	}
	void addParameter( RDOFUNArithm* param ) {
		params.push_back( param );
	}
	RDOFUNArithm* createCall( const std::string& funName ) const;
	RDOFUNArithm* createSeqCall( const std::string& seqName ) const;
};

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
class RDOFUNSequence: public RDOParserObject, public RDOParserSrcInfo
{
public:
	class RDOFUNSequenceHeader: public RDOParserObject, public RDOParserSrcInfo
	{
	private:
		const RDORTPParamType* type;

	public:
		RDOFUNSequenceHeader( RDOParser* _parser, const RDORTPParamType* _type, const RDOParserSrcInfo& _src_info ):
			RDOParserObject( _parser ),
			RDOParserSrcInfo( _src_info ),
			type( _type )
		{
		}
		const RDORTPParamType* getType() const { return type; }
	};

	RDOFUNSequenceHeader* header;
	rdoRuntime::RDOCalcSeqInit* init_calc;
	rdoRuntime::RDOCalcSeqNext* next_calc;

protected:
	int base;

	RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base );
	void initResult();
	void initCalcSrcInfo();

public:
	const std::string& getName() const  { return header->src_text(); }
	virtual void createCalcs() = 0;
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- ������� ��������� �����
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
class RDOFUNSequenceUniform: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
class RDOFUNSequenceExponential: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base =123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
class RDOFUNSequenceNormal: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- �����������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	class RDOFUNSequenceByHistHeader: public RDOParserObject, public RDOParserSrcInfo
	{
	public:
		RDOFUNSequence::RDOFUNSequenceHeader* header;
		int                                   base;

		RDOFUNSequenceByHistHeader( RDOParser* _parser, RDOFUNSequence::RDOFUNSequenceHeader* _header, int _base = 123456789 ):
			RDOParserObject( _parser ),
			RDOParserSrcInfo( _header->src_info() ),
			header( _header ),
			base( _base )
		{
		}
	};

	RDOFUNSequenceByHist( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
		_header->reparent( this );
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
private:
	virtual void createCalcs();

public:
	std::vector< rdoRuntime::RDOValue > from;
	std::vector< rdoRuntime::RDOValue > to;
	std::vector< rdoRuntime::RDOValue > freq;

	RDOFUNSequenceByHistReal( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, rdoRuntime::RDOValue _from, rdoRuntime::RDOValue _to, rdoRuntime::RDOValue _freq, const YYLTYPE& _from_pos, const YYLTYPE& _to_pos, const YYLTYPE& _freq_pos ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addReal( _from, _to, _freq, _from_pos, _to_pos, _freq_pos );
	}
	void addReal( rdoRuntime::RDOValue _from, rdoRuntime::RDOValue _to, rdoRuntime::RDOValue _freq, const YYLTYPE& _from_pos, const YYLTYPE& _to_pos, const YYLTYPE& _freq_pos );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
private:
	virtual void createCalcs();

public:
	std::vector< rdoRuntime::RDOValue > val;
	std::vector< rdoRuntime::RDOValue > freq;

	RDOFUNSequenceByHistEnum( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const RDOParserSrcInfo& _value_info, rdoRuntime::RDOValue _freq, const YYLTYPE& _freq_pos ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addEnum( _value_info, _freq, _freq_pos );
	}
	void addEnum( const RDOParserSrcInfo& _value_info, rdoRuntime::RDOValue _freq, const YYLTYPE& _freq_pos );
};

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	RDOFUNSequenceEnumerative( RDOParser* _parser, RDOFUNSequenceHeader* _header ):
		RDOFUNSequence( _parser, _header, 0 )
	{
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeInt
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeInt: public RDOFUNSequenceEnumerative
{
private:
	virtual void createCalcs();

public:
	std::vector< int > val;

	RDOFUNSequenceEnumerativeInt( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addInt( _val );
	}
	void addInt( int val );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeReal
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeReal: public RDOFUNSequenceEnumerative
{
private:
	virtual void createCalcs();

public:
	std::vector< double > val;

	RDOFUNSequenceEnumerativeReal( RDOParser* _parser, RDOFUNSequenceHeader* _header, double _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addReal( _val );
	}
	void addReal( double _val );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeEnum: public RDOFUNSequenceEnumerative
{
private:
	virtual void createCalcs();

public:
	std::vector< rdoRuntime::RDOValue > val;

	RDOFUNSequenceEnumerativeEnum( RDOParser* _parser, RDOFUNSequenceHeader* _header, const RDOParserSrcInfo& _value_info ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addEnum( _value_info );
	}
	void addEnum( const RDOParserSrcInfo& _value_info );
};

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionParam
// ----------------------------------------------------------------------------
// ���������, ��������� ����� ����� ������� ����� ��������� ����� $Parameters
// ----------------------------------------------------------------------------
class RDOFUNFunctionParam: public RDOParserObject, public RDOParserSrcInfo
{
private:
	RDORTPParamType* type;

public:
	RDOFUNFunctionParam( const RDOParserObject* _parent, const std::string& _name, RDORTPParamType* _type ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _name ),
		type( _type )
	{
	}
	RDOFUNFunctionParam( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, RDORTPParamType* _type ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		type( _type )
	{
	}
	const std::string& getName() const           { return src_info().src_text(); }
	const RDORTPParamType* const getType() const { return type;                  }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElement: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNFunctionListElement( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info )
	{
	}
	virtual ~RDOFUNFunctionListElement() {}
	virtual rdoRuntime::RDOCalcIsEqual* createIsEqualCalc( const RDORTPParamType* const retType, rdoRuntime::RDOCalcFuncParam* const funcParam, const RDOParserSrcInfo& _src_pos ) const;
	virtual rdoRuntime::RDOCalcConst*   createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const = 0;
	virtual bool isEquivalence() const { return false; }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementIdentif( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info ):
		RDOFUNFunctionListElement( _parent, _src_info )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
public:
	double value;

	RDOFUNFunctionListElementReal( const RDOParserObject* _parent, const YYLTYPE& _pos, double _value ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, rdo::format("%f", _value) ) ),
		value( _value )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
public:
	int value;

	RDOFUNFunctionListElementInt( const RDOParserObject* _parent, const YYLTYPE& _pos, int _value ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, rdo::format("%d", _value) ) ),
		value( _value )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementEq( const RDOParserObject* _parent, const YYLTYPE& _pos ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, "=" ) )
	{
	}
	virtual bool isEquivalence() const { return true; }
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
class RDOFUNCalculateIf: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNLogic*  condition;
	RDOFUNArithm* action;

	RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, RDOFUNArithm* _action );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
class RDOFUNFunction: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOParser;
private:
	const RDORTPParamType* const retType;
	std::vector< const RDOFUNFunctionParam* >       params;
	std::vector< const RDOFUNFunctionListElement* > elements;    // for list and table
	std::vector< const RDOFUNCalculateIf* >         calculateIf; // for algorithmic
	rdoRuntime::RDOFunCalc* functionCalc;

public:
	RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _retType );
	RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType );
	void add( const RDOFUNFunctionParam* const _param );
	void add( const RDOFUNFunctionListElement* const _listElement );
	void add( const RDOFUNCalculateIf* const _calculateIf );
	const RDOFUNFunctionParam* const findFUNFunctionParam( const std::string& paramName ) const;
	int findFUNFunctionParamNum( const std::string& paramName ) const;
	void createListCalc();
	void createTableCalc( const YYLTYPE& _elements_pos );
	void createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info );
	const std::string& getName() const                                { return src_info().src_text(); }
	const std::vector< const RDOFUNFunctionParam* > getParams() const { return params;                }
	      rdoRuntime::RDOFunCalc* getFunctionCalc() const             { return functionCalc;          }
	const RDORTPParamType* const getType() const                      { return retType;               }
};

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
class RDOFUNGroup: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	void init( const RDOParserSrcInfo& _res_info );

public:
	const RDORTPResType* resType;

	RDOFUNGroup( RDOParser* _parser, const RDOParserSrcInfo& _res_info );
	RDOFUNGroup( const RDOParserObject* _parent, const RDOParserSrcInfo& _res_info );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
class RDOFUNGroupLogic: public RDOFUNGroup
{
public:
	enum FunGroupType {
		fgt_unknow    = 0,
		fgt_exist     = 1,
		fgt_notexist  = 2,
		fgt_forall    = 3,
		fgt_notforall = 4
	};

private:
	const int funType;

public:
	RDOFUNGroupLogic( RDOParser* _parser, FunGroupType _funType, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parser, _res_info ),
		funType( _funType )
	{
	}
	RDOFUNGroupLogic( const RDOParserObject* _parent, FunGroupType _funType, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parent, _res_info ),
		funType( _funType )
	{
	}
	RDOFUNLogic* createFunLogic( RDOFUNLogic* cond );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
class RDOFUNSelect: public RDOFUNGroup
{
private:
	rdoRuntime::RDOFunCalcSelect* select;

public:
	RDOFUNSelect( RDOParser* _parser, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parser, _res_info.src_text() ),
		select( NULL )
	{
	}
	void initSelect( const RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty( const RDOParserSrcInfo& _empty_info );
	RDOFUNArithm* createFunSelectSize( const RDOParserSrcInfo& _size_info );
};

} // namespace rdoParse 

#endif // RDOFUN_H
