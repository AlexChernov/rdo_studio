#ifndef RDORUNTIME_OBJECT_H
#define RDORUNTIME_OBJECT_H

#include <rdocommon.h>
#include <vector>
#include <algorithm>

// ----------------------------------------------------------------------------
// ---------- toString
// ----------------------------------------------------------------------------
template < class Arg > inline std::string toString( Arg i )
{
   std::ostringstream str;
   str << i;
   return str.str();
}

// ----------------------------------------------------------------------------
// ---------- DeleteAllObjects
// ----------------------------------------------------------------------------
template < class Stor > void DeleteAllObjects( Stor& storage )
{
	Stor::reverse_iterator it = storage.rbegin();
	while ( it != storage.rend() ) {
		delete *it;
		it++;
	}
	storage.clear();
}

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOException
// ----------------------------------------------------------------------------
class RDOException
{
public:
	RDOException( const std::string& message ): m_message( message ) {}
	virtual std::string getType() const = 0;
	
	const std::string& message() const { return m_message; }

private:
	std::string m_message;
};

class RDORuntimeException: public RDOException
{
public:
	RDORuntimeException( const std::string& message ): RDOException( message ) {}
	virtual std::string getType() const { return "RDO Runtime Error"; }
};

class RDORuntimeRDOValue: public RDOException
{
public:
	RDORuntimeRDOValue( const std::string& message ): RDOException( message ) {}
	virtual std::string getType() const { return "RDOValue Error"; }
};

class RDOInternalException: public RDORuntimeException
{
public:
	RDOInternalException( const std::string& message ): RDORuntimeException( message ) {}
	virtual std::string getType() const { return "RDO Internal Error"; }
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
class RDORuntime;
class RDORuntimeParent;

class RDORuntimeObject
{
friend class RDORuntime;
public:
	virtual ~RDORuntimeObject();
	void reparent( RDORuntimeParent* parent );
	RDORuntimeParent* getParent() const { return m_parent;  }
//	RDORuntime*       getRuntime() const;

	void detach();

#ifndef _DEBUG
	void* operator new( size_t sz );
	void operator delete( void* v );
#endif

protected:
	RDORuntimeParent* m_parent;

	RDORuntimeObject( RDORuntimeParent* parent );
	virtual void notify( RDORuntimeObject* from, unsigned int message, void* param = NULL ) {};

private:
	size_t        m_object_size; // ������ �������� �������
	static size_t s_memory_size; // ����� �������� ���� ��������
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeContainer
// ----------------------------------------------------------------------------
class RDORuntimeContainer
{
protected:
	RDORuntimeContainer( RDORuntime* runtime ):
		m_runtime( runtime )
	{
	}

	RDORuntime* getRuntime() const { return m_runtime; }

private:
	RDORuntime* m_runtime;
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
class RDORuntimeParent: public RDORuntimeObject
{
public:
	RDORuntimeParent( RDORuntimeParent* parent );
	virtual ~RDORuntimeParent();

	void insertObject( RDORuntimeObject* object ) {
		if ( object ) {
//			TRACE( "insert object: %d\n", object );
			if ( object == this ) {
//				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
			} else {
				m_objects.push_back( object );
			}
		} else {
//			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
		}
	}
	void removeObject( RDORuntimeObject* object ) {
		std::vector< RDORuntimeObject* >::reverse_iterator it = std::find( m_objects.rbegin(), m_objects.rend(), object );
		if ( it != m_objects.rend() ) {
//			TRACE( "remove object: %d\n", object );
			// ������� it.base() �������� ����������� �������� � �����������,
			// �� ����� ���� ���������� ������� ���������
			it++;
			m_objects.erase( it.base() );
		} else {
//			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
		}
	}
	void deleteObjects() {
		std::vector< RDORuntimeObject* >::reverse_iterator it = m_objects.rbegin();
		while ( it != m_objects.rend() ) {
			delete *it;
			it = m_objects.rbegin();
		}
		m_objects.clear();
	}

protected:
	std::vector< RDORuntimeObject* > m_objects;
};

// ----------------------------------------------------------------------------
// ---------- RDOSrcInfo
// ----------------------------------------------------------------------------
class RDOSrcInfo
{
public:
	struct Position {
		int m_first_line;
		int m_first_pos;
		int m_last_line;
		int m_last_pos;
		Position::Position():
			m_first_line( -1 ),
			m_first_pos( -1 ),
			m_last_line( -1 ),
			m_last_pos( -1 )
		{
		}
		Position::Position( int first_line, int first_pos, int last_line, int last_pos ):
			m_first_line( first_line ),
			m_first_pos( first_pos ),
			m_last_line( last_line ),
			m_last_pos( last_pos )
		{
		}
		bool empty() const
		{
			return m_first_line == -1 && m_first_pos == -1 && m_last_line == -1 && m_last_pos == -1;
		}
	};

	RDOSrcInfo():
		m_text_data( "" ),
		m_file_type( rdoModelObjects::TRC )
	{
	}
	void setSrcInfo( const RDOSrcInfo& info ) {
		setSrcPos( info.m_position );
		setSrcText( info.m_text_data );
		setSrcFileType( info.m_file_type );
	}
	void setSrcInfo( const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end ) {
		setSrcPos( begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos );
		setSrcText( begin.src_text() + delim + end.src_text() );
		setSrcFileType( begin.src_filetype() );
	}
	void setSrcPos( const Position& position ) {
		m_position.m_first_line = position.m_first_line;
		m_position.m_first_pos  = position.m_first_pos;
		m_position.m_last_line  = position.m_last_line;
		m_position.m_last_pos   = position.m_last_pos;
	}
	void setSrcPos( const Position& position_begin, const Position& position_end ) {
		m_position.m_first_line = position_begin.m_first_line;
		m_position.m_first_pos  = position_begin.m_first_pos;
		m_position.m_last_line  = position_end.m_last_line;
		m_position.m_last_pos   = position_end.m_last_pos;
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos ) {
		m_position.m_first_line = first_line;
		m_position.m_first_pos  = first_pos;
		m_position.m_last_line  = last_line;
		m_position.m_last_pos   = last_pos;
	}
	virtual void setSrcText( const std::string& value ) {
		m_text_data = value;
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value ) {
		m_file_type = value;
	}
	const RDOSrcInfo&                  src_info() const     { return *this;       }
	const Position&                    src_pos()  const     { return m_position;  }
	const std::string&                 src_text() const     { return m_text_data; }
	const rdoModelObjects::RDOFileType src_filetype() const { return m_file_type; }
	bool                               src_empty() const
	{
		return m_position.empty() && m_text_data.empty() && m_file_type == rdoModelObjects::TRC;
	}

private:
	Position                     m_position;
	std::string                  m_text_data;
	rdoModelObjects::RDOFileType m_file_type;
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
	typedef std::string              EnumItem;
	typedef RDOStdVector< EnumItem > Enums;
	typedef Enums::const_iterator    CIterator;

	RDOEnum( RDORuntimeParent* parent ):
		RDORuntimeObject( parent )
	{
	}
	RDOEnum( RDORuntimeParent* parent, const Enums& enums ):
		RDORuntimeObject( parent ),
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
		rvt_int,
		rvt_real,
		rvt_enum,
		rvt_string
	};

	RDOValue():
		m_type( rvt_unknow )
	{
		m_value.i_value = 0;
	}
	~RDOValue()
	{
		switch ( m_type )
		{
			case rvt_string: {
				delete __string();
				break;
			}
		}
	}
	RDOValue( const RDOValue& rdovalue ):
		m_type( rdovalue.m_type ),
		m_value( rdovalue.m_value ),
		m_data( rdovalue.m_data )
	{
		switch ( m_type )
		{
			case rvt_string: {
				m_data = new std::string( *rdovalue.__string() );
				break;
			}
		}
	}
	RDOValue( Type type ):
		m_type( type )
	{
		switch ( m_type ) {
			case rvt_int   : m_value.i_value = 0; break;
			case rvt_real  : m_value.d_value = 0; break;
			case rvt_enum  : m_value.i_value = 0; m_data = NULL; break;
			case rvt_string: m_value.i_value = 0; m_data = new std::string(""); break;
			default        : throw RDORuntimeRDOValue("");
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
	RDOValue( const std::string& value ):
		m_type( rvt_string )
	{
		m_data = new std::string( value );
	}
	
	Type getType() const { return m_type; }
	
	int getInt() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value;
			case rvt_real: return (int)m_value.d_value;
			case rvt_enum: return m_value.i_value;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	int getEnumAsInt() const
	{
		switch ( m_type ) {
			case rvt_int : return m_value.i_value;
			case rvt_real: return (int)m_value.d_value;
			case rvt_enum: return m_value.i_value;
			default      : throw RDORuntimeRDOValue("");
		}
	}
	RDOEnum& getEnum() const
	{
		switch ( m_type ) {
			case rvt_enum: return *__enum();
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
	const std::string& getString() const
	{
		switch ( m_type ) {
			case rvt_string: return *__string();
			default        : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsString() const
	{
		switch ( m_type ) {
			case rvt_int   : return rdo::format( "%d", m_value.i_value );
			case rvt_real  : return toString( m_value.d_value );
			case rvt_enum  : return __enum()->getValues().at( m_value.i_value );
			case rvt_string: return *__string();
			default        : throw RDORuntimeRDOValue("");
		}
	}
	std::string getAsStringForTrace() const
	{
		switch ( m_type ) {
			case rvt_int   : return rdo::format( "%d", m_value.i_value );
			case rvt_real  : return toString( m_value.d_value );
			case rvt_enum  : return rdo::format( "%d", m_value.i_value );
			case rvt_string: return *__string();
			default        : throw RDORuntimeRDOValue("");
		}
	}
	std::string getTypeAsString() const
	{
		switch ( m_type ) {
			case rvt_unknow: return "unknow";
			case rvt_int   : return "int";
			case rvt_real  : return "double";
			case rvt_enum  : return __enum()->str();
			case rvt_string: return "string";
			default        : throw RDORuntimeRDOValue("");
		}
	}
	RDOValue& operator= ( const RDOValue& rdovalue )
	{
		switch ( m_type )
		{
			case rvt_string: {
				if ( __string() ) delete __string();
				break;
			}
		}
		m_type  = rdovalue.m_type;
		m_value = rdovalue.m_value;
		m_data  = rdovalue.m_data;
		switch ( m_type )
		{
			case rvt_string: {
				m_data = new std::string( *rdovalue.__string() );
				break;
			}
		}
		return *this;
	}
	bool operator== ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value == rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value == rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value  == rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value  == rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_enum  : switch ( rdovalue.m_type ) {
								case rvt_enum  : return m_data == rdovalue.m_data && m_value.i_value == rdovalue.m_value.i_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() == rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
		}
	}
	bool operator!= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value != rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value != rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value != rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value != rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_enum  : switch ( rdovalue.m_type ) {
								case rvt_enum  : return m_value.i_value != rdovalue.m_value.i_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() != rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
		}
	}
	bool operator< ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value < rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value < rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value < rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value < rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() < rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
		}
	}
	bool operator> ( const RDOValue& rdovalue )
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value > rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value > rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value > rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value > rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() > rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
		}
	}
	bool operator<= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value <= rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value <= rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value <= rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value <= rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() <= rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
		}
	}
	bool operator>= ( const RDOValue& rdovalue ) const
	{
		switch ( m_type ) {
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.i_value >= rdovalue.m_value.i_value;
								case rvt_real  : return m_value.i_value >= rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   : return m_value.d_value >= rdovalue.m_value.i_value;
								case rvt_real  : return m_value.d_value >= rdovalue.m_value.d_value;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: return __string() >= rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
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
			case rvt_int   : switch ( rdovalue.m_type ) {
								case rvt_int   :
								case rvt_real  : m_value.i_value += rdovalue.getInt(); return;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_real  : switch ( rdovalue.m_type ) {
								case rvt_int   :
								case rvt_real  : m_value.d_value += rdovalue.getDouble(); return;
								default        : throw RDORuntimeRDOValue("");
							}
			case rvt_string: switch ( rdovalue.m_type ) {
								case rvt_string: *__string() += *rdovalue.__string();
								default        : throw RDORuntimeRDOValue("");
							}
			default        : throw RDORuntimeRDOValue("");
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

	RDOEnum*     __enum()   const { return reinterpret_cast<RDOEnum*>(m_data);     }
	std::string* __string() const { return reinterpret_cast<std::string*>(m_data); }
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

#endif // RDORUNTIME_OBJECT_H
