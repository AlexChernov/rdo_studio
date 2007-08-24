#ifndef RDORTP_RTP
#define RDORTP_RTP

#include "rdoparser_object.h"
#include <rdoruntime_object.h>

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
	enum ParamType { pt_int = 0, pt_real = 1, pt_enum = 2, pt_unknow = 3 };
	RDORTPDefVal* dv;
	// ��� ���������� �����, ��������, ��� ���������� ����������� ��������
	RDORTPParamType( RDOParser* _parser, RDORTPDefVal* _dv ):
		RDOParserObject( _parser ),
		dv( _dv )
	{
	}
	// ��� ���������� ����������
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo(),
		dv( _dv )
	{
	}
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		dv( _dv )
	{
	}
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual int getDiapTableFunc() const = 0;
	virtual ParamType getType() const = 0;
	virtual int writeModelStructure() const = 0;

	void checkParamType( const RDOFUNArithm* const action ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
// �������� ������� ��� ���������, �.�. � ��������� ���� ���� ������������,
// �.�. �� ��� RDORTPResType ��� NULL (��� ���������)
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo
{
private:
	std::string name;
	const RDORTPParamType* const parType;
	const RDORTPResType*   const resType;

public:
	RDORTPParam( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info );
	RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType );
	RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info );
	const std::string&           getName() const    { return name;    }
	const RDORTPParamType* const getType() const    { return parType; }
	const RDORTPResType* const   getResType() const { return resType; }
	int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	std::string                       name;
	const int                         number;
	const bool                        permanent;
	std::vector< const RDORTPParam* > params;

public:
	RDORTPResType( RDOParser* _parser, const std::string& _name, const bool _permanent );
	const std::string& getName() const       { return name;       };
	int getNumber() const                    { return number;     };
	bool isPermanent() const                 { return permanent;  };
	bool isTemporary() const                 { return !permanent; };

	void addParam( const RDORTPParam* const param );
	const RDORTPParam* findRTPParam( const std::string& param ) const;
	int getRTPParamNumber( const std::string& param ) const;
	const std::vector< const RDORTPParam* >& getParams() const { return params; }

	int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - �������� ��-���������, ������� �����
// ----------------------------------------------------------------------------
// ������� ��������� � �������, � ����� ���� reparent �� ��� �������
// ----------------------------------------------------------------------------
class RDORTPDefVal: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	RDORTPDefVal( const RDORTPDefVal& dv );
	RDORTPDefVal( bool _exist );
	RDORTPDefVal( bool _exist, const RDOParserSrcInfo& _src_info );
	virtual int getIntValue() const;
	virtual double getRealValue() const;
	virtual const std::string& getEnumValue() const;
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDefVal - �������� ��-���������, ��������, = 20
// ----------------------------------------------------------------------------
class RDORTPIntDefVal: public RDORTPDefVal
{
private:
	int val;

public:
	RDORTPIntDefVal():
		RDORTPDefVal( false )
	{
	}
	RDORTPIntDefVal( const RDORTPIntDefVal& dv ):
		RDORTPDefVal( dv ),
		val( dv.val )
	{
	}
	RDORTPIntDefVal( int _val ):
		RDORTPDefVal( true ),
		val( _val )
	{
		setSrcText( rdo::format("%d", val) );
	}
	RDORTPIntDefVal( const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( false, _src_info )
	{
	}
	RDORTPIntDefVal( int _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( true, _src_info ),
		val( _val )
	{
		setSrcText( rdo::format("%d", val) );
	}
	virtual int getIntValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - ��������, ��������, [1 .. 4]
// ----------------------------------------------------------------------------
// ������� ��������� � �������, � ����� ���� reparent �� ��� �������
// ----------------------------------------------------------------------------
class RDORTPIntDiap: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	int minVal, maxVal;
	RDORTPIntDiap();
	RDORTPIntDiap( const RDORTPIntDiap& diap );
	RDORTPIntDiap( const RDOParserSrcInfo& _src_info );
	RDORTPIntDiap( int _minVal, int _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos );
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
class RDORTPIntParamType: public RDORTPParamType
{
private:
	void init();

public:
	RDORTPIntDiap* diap;
	RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;					// the function also check range if exist
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_int; }
	virtual int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDefVal - �������� ��-���������, ��������, = 20.2
// ----------------------------------------------------------------------------
class RDORTPRealDefVal: public RDORTPDefVal
{
private:
	double val;

public:
	RDORTPRealDefVal():
		RDORTPDefVal( false )
	{
	}
	RDORTPRealDefVal( const RDORTPIntDefVal& dv ):
		RDORTPDefVal( dv ),
		val( dv.getRealValue() )
	{
	}
	RDORTPRealDefVal( double _val ):
		RDORTPDefVal( true ),
		val( _val )
	{
		setSrcText( rdo::format("%f", val) );
	}
	RDORTPRealDefVal( const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( false, _src_info )
	{
	}
	RDORTPRealDefVal( double _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( true, _src_info ),
		val( _val )
	{
		setSrcText( rdo::format("%f", val) );
	}
	virtual double getRealValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - ��������, ��������, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
// ������� ��������� � �������, � ����� ���� reparent �� ��� �������
// ----------------------------------------------------------------------------
class RDORTPRealDiap: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	double minVal, maxVal;
	RDORTPRealDiap();
	RDORTPRealDiap( const RDORTPRealDiap& diap );
	RDORTPRealDiap( const RDOParserSrcInfo& _src_info );
	RDORTPRealDiap( double _minVal, double _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos );
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
class RDORTPRealParamType: public RDORTPParamType
{
private:
	void init();

public:
	RDORTPRealDiap* diap;
	RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;				// this function too
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info )const ; 	// the function also check range if exist
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_real; }
	virtual int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOParserObject, public RDOParserSrcInfo
{
public:
	std::vector< std::string > enumVals;
	RDORTPEnum( const RDOParserObject* _parent, const std::string& first ):
		RDOParserObject( _parent )
	{
		enumVals.push_back( first );
	}
	void add( const std::string& next, const YYLTYPE& _pos );
	int findEnumValueWithThrow( const RDOParserSrcInfo& _src_info, const std::string& val ) const;
	int findEnumValueWithoutThrow( const std::string& val ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumDefVal
// ----------------------------------------------------------------------------
class RDORTPEnumDefVal: public RDORTPDefVal
{
private:
	std::string val;

public:
	RDORTPEnumDefVal():
		RDORTPDefVal( false ),
		val( "" )
	{
	}
	RDORTPEnumDefVal( const RDORTPEnumDefVal& dv ):
		RDORTPDefVal( dv ),
		val( dv.val )
	{
		setSrcText( val );
	}
	RDORTPEnumDefVal( const std::string& _val ):
		RDORTPDefVal( true ),
		val( _val )
	{
		setSrcText( val );
	}
	RDORTPEnumDefVal( const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( false, _src_info ),
		val( "" )
	{
	}
	RDORTPEnumDefVal( const std::string& _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( true, _src_info ),
		val( _val )
	{
		setSrcText( val );
	}
	virtual const std::string& getEnumValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
class RDORTPEnumParamType: public RDORTPParamType
{
private:
	void init_src_info();

public:
	RDORTPEnum* enu;
	std::string enum_name; // ������������ � ���������� �� �������
	bool        enum_fun;  // ������������ � ���������� �� �������
	RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPEnumDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_enum; }
	virtual int writeModelStructure() const;
};

} // namespace rdoParse

#endif // RDORTP_RTP
