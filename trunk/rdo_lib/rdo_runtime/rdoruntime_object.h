#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

#include "rdodefines.h"
#include <rdocommon.h>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOException
// ----------------------------------------------------------------------------
class RDOException
{
public:
	std::string mess;
	virtual std::string getType() const = 0;
	RDOException( const std::string& str ): mess( str ) {}
};

class RDORuntimeException: public RDOException
{
public:
	std::string getType() const { return "RDO Runtime Error"; }
	RDORuntimeException( const std::string& str ): RDOException( str ) {}
};

class RDORuntimeRDOValue: public RDOException
{
public:
	std::string getType() const { return "RDOValue Error"; }
	RDORuntimeRDOValue( const std::string& str ): RDOException( str ) {}
};

class RDOInternalException: public RDORuntimeException
{
public:
	std::string getType() const { return "RDO Internal Error"; }
	RDOInternalException( const std::string& str ): RDORuntimeException( str ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
class RDORuntime;
class RDORuntimeParent;

class RDORuntimeObject
{
friend class RDORuntime;
private:
	size_t        object_size; // ������ �������� �������
	static size_t memory_size; // ����� �������� ���� ��������

protected:
	RDORuntimeParent* parent;

	RDORuntimeObject( RDORuntimeParent* _parent );
	virtual void notify( RDORuntimeObject* from, unsigned int message, void* param = NULL ) {};

public:
	virtual ~RDORuntimeObject();
	void reparent( RDORuntimeParent* _parent );
	RDORuntimeParent* getParent() const { return parent;  }
//	RDORuntime*       getRuntime() const;

	void detach();

#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
class RDORuntimeParent: public RDORuntimeObject
{
protected:
	std::vector< RDORuntimeObject* > objects;

public:
	RDORuntimeParent( RDORuntimeParent* _parent );
	virtual ~RDORuntimeParent();

	void insertObject( RDORuntimeObject* object ) {
		if ( object ) {
//			TRACE( "insert object: %d\n", object );
			if ( object == this ) {
//				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
			} else {
				objects.push_back( object );
			}
		} else {
//			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
		}
	}
	void removeObject( RDORuntimeObject* object ) {
		std::vector< RDORuntimeObject* >::reverse_iterator it = std::find( objects.rbegin(), objects.rend(), object );
		if ( it != objects.rend() ) {
//			TRACE( "remove object: %d\n", object );
			// ������� it.base() �������� ����������� �������� � �����������,
			// �� ����� ���� ���������� ������� ���������
			it++;
			objects.erase( it.base() );
		} else {
//			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
		}
	}
	void deleteObjects() {
		std::vector< RDORuntimeObject* >::reverse_iterator it = objects.rbegin();
		while ( it != objects.rend() ) {
			delete *it;
			it = objects.rbegin();
		}
		objects.clear();
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSrcInfo
// ----------------------------------------------------------------------------
class RDOSrcInfo
{
public:
	struct Position {
		int first_line;
		int first_pos;
		int last_line;
		int last_pos;
		Position::Position():
			first_line( -1 ),
			first_pos( -1 ),
			last_line( -1 ),
			last_pos( -1 )
		{
		}
		Position::Position( int _first_line, int _first_pos, int _last_line, int _last_pos ):
			first_line( _first_line ),
			first_pos( _first_pos ),
			last_line( _last_line ),
			last_pos( _last_pos )
		{
		}
		bool empty() const
		{
			return first_line == -1 && first_pos == -1 && last_line == -1 && last_pos == -1;
		}
	};

private:
	Position                     position;
	std::string                  text_data;
	rdoModelObjects::RDOFileType file_type;

public:
	RDOSrcInfo():
		text_data( "" ),
		file_type( rdoModelObjects::TRC )
	{
	}
	void setSrcInfo( const RDOSrcInfo& _info ) {
		setSrcPos( _info.position );
		setSrcText( _info.text_data );
		setSrcFileType( _info.file_type );
	}
	void setSrcInfo( const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end ) {
		setSrcPos( begin.src_pos().first_line, begin.src_pos().first_pos, end.src_pos().last_line, end.src_pos().last_pos );
		setSrcText( begin.src_text() + delim + end.src_text() );
		setSrcFileType( begin.src_filetype() );
	}
	void setSrcPos( const Position& _position ) {
		position.first_line = _position.first_line;
		position.first_pos  = _position.first_pos;
		position.last_line  = _position.last_line;
		position.last_pos   = _position.last_pos;
	}
	void setSrcPos( const Position& _position_begin, const Position& _position_end ) {
		position.first_line = _position_begin.first_line;
		position.first_pos  = _position_begin.first_pos;
		position.last_line  = _position_end.last_line;
		position.last_pos   = _position_end.last_pos;
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		position.first_line = first_line;
		position.first_pos  = first_pos;
		position.last_line  = last_line;
		position.last_pos   = last_pos;
	}
	virtual void setSrcText( const std::string& value ) {
		text_data = value;
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value ) {
		file_type = value;
	}
	const RDOSrcInfo&                  src_info() const     { return *this;     }
	const Position&                    src_pos()  const     { return position;  }
	const std::string&                 src_text() const     { return text_data; }
	const rdoModelObjects::RDOFileType src_filetype() const { return file_type; }
	bool                               src_empty() const
	{
		return position.empty() && text_data.empty() && file_type == rdoModelObjects::TRC;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOStdVector
// ----------------------------------------------------------------------------
template <class T>
class RDOStdVector: public std::vector<T>
{
public:
	RDOStdVector()
	{
	}
	RDOStdVector( const T& item )
	{
		push_back( item );
	}
	RDOStdVector& operator() ( const T& item )
	{
		push_back( item );
		return *this;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOEnum
// ----------------------------------------------------------------------------
class RDOEnum: public RDORuntimeObject
{
public:
	typedef std::string             EnumItem;
	typedef std::vector< EnumItem > Enums;
	typedef Enums::const_iterator   CIterator;
	typedef RDOStdVector<EnumItem>  EnumArray;

	RDOEnum( RDORuntimeParent* _parent ):
		RDORuntimeObject( _parent )
	{
	}
	RDOEnum( RDORuntimeParent* _parent, const Enums& enums ):
		RDORuntimeObject( _parent ),
		m_enum( enums )
	{
	}
	void add( const std::string& next )
	{
		m_enum.push_back( next );
	}
	int findEnum( const std::string& val ) const
	{
		CIterator it = std::find( m_enum.begin(), m_enum.end(), val );
		return it != m_enum.end() ? it - m_enum.begin() : -1;
	}
	bool exist( const std::string& val ) const
	{
		return findEnum( val ) != -1;
	}

	bool            empty() const     { return m_enum.empty(); }
	const CIterator begin() const     { return m_enum.begin(); }
	const CIterator end() const       { return m_enum.end();   }
	const Enums&    getValues() const { return m_enum;	       }

	std::string str() const
	{
		std::string _str = "(";
		std::vector< std::string >::const_iterator it = begin();
		while ( it != end() )
		{
			_str += *it;
			it++;
			if ( it != end() )
			{
				_str += ", ";
			}
		}
		_str += ")";
		return _str;
	}

private:
	Enums m_enum;
};

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue
{
public:
	enum Type {
		rvt_unknow = 0,
		rvt_int    = 1,
		rvt_real   = 2,
		rvt_enum   = 3
	};

	RDOValue():
		m_type( rvt_unknow )
	{
		m_value.i_value = 0;
	}
	~RDOValue()
	{
	}
	RDOValue( const RDOValue& rdovalue ):
		m_type( rdovalue.m_type ),
		m_value( rdovalue.m_value ),
		m_data( rdovalue.m_data )
	{
	}
	RDOValue( Type type ):
		m_type( type )
	{
		switch ( m_type ) {
			case rvt_int : m_value.i_value = 0; break;
			case rvt_real: m_value.d_value = 0; break;
			case rvt_enum: m_value.i_value = 0; m_data = NULL; break;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue( int value ):
		m_type( rvt_int )
	{
		m_value.i_value = value;
	}
	RDOValue( unsigned int value ):
		m_type( rvt_int )
	{
		m_value.i_value = value;
	}
	RDOValue( double value ):
		m_type( rvt_real )
	{
		m_value.d_value = value;
	}
	RDOValue( RDOEnum& enums ):
		m_type( rvt_enum )
	{
		if ( enums.empty() )
		{
			RDORuntimeRDOValue("");
		}
		m_data          = &enums;
		m_value.i_value = enums.findEnum( enums.getValues()[0] );
	}
	RDOValue( RDOEnum& enums, const std::string& value ):
		m_type( rvt_enum )
	{
		m_data          = &enums;
		m_value.i_value = enums.findEnum( value );
		if ( m_value.i_value == -1 )
		{
			RDORuntimeRDOValue("");
		}
	}
	
	Type getType() const { return m_type; }
	
	int getInt() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value;
			case rvt_real: return m_value.d_value;
			case rvt_enum: return m_value.i_value;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	int getEnumAsInt() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value;
			case rvt_real: return m_value.d_value;
			case rvt_enum: return m_value.i_value;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	RDOEnum& getEnum() const
	{
		switch ( m_type ) {
			case rvt_enum: return *reinterpret_cast<RDOEnum*>(m_data);
			default      : throw RDORuntimeRDOValue("");
		}
	}
	double getDouble() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value;
			case rvt_real: return m_value.d_value;
			case rvt_enum: return m_value.i_value;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool getBool() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value ? true : false;
			case rvt_real: return m_value.d_value ? true : false;
			case rvt_enum: return m_value.i_value ? true : false;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsString() const
	{
		switch ( m_type ) {
			case rvt_int : return rdo::format( "%d", m_value.i_value );
			case rvt_real: return toString( m_value.d_value );
			case rvt_enum: return reinterpret_cast<RDOEnum*>(m_data)->getValues().at( m_value.i_value );
			default      : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsStringForTrace() const
	{
		switch ( m_type ) {
			case rvt_int : return rdo::format( "%d", m_value.i_value );
			case rvt_real: return toString( m_value.d_value );
			case rvt_enum: return rdo::format( "%d", m_value.i_value );
			default      : throw RDORuntimeRDOValue("");
		}
	}
	std::string getTypeAsString() const
	{
		switch ( m_type ) {
			case rvt_unknow: return "unknow";
			case rvt_int   : return "int";
			case rvt_real  : return "double";
			case rvt_enum  : return reinterpret_cast<RDOEnum*>(m_data)->str();
			default        : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue& operator= ( const RDOValue& rdovalue )
	{
		m_type  = rdovalue.m_type;
		m_value = rdovalue.m_value;
		m_data  = rdovalue.m_data;
		return *this;
	}
	bool operator< ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value < rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value < rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value < rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value < rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool operator> ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value > rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value > rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value > rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value > rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool operator!= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value != rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value != rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value != rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value != rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_enum: switch ( rdovalue.m_type ) {
								case rvt_enum: return m_value.i_value != rdovalue.m_value.i_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool operator== ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value == rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value == rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value  == rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value  == rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_enum: switch ( rdovalue.m_type ) {
								case rvt_enum: return m_data == rdovalue.m_data && m_value.i_value == rdovalue.m_value.i_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool operator<= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value <= rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value <= rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value <= rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value <= rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	bool operator>= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.i_value >= rdovalue.m_value.i_value;
								case rvt_real: return m_value.i_value >= rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int : return m_value.d_value >= rdovalue.m_value.i_value;
								case rvt_real: return m_value.d_value >= rdovalue.m_value.d_value;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue operator- () const
	{
		RDOValue rdovalue( *this );
		switch ( m_type ) {
			case rvt_int : rdovalue.m_value.i_value = -m_value.i_value; break;
			case rvt_real: rdovalue.m_value.d_value = -m_value.d_value; break;
			default      : throw RDORuntimeRDOValue("");
		}
		return rdovalue;
	}
	void operator+= ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.i_value += rdovalue.getInt(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.d_value += rdovalue.getDouble(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	void operator-= ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.i_value -= rdovalue.getInt(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.d_value -= rdovalue.getDouble(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	void operator*= ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int : m_value.i_value *= rdovalue.m_value.i_value; return;
								case rvt_real: m_value.d_value = ((double)m_value.i_value) * rdovalue.m_value.d_value; m_type = rvt_real; return;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.d_value *= rdovalue.getDouble(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	void operator/= ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int : switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.d_value = ((double)m_value.i_value) / rdovalue.getDouble(); m_type = rvt_real; return;
								default      : throw RDORuntimeRDOValue("");
							}
			case rvt_real: switch ( rdovalue.m_type ) {
								case rvt_int :
								case rvt_real: m_value.d_value = m_value.d_value / rdovalue.getDouble(); return;
								default      : throw RDORuntimeRDOValue("");
							}
			default      : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue operator+ ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 += rdovalue;
		return value2;
	}
	RDOValue operator- ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 -= rdovalue;
		return value2;
	}
	RDOValue operator* ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 *= rdovalue;
		return value2;
	}
	RDOValue operator/ ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 /= rdovalue;
		return value2;
	}

private:
	union {
		int    i_value;
		double d_value;
	}                       m_value;
	Type                    m_type;
	void*                   m_data;
};

inline std::ostream& operator<< ( std::ostream& stream, const RDOValue& rdovalue )
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

inline bool operator< ( const RDOValue& rdovalue1, const RDOValue& rdovalue2 )
{
	return const_cast<RDOValue&>(rdovalue1).operator<( rdovalue2 );
}

} // namespace rdoRuntime

namespace std
{

template <class _Ret, class _Tp>
class const_mem_fun_t: public unary_function<const _Tp*,_Ret> {
public:
	explicit const_mem_fun_t(_Ret (_Tp::*__pf)() const) : _M_f(__pf) {}
	_Ret operator()(const _Tp* __p) const { return (__p->*_M_f)(); }
private:
	_Ret (_Tp::*_M_f)() const;
};

template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_t: public binary_function<const _Tp*,_Arg,_Ret> {
public:
	explicit const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const) : _M_f(__pf) {}
	_Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }
private:
	_Ret (_Tp::*_M_f)(_Arg) const;
};

template <class _Ret, class _Tp>
inline const_mem_fun_t<_Ret,_Tp> mem_fun(_Ret (_Tp::*__f)() const)
{
	return const_mem_fun_t<_Ret,_Tp>(__f);
}

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun(_Ret (_Tp::*__f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f);
}

template <class _Ret, class _Tp, class _Arg>
inline const_mem_fun1_t<_Ret,_Tp,_Arg> mem_fun1(_Ret (_Tp::*__f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret,_Tp,_Arg>(__f);
}

} // namespace std

inline std::string operator+ ( char* str1, std::string& str2 )
{
	return std::string(str1).append(str2);
}

inline std::string operator+ ( std::string& str1, char* str2 )
{
	return std::string(str1).append(str2);
}

#endif // RDORUNTIME_OBJECT_H
