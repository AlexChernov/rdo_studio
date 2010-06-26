#ifndef RDOFUN_H
#define RDOFUN_H

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoRuntime
{
class RDOCalcConst;
class RDOCalcIsEqual;
class RDOCalcFuncParam;
class RDOCalcDoubleToIntByResult;
class RDOCalcFunctionCall;
class RDOFunCalc;
class RDOCalcSeqInit;
class RDOCalcSeqNext;
class RDOFunCalcSelect;
}

namespace rdoParse 
{

int  funparse(PTR(void) lexer);
int  funlex   (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void funerror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
public:
	void roundCalc();
	void push_back(CREF(rdoRuntime::LPRDOCalcDoubleToIntByResult) pCalc)
	{
		m_int_or_double.push_back(pCalc);
	}
	void insert( const RDOFUNDoubleToIntByResult& first )
	{
		m_int_or_double.insert( m_int_or_double.end(), first.m_int_or_double.begin(), first.m_int_or_double.end() );
	}
	void insert( const RDOFUNDoubleToIntByResult& first, const RDOFUNDoubleToIntByResult& second )
	{
		m_int_or_double.insert( m_int_or_double.end(), first.m_int_or_double.begin(), first.m_int_or_double.end() );
		m_int_or_double.insert( m_int_or_double.end(), second.m_int_or_double.begin(), second.m_int_or_double.end() );
	}

private:
	typedef std::vector<rdoRuntime::LPRDOCalcDoubleToIntByResult> CalcList;
	CalcList m_int_or_double;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
class RDOFUNLogic: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOFUNArithm;

public:
	rdoRuntime::LPRDOCalc getCalc( rdoRuntime::RDOType::TypeID _id = rdoRuntime::RDOType::t_real );

	RDOFUNLogic( const RDOFUNArithm& arithm );
	RDOFUNLogic( const RDOParserObject* _parent, CREF(rdoRuntime::LPRDOCalc) pCalc, bool hide_warning = false );
	virtual ~RDOFUNLogic() {}

	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcPos( const YYLTYPE& _error_pos )
	{
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
	{
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}

private:
	rdoRuntime::LPRDOCalc     m_pCalc;
	RDOFUNDoubleToIntByResult m_int_or_double;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
class RDOFUNArithm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNArithm( RDOParser* parser, const RDOValue& value );
	RDOFUNArithm( RDOParser* parser, const RDOValue& value, CREF(rdoRuntime::LPRDOCalc) pCalc );
	RDOFUNArithm( RDOParser* parser, const RDOValue& res_name, const RDOValue& par_name );

	RDOFUNArithm( const RDOFUNArithm*    parent, const RDOValue& value );
	RDOFUNArithm( const RDOParserObject* parent, const RDOValue& value, CREF(rdoRuntime::LPRDOCalc) pCalc );
	RDOFUNArithm( const RDOFUNArithm*    parent, const RDOValue& res_name, const RDOValue& par_name );

	virtual ~RDOFUNArithm() {}

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

	rdoRuntime::LPRDOCalc          createCalc       (CREF(LPRDOTypeParam) forType = NULL);
	rdoRuntime::LPRDOCalc          calc() const     { return m_pCalc;                    }
	const RDOValue&                value() const    { return m_value;                    }
	LPRDOType                      type() const     { return m_value.type();             }
	LPRDOEnumType                  enumType() const { return type().object_static_cast<RDOEnumType>(); }
	rdoRuntime::RDOType::TypeID    typeID() const   { return type()->type().typeID();    }

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
	{
		RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	}
	void setSrcPos( const YYLTYPE& _error_pos )
	{
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
	{
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}

	void checkParamType(CREF(LPRDOTypeParam) pType);

private:
	RDOValue                   m_value;
	rdoRuntime::LPRDOCalc      m_pCalc;
	RDOFUNDoubleToIntByResult  m_int_or_double;

	void init( const RDOValue& value );
	void init( const RDOValue& res_name, const RDOValue& par_name );

	enum CastResult
	{
		CR_DONE,
		CR_CONTINUE
	};
	CastResult beforeCastValue(REF(RDOFUNArithm)  second);
	LPRDOType  getPreType     (CREF(RDOFUNArithm) second);
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
class RDOFUNConstant: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNConstant(PTR(RDOParser) parser, CREF(tstring) name, CREF(LPRDOTypeParam) type);
	virtual ~RDOFUNConstant()
	{}

	CREF(tstring)  name() const      { return m_name;   }
	LPRDOTypeParam getType() const   { return m_type;   }
	int            getNumber() const { return m_number; }

private:
	tstring         m_name;
	LPRDOTypeParam  m_type;
	int             m_number;
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
	virtual ~RDOFUNParams() {}

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
	public:
		RDOFUNSequenceHeader(PTR(RDOParser) parser, CREF(LPRDOTypeParam) type, CREF(RDOParserSrcInfo) src_info)
			: RDOParserObject (parser  )
			, RDOParserSrcInfo(src_info)
			, m_type          (type    )
		{}
		CREF(LPRDOTypeParam) getType() const
		{
			return m_type;
		}

	private:
		LPRDOTypeParam m_type;
	};

	RDOFUNSequenceHeader* header;
	rdoRuntime::LPRDOCalcSeqInit init_calc;
	rdoRuntime::LPRDOCalcSeqNext next_calc;

protected:
	int base;

	RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base );
	virtual ~RDOFUNSequence() {}

	void initResult();
	void initCalcSrcInfo();

public:
	const std::string& name() const  { return header->src_text(); }
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
public:
	std::vector< rdoRuntime::RDOValue > m_from;
	std::vector< rdoRuntime::RDOValue > m_to;
	std::vector< rdoRuntime::RDOValue > m_freq;

	RDOFUNSequenceByHistReal( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const RDOValue& from, const RDOValue& to, const RDOValue& freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addReal( from, to, freq );
	}
	void addReal( const RDOValue& from, const RDOValue& to, const RDOValue& freq );

private:
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
public:
	std::vector< rdoRuntime::RDOValue > m_values;
	std::vector< rdoRuntime::RDOValue > m_freq;

	RDOFUNSequenceByHistEnum( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const RDOValue& value, const RDOValue& freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addEnum( value, freq );
	}
	void addEnum( const RDOValue& value, const RDOValue& freq );

private:
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	RDOFUNSequenceEnumerative( RDOParser* _parser, RDOFUNSequenceHeader* _header, const RDOValue& value ):
		RDOFUNSequence( _parser, _header, 0 )
	{
		addValue( value );
	}
	virtual ~RDOFUNSequenceEnumerative()
	{
	}
	void addValue( const RDOValue& value )
	{
		m_values.push_back( header->getType()->value_cast(value) );
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;

private:
	std::vector< RDOValue > m_values;
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionParam
// ----------------------------------------------------------------------------
// ���������, ��������� ����� ����� ������� ����� ��������� ����� $Parameters
// ----------------------------------------------------------------------------
OBJECT(RDOFUNFunctionParam) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFUNFunctionParam)
public:
	CREF(tstring)  name   () const { return src_info().src_text(); }
	LPRDOTypeParam getType() const { return m_type;                }

private:
	RDOFUNFunctionParam(CREF(tstring) name, CREF(LPRDOTypeParam) type)
		: RDOParserSrcInfo(name  )
		, m_type          (type  )
	{}
	RDOFUNFunctionParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) type)
		: RDOParserSrcInfo(src_info)
		, m_type          (type    )
	{}
	virtual ~RDOFUNFunctionParam()
	{}

	LPRDOTypeParam m_type;
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
	virtual rdoRuntime::LPRDOCalcIsEqual createIsEqualCalc(CREF(LPRDOTypeParam) retType, CREF(rdoRuntime::LPRDOCalcFuncParam) funcParam, const RDOParserSrcInfo& _src_pos ) const;
	virtual rdoRuntime::LPRDOCalcConst   createResultCalc (CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const = 0;
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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const;
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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const;
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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const;
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
	virtual rdoRuntime::LPRDOCalcConst createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const;
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
	virtual ~RDOFUNCalculateIf() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
class RDOFUNFunction: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOParser;
public:
	typedef std::vector<LPRDOFUNFunctionParam> ParamList;

	RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, CREF(LPRDOTypeParam) _retType );
	RDOFUNFunction( RDOParser* _parser, const std::string& _name, CREF(LPRDOTypeParam) _retType );
	virtual ~RDOFUNFunction() {}

	void add(CREF(LPRDOFUNFunctionParam) _param);
	void add( const RDOFUNFunctionListElement* const _listElement );
	void add( const RDOFUNCalculateIf* const _calculateIf );
	LPRDOFUNFunctionParam findFUNFunctionParam( const std::string& paramName ) const;
	int findFUNFunctionParamNum( const std::string& paramName ) const;
	void createListCalc();
	void createTableCalc( const YYLTYPE& _elements_pos );
	void createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info );
	const std::string& name() const                                   { return src_info().src_text(); }
	const ParamList getParams() const { return params;                }

	void setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) calc);
	rdoRuntime::LPRDOFunCalc getFunctionCalc() const                   { return functionCalc;          }

	CREF(LPRDOTypeParam) getType() const                              { return retType;               }
	void insertPostLinked(CREF(rdoRuntime::LPRDOCalcFunctionCall) pCalc)
	{
		post_linked.push_back(pCalc);
	}

private:
	LPRDOTypeParam retType;
	ParamList      params;
	std::vector< const RDOFUNFunctionListElement* > elements;    // for list and table
	std::vector< const RDOFUNCalculateIf* >         calculateIf; // for algorithmic
	rdoRuntime::LPRDOFunCalc functionCalc;

	typedef std::vector<rdoRuntime::LPRDOCalcFunctionCall> PostLinkedList;
	PostLinkedList post_linked; // ��� ������������ ������
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
	LPRDORTPResType resType;

	RDOFUNGroup( RDOParser* _parser, const RDOParserSrcInfo& _res_info );
	RDOFUNGroup( const RDOParserObject* _parent, const RDOParserSrcInfo& _res_info );
	virtual ~RDOFUNGroup() {}
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
	rdoRuntime::LPRDOFunCalcSelect select;

public:
	RDOFUNSelect( RDOParser* _parser, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parser, RDOParserSrcInfo(_res_info.src_text()) )
	{
	}
	void initSelect( const RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty( const RDOParserSrcInfo& _empty_info );
	RDOFUNArithm* createFunSelectSize( const RDOParserSrcInfo& _size_info );
};

} // namespace rdoParse 

#endif // RDOFUN_H
