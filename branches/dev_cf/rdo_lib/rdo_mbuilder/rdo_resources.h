#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoruntime_object.h>
#include <rdoparser_object.h>
#include <rdoparser.h>
#include <rdorss.h>
#include <list>

namespace rdoParse
{
class RDOParser;
class RDORTPResType;
class RDORTPParam;
class RDORSSResource;
}

namespace rdoMBuilder {

// --------------------------------------------------------------------
// ---------- RDOList
// --------------------------------------------------------------------
template <class T>
class RDOList
{
public:
	typedef std::list< T >                List;
	typedef typename List::const_iterator CIterator;

	RDOList():
		m_parser( NULL )
	{
	}
	RDOList( rdoParse::RDOParser* parser ):
		m_parser( parser )
	{
	}

	CIterator     begin() const { return m_list.begin(); }
	CIterator     end  () const { return m_list.end();   }
	unsigned int  size () const { return m_list.size();  }
	CIterator     found( const std::string& name ) const
	{
		return std::find_if( begin(), end(), rdoParse::compareNameRef<T>(name) );
	}
	bool exist( const std::string& name ) const
	{
		return found( name ) != end();
	}
	const T& operator[] ( const std::string& name ) const
	{
		List::const_iterator it = found( name );
		if ( it != end() )
		{
			return *it;
		}
		else
		{
			static T __t;
			return __t;
		}
	}

protected:
	List                 m_list;
	rdoParse::RDOParser* m_parser;
};

// --------------------------------------------------------------------
// ---------- ������� ����� mbuilder-�������
// --------------------------------------------------------------------
#define MBUILDER_OBJECT( Class )                           \
class Class                                                \
{                                                          \
friend class Class##List;                                  \
public:                                                    \
	Class(): m_exist( false ), m_name("") {}               \
                                                           \
	const std::string& name() const    { return m_name;  } \
	bool               exist() const   { return m_exist; } \
                                                           \
private:                                                   \
	std::string m_name;                                    \
	bool        m_exist;

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResType)
public:
	enum Type
	{
		rt_permanent,
		rt_temporary
	};
	// ������������������� �� ������������� ����
	RDOResType( const rdoParse::RDORTPResType& rtp );
	// ������� ����� ���
	RDOResType( const std::string& name, Type type = rt_permanent );

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		Param( const rdoParse::RDORTPParam& param );
		Param( const std::string& name, rdoRuntime::RDOValue::Type type );
		Param( const std::string& name, rdoRuntime::RDOValue::Type type, const rdoRuntime::RDOValue& def );
		Param( const std::string& name, const rdoRuntime::RDOValue& type );
		Param( const std::string& name, const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max, const rdoRuntime::RDOValue& def = rdoRuntime::RDOValue::rvt_unknow );
		Param( const std::string& name, rdoRuntime::RDOEnum* enums );
		Param( const std::string& name, rdoRuntime::RDOEnum* enums, const rdoRuntime::RDOEnum::EnumItem& defaultValue );

		rdoRuntime::RDOValue        getTypeObject() const { return m_type;                   }
		rdoRuntime::RDOValue::Type  getType() const       { return m_type.getType();         }
		std::string                 getTypeStr() const    { return m_type.getTypeAsString(); }
		int                         id() const            { return m_id;                     }

		bool                        hasDiap() const    { return m_min.getType() != rdoRuntime::RDOValue::rvt_unknow && m_max.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getMin() const     { return m_min; }
		const rdoRuntime::RDOValue& getMax() const     { return m_max; }
		void                        setDiap( const rdoRuntime::RDOValue& min, const rdoRuntime::RDOValue& max );

		bool                        hasDefault() const { return m_default.getType() != rdoRuntime::RDOValue::rvt_unknow; }
		const rdoRuntime::RDOValue& getDefault() const { return m_default; }
		void                        setDefault( const rdoRuntime::RDOValue& def );

		const rdoRuntime::RDOEnum&  getEnum() const    { return m_type.getEnum(); }

		bool operator== ( const Param& param ) const;

	private:
		rdoRuntime::RDOValue   m_type;
		rdoRuntime::RDOValue   m_min;
		rdoRuntime::RDOValue   m_max;
		rdoRuntime::RDOValue   m_default;
		int                    m_id;
	};
	class ParamList: public RDOList<Param>
	{
	public:
		bool append( Param& param );
	};
	ParamList m_params;

	Type   getType()     const { return m_type;                 }
	bool   isPermanent() const { return m_type == rt_permanent; }
	int    id()          const { return m_id;                   }

private:
	Type   m_type;
	int    m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
	// ������������������� �� ������������� �������
	RDOResource( const rdoParse::RDORSSResource& rss );
	// ������� ����� ������
	RDOResource( const RDOResType& rtp, const std::string& name );

	const RDOResType&  getType() const { return m_rtp;  }

	typedef std::map< std::string, rdoRuntime::RDOValue > Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	unsigned int           size () const { return m_params.size();  }
	rdoRuntime::RDOValue&  operator[] ( const std::string& param );
	Params::const_iterator operator[] ( const std::string& param ) const;

private:
	RDOResType m_rtp;
	Params     m_params;
};

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
	RDOResTypeList( rdoParse::RDOParser* parser );
	bool append( RDOResType& rtp );
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList( rdoParse::RDOParser* parser );

	// --------------------------------------------------------------------
	// ---- ���������� *������* �������
	// --------------------------------------------------------------------
	template<class T> bool append( RDOResource& rss )
	{
		if ( std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResource>(rss.name())) == end() )
		{
			const rdoParse::RDORTPResType* pRTP = m_parser->findRTPResType(rss.getType().name());
			if ( !pRTP )
			{
				return false;
			}
			rdoParse::RDORSSResource* pRSS = new T( m_parser, rss.name(), pRTP );
			RDOResType::ParamList::List::const_iterator param_it = rss.getType().m_params.begin();
			while ( param_it != rss.getType().m_params.end() )
			{
				RDOResource::Params::const_iterator value_it = const_cast<const RDOResource&>(rss)[param_it->name()];
				if ( value_it == rss.end() )
				{
					delete pRSS;
					return false;
				}
				pRSS->addValue( value_it->second );
				param_it++;
			}
			rss.m_exist = true;
			m_list.push_back( rss );
			return true;
		}
		else
		{
			return false;
		}
	}
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
