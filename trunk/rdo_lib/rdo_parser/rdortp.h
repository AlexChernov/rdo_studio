#ifndef RDORTP_H
#define RDORTP_H

#include "rdoparser_object.h"
#include "rdoparser_value.h"
#include <rdoruntime_object.h>
#include <rdo_value.h>

namespace rdoParse 
{

int rtpparse( void* lexer );
int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rtperror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
class RDOFUNArithm;
class RDORTPDefVal;

class RDORTPParamType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	virtual void                  checkValue       ( const RDOValue& value ) const = 0;
	virtual rdoRuntime::RDOValue  getValue         ( const RDOValue& value ) const = 0;
	virtual rdoRuntime::RDOValue  getDefaultValue  ( const RDOParserSrcInfo& _src_info ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const = 0;

	virtual unsigned int getDiapTableFunc() const;

	virtual void writeModelStructure( std::ostream& stream ) const = 0;

	virtual const rdoRuntime::RDOType*   type() const = 0;
	rdoRuntime::RDOType::ID            typeID() const { return type()->id(); }

	void checkParamType( const RDOFUNArithm* const action, bool warning = true ) const;
	void checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const;

	const RDORTPDefVal& getDV() const { return *m_dv; }

protected:
	RDORTPDefVal* m_dv;

	// ��� ���������� �����, ��������, ��� ���������� ����������� ��������
	RDORTPParamType( RDOParser* _parser, RDORTPDefVal* _dv ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo(),
		m_dv( _dv )
	{
	}
	// ��� ���������� ����������
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo(),
		m_dv( _dv )
	{
	}
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		m_dv( _dv )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
// �������� ������� (RDORTPParam) ��� ���������(RDOFUNConst)
// � ������� getResType() ���� ���� ������������, �.�.
// �� ��� RDORTPResType ��� NULL ��� ���������
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPParam( RDORTPResType* _parent, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );
	const std::string&           name() const       { return src_info().src_text(); }
	const RDORTPParamType* const getType() const    { return m_parType; }
	const RDORTPResType* const   getResType() const { return m_resType; }
	void writeModelStructure( std::ostream& stream ) const;

protected:
	RDORTPParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );

	const RDORTPParamType* const m_parType;
	const RDORTPResType*   const m_resType;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConst
// ----------------------------------------------------------------------------
class RDOFUNConst: public RDORTPParam
{
public:
	RDOFUNConst( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );
};

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPResType( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const bool _permanent );
	const std::string& name() const          { return src_text();   };
	int getNumber() const                    { return m_number;     };
	bool isPermanent() const                 { return m_permanent;  };
	bool isTemporary() const                 { return !m_permanent; };

	void addParam( const RDORTPParam* const param );
	void addParam( const std::string param_name, rdoRuntime::RDOType::ID param_typeID );
	const RDORTPParam* findRTPParam( const std::string& param ) const;

	int getRTPParamNumber( const std::string& param ) const;
	const std::vector< const RDORTPParam* >& getParams() const { return m_params; }


	void writeModelStructure( std::ostream& stream ) const;

//	const std::vector< const RDORTPFuzzyParam* >& getFuzzyParams() const { return m_fuzzy_params; }
//	void addFuzzyParam( const RDORTPFuzzyParam* const fuzzy_param );
//	int getRTPFuzzyParamNumber( const std::string& fuzzy_param ) const;
//	const RDORTPFuzzyParam* findRTPFuzzyParam( const std::string& fuzzy_param ) const;
private:
	const int                         m_number;
	const bool                        m_permanent;
	std::vector< const RDORTPParam* > m_params;
//	std::vector< const RDORTPFuzzyParam* > m_fuzzy_params;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - �������� ��-���������
// ----------------------------------------------------------------------------
// ������� ��������� � �������, � ����� ���� reparent �� ��� �������
// ----------------------------------------------------------------------------
class RDORTPDefVal: public RDOParserObject
{
public:
	RDORTPDefVal( RDOParser* _parser ):
		RDOParserObject( _parser )
	{
	}
	RDORTPDefVal( RDOParser* _parser, const RDOValue& value ):
		RDOParserObject( _parser ),
		m_value( value )
	{
	}
	RDORTPDefVal( const RDORTPDefVal& copy ):
		RDOParserObject( copy.parser() ),
		m_value( copy.m_value )
	{
	}

	bool isExist() const { return m_value.defined(); }

	const RDOValue& value() const { return m_value; }

	void setSrcInfo( const RDOParserSrcInfo& src_info )
	{
		m_value.setSrcInfo( src_info );
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
	{
		m_value.setSrcPos( first_line, first_pos, last_line, last_pos );
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value )
	{
		m_value.setSrcFileType( value );
	}

private:
	RDOValue m_value;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPDiap - ������ ������� ���������� ��� integer � real
// ----------------------------------------------------------------------------
// ������� ��������� � �������, � ����� ���� reparent �� ��� �������
// ----------------------------------------------------------------------------
template<class T>
class RDORTPDiap: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPDiap( RDOParser* _parser ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo(),
		m_exist( false ),
		m_min_value( 0 ),
		m_max_value( 0 )
	{
	}
	RDORTPDiap( RDOParser* _parser, const RDORTPDiap<T>& copy ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( copy.src_info() ),
		m_exist( copy.m_exist ),
		m_min_value( copy.m_min_value ),
		m_max_value( copy.m_max_value )
	{
	}
	RDORTPDiap( const RDORTPDiap<T>& copy ):
		RDOParserObject( copy.parser() ),
		RDOParserSrcInfo( copy.src_info() ),
		m_exist( copy.m_exist ),
		m_min_value( copy.m_min_value ),
		m_max_value( copy.m_max_value )
	{
	}
	RDORTPDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info ),
		m_exist( false ),
		m_min_value( 0 ),
		m_max_value( 0 )
	{
	}
	RDORTPDiap( RDOParser* _parser, T min_value, T max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& max_value_pos ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info ),
		m_exist( true ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
		init( &max_value_pos );
	}
	RDORTPDiap( RDOParser* _parser, T min_value, T max_value ):
		RDOParserObject( _parser ),
		m_exist( true ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
		init( NULL );
	}
	bool isExist() const { return m_exist;     }
	T  getMin() const    { return m_min_value; }
	T  getMax() const    { return m_max_value; }

private:
	bool m_exist;
	T    m_min_value;
	T    m_max_value;

	void init( const YYLTYPE* const max_value_pos )
	{
		if ( max_value_pos && m_min_value > m_max_value ) {
			parser()->error( *max_value_pos, "����� ������� ��������� ������ ���� ������ ������" );
		}
		setSrcText( rdo::format("[%s..%s]", rdoRuntime::RDOValue(m_min_value).getAsString().c_str(), rdoRuntime::RDOValue(m_max_value).getAsString().c_str()) );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - integer ��������, ��������, [1 .. 4]
// ----------------------------------------------------------------------------
typedef RDORTPDiap<int>    RDORTPIntDiap;
// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - real ��������, ��������, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
typedef RDORTPDiap<double> RDORTPRealDiap;

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
class RDORTPIntParamType: public RDORTPParamType
{
public:
	RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual unsigned int getDiapTableFunc() const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const rdoRuntime::RDOType* type() const { return &rdoRuntime::g_int; }

	const RDORTPIntDiap& getDiap() const { return *m_diap; }

private:
	RDORTPIntDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
class RDORTPRealParamType: public RDORTPParamType
{
public:
	RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const rdoRuntime::RDOType* type() const { return &rdoRuntime::g_real; }
	const RDORTPRealDiap& getDiap() const { return *m_diap; }

private:
	RDORTPRealDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPEnum( const RDOParserObject* _parent, const std::string& first );
	virtual ~RDORTPEnum();

	void add( const RDOParserSrcInfo& next );
	rdoRuntime::RDOValue     findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const;
	rdoRuntime::RDOValue     getFirstValue() const;
	rdoRuntime::RDOEnumType& getEnums() { return *m_enums; }

private:
	rdoRuntime::RDOEnumType* m_enums;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
class RDORTPEnumParamType: public RDORTPParamType
{
public:
	RDORTPEnum* m_enum;
	std::string enum_name; // ������������ � ���������� �� �������
	bool        enum_fun;  // ������������ � ���������� �� �������
	RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );

	virtual void                  checkValue       ( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue         ( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getDefaultValue  ( const RDOParserSrcInfo& _src_info ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual unsigned int getDiapTableFunc() const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const rdoRuntime::RDOType* type() const { return &m_enum->getEnums(); }

private:
	void init_src_info();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPStringParamType
// ----------------------------------------------------------------------------
class RDORTPStringParamType: public RDORTPParamType
{
public:
	RDORTPStringParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const rdoRuntime::RDOType* type() const { return &rdoRuntime::g_string; }
};

// ----------------------------------------------------------------------------
//------------------------------ FOR FUZZY LOGIC ------------------------------	
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftPoint - ����� �-�� �������������� ��������� �����
// ----------------------------------------------------------------------------
class RDORTPFuzzyMembershiftPoint: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftPoint( RDOParser* _parser, const RDOParserSrcInfo& _src_info, double x_value, double y_value ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_x_value( x_value ),
		m_y_value( y_value )
		{
		}
	double  getX() const { return m_x_value; }
	double  getY() const { return m_y_value; }

private:
	double    m_x_value;
	double    m_y_value;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftFun - �-�� �������������� ��� ��������� �����
// ----------------------------------------------------------------------------

class RDORTPFuzzyMembershiftFun: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftFun( RDOParser* _parser ):
		RDOParserObject( _parser )
		{
		}
	typedef RDORTPFuzzyMembershiftPoint* Item;
	typedef std::vector< Item >          Items;

	void add( Item point )
	{
		m_points.push_back( point );
	}
	double  getVal() const 
	{ 
		return m_value; 
	}

private:
	Items m_points;	// �����, ������������ �-�� ��������������
	double m_value;	// �������� �-�� �������������� ��� ����������� ������� ��������
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTerm - �������� ������
// ----------------------------------------------------------------------------
class RDORTPFuzzyTerm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyTerm( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyMembershiftFun* membersfift_fun):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_fun (membersfift_fun)
		{
		}
	
	const	std::string& name()	const	{ return src_info().src_text(); }
	double  MemberShift()			const	{ return m_fun->getVal(); }

private:
	RDORTPFuzzyMembershiftFun* m_fun;
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTermsSet - ����� �������� ������ ���������
// ----------------------------------------------------------------------------
class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
{
	public:
	RDORTPFuzzyTermsSet( RDOParser* _parser ):
		RDOParserObject( _parser )
		{
		}
	typedef RDORTPFuzzyTerm*	Item;
	typedef std::vector< Item >	Items;

	void add( Item term )
	{
		m_terms.push_back( term );
	}

private:
	Items m_terms;	// ����� �������� ������ ���������

};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyParam
// ----------------------------------------------------------------------------
class RDORTPFuzzyParam : public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyTermsSet* terms_set ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_set (terms_set)
		{
		}
	const std::string&           name() const       { return src_info().src_text(); }

private:
	RDORTPFuzzyTermsSet* m_set; // ����� �������� ���������
};

} // namespace rdoParse

#endif // RDORTP_H
