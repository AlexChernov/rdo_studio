#ifndef RDO_MB_RESOURCES_H
#define RDO_MB_RESOURCES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include "rdo_common/rdodebug.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"

namespace rdoParse
{
class RDOParser;
class RDORTPResType;
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
	typedef typename List::iterator       Iterator;
	typedef typename List::const_iterator CIterator;
	typedef typename List::const_iterator const_iterator;

	RDOList()
		: m_parser(NULL)
	{}

	RDOList(PTR(rdoParse::RDOParser) parser)
		: m_parser(parser)
	{}

	void operator= (CREF(RDOList<T>) obj)
	{
		m_parser = obj.m_parser;
		m_list.clear();
		for (CIterator it = obj.begin(); it != obj.end(); ++it)
			m_list.push_back(*it);
	}

	 Iterator     begin()       { return m_list.begin(); }
	 Iterator     end  ()       { return m_list.end();   }
	CIterator     begin() const { return m_list.begin(); }
	CIterator     end  () const { return m_list.end();   }
	ruint         size () const { return m_list.size();  }
	CIterator     found(CREF(tstring) name) const
	{
		return std::find_if(begin(), end(), rdoParse::compareNameRef<T>(name));
	}
	Iterator      found(CREF(tstring) name)
	{
		return std::find_if(begin(), end(), rdoParse::compareNameRef<T>(name));
	}
	rbool exist(CREF(tstring) name) const
	{
		return found( name ) != end();
	}
	CREF(T) operator[] (CREF(tstring) name) const
	{
		List::const_iterator it = found(name);
		if (it != end())
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
	List                      m_list;
	PTR(rdoParse::RDOParser)  m_parser;
};

// --------------------------------------------------------------------
// ---------- ������� ����� mbuilder-�������
// --------------------------------------------------------------------
#define MBUILDER_OBJECT(Class)                             \
class Class: public rdoParse::RDOParserSrcInfo             \
{                                                          \
friend class Class##List;                                  \
public:                                                    \
	Class(): m_exist( false ), m_name(_T("")) {}           \
	Class(CREF(Class) obj);                                \
                                                           \
	CREF(tstring)  name () const       { return m_name;  } \
	rbool          exist() const       { return m_exist; } \
	void operator= (CREF(Class) copy);                     \
                                                           \
private:                                                   \
	tstring  m_name;                                       \
	rbool    m_exist;

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
	RDOResType(CREF(rdoParse::LPRDORTPResType) rtp);
	// ������� ����� ���
	RDOResType(CREF(tstring) name, Type type = rt_permanent);

	MBUILDER_OBJECT(Param)
	friend class RDOResType;
	public:
		explicit Param(CREF(rdoParse::LPRDORTPParam) param);
		explicit Param(CREF(tstring) name, CREF(rdoParse::LPRDOTypeParam) type);
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__int>)  type,  CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());
		explicit Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__real>) type,  CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());
		explicit Param(CREF(tstring) name, CREF(rdoRuntime::RDOEnumType::Enums)              enums, CREF(rdoParse::RDOValue) default = rdoParse::RDOValue());

		CREF(rdoParse::LPRDOTypeParam)     type() const       { return m_type;                   }
		const rdoRuntime::RDOType::TypeID  typeID() const     { return m_type->type()->typeID(); }
		tstring                            typeStr() const    { return m_type->type()->name();   }

		rsint                     id() const          { return m_id;  }

		rbool                     hasDiap() const     { return m_min.typeID() != rdoRuntime::RDOType::t_unknow && m_max.typeID() != rdoRuntime::RDOType::t_unknow; }
		CREF(rdoParse::RDOValue)  getMin () const     { return m_min; }
		CREF(rdoParse::RDOValue)  getMax () const     { return m_max; }
		void                      setDiap(CREF(rdoParse::RDOValue) min, CREF(rdoParse::RDOValue) max);

		rbool                     hasDefault() const  { return m_type->default().defined(); }
		CREF(rdoParse::RDOValue)  getDefault() const  { return m_type->default();           }
		void                      setDefault(CREF(rdoParse::RDOValue) default);

		rdoParse::LPRDOEnumType   getEnum() const
		{
			ASSERT(typeID() == rdoRuntime::RDOType::t_enum);
			return type()->type().object_cast<rdoParse::RDOEnumType>();
		}

		rbool operator== (CREF(Param) param) const;

	private:
		rdoParse::LPRDOTypeParam   m_type;
		rdoParse::RDOValue         m_min;
		rdoParse::RDOValue         m_max;
		rsint                      m_id;

		template <class T>
		void initType(CREF(T) type)
		{
			m_type = rdo::Factory<rdoParse::RDOTypeParam>::create(type, rdoParse::RDOValue(), rdoParse::RDOParserSrcInfo());
			ASSERT(m_type);
		}
		template <>
		void initType(CREF(rdoRuntime::RDOEnumType::Enums) enums)
		{
			rdoParse::LPRDOEnumType pEnum = rdo::Factory<rdoParse::RDOEnumType>::create();
			ASSERT(pEnum)
			STL_FOR_ALL_CONST(rdoRuntime::RDOEnumType::Enums, enums, it)
			{
				pEnum->add(rdoParse::RDOValue::getIdentificator(*it));
			}
//			m_default = rdoParse::RDOValue(rdoParse::RDOValue::getIdentificator(m_default.value().getAsString()).value(), pEnum, rdoParse::RDOParserSrcInfo(m_default));
			m_type    = rdo::Factory<rdoParse::RDOTypeParam>::create(pEnum.object_cast<rdoParse::RDOType>(), rdoParse::RDOValue(), rdoParse::RDOParserSrcInfo());
			ASSERT(m_type);
		}
	};
	class ParamList: public RDOList<Param>
	{
	public:
		rbool append(REF(Param) param);
	};
	ParamList m_params;

	Type   getType    () const { return m_type;                 }
	rbool  isPermanent() const { return m_type == rt_permanent; }
	rsint  id         () const { return m_id;                   }

private:
	Type   m_type;
	rsint  m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
MBUILDER_OBJECT(RDOResource)
public:
	// ������������������� �� ������������� �������
	RDOResource(CREF(rdoParse::RDORSSResource) rss);
	// ������� ����� ������
	RDOResource(CREF(RDOResType) rtp, CREF(tstring) name);

	CREF(RDOResType)  getType() const { return m_rtp; }
	rsint             getID  () const { return m_id;  }

	typedef std::map< tstring, rdoParse::RDOValue > Params;

	Params::const_iterator begin() const { return m_params.begin(); }
	Params::const_iterator end  () const { return m_params.end();   }
	ruint                  size () const { return m_params.size();  }

	REF(Params::mapped_type)   operator[] (CREF(tstring) param);
	Params::const_iterator     operator[] (CREF(tstring) param) const;

	CPTR(rdoParse::RDORSSResource) getParserResource(CREF(rdoParse::RDOParser) parser) const;

	template <class T>
	rbool checkParserResourceType(CREF(rdoParse::RDOParser) parser) const
	{
		return dynamic_cast<CPTR(T)>(getParserResource(parser)) != NULL;
	}

	template <class T>
	PTR(rdoParse::RDORSSResource) createParserResource(REF(rdoParse::RDOParser) parser, rsint id = rdoParse::RDORSSResource::UNDEFINED_ID) const
	{
		rdoParse::LPRDORTPResType pRTP = parser.findRTPResType(getType().name());
		if (!pRTP)
			return NULL;

		return new T(&parser, RDOParserSrcInfo(name()), pRTP, id == rdoParse::RDORSSResource::UNDEFINED_ID ? getID() : id);
	}

	rbool fillParserResourceParams(PTR(rdoParse::RDORSSResource) toParserRSS) const;

private:
	RDOResType  m_rtp;
	Params      m_params;
	rsint       m_id;
};

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
class RDOResTypeList: public RDOList<RDOResType>
{
public:
	RDOResTypeList(PTR(rdoParse::RDOParser) parser);
	rbool append(REF(RDOResType) rtp);
};

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
class RDOResourceList: public RDOList<RDOResource>
{
public:
	RDOResourceList(PTR(rdoParse::RDOParser) parser);

	// --------------------------------------------------------------------
	// ---- ���������� *������* �������
	// --------------------------------------------------------------------
	template<class T> rbool append(REF(RDOResource) mbuilderRSS)
	{
		if (exist(mbuilderRSS.name()))
			return false;

		std::auto_ptr<rdoParse::RDORSSResource> parserRSS(mbuilderRSS.createParserResource<T>(*m_parser));
		if (!parserRSS.get())
			return false;
		if (!mbuilderRSS.fillParserResourceParams(parserRSS.get()))
			return false;

		parserRSS.get()->setTrace(true);

		mbuilderRSS.m_exist = true;
		m_list.push_back(mbuilderRSS);

		//! ������� ������� �� ������-������, ����� �� �� �������� ��� ������
		parserRSS.release();
		return true;
	}
	// --------------------------------------------------------------------
	// ---- ������ ������������� ������� �����
	// --------------------------------------------------------------------
	template<class T> rbool replace(REF(RDOResource) mbuilderRSSNew)
	{
		Iterator mbuilderRSSPrevIt = found(mbuilderRSSNew.name());
		if (mbuilderRSSPrevIt == end())
			return false;

		CPTR(rdoParse::RDORSSResource) parserRSSPrev = mbuilderRSSPrevIt->getParserResource(*m_parser);
		ASSERT(parserRSSPrev);

		std::auto_ptr<rdoParse::RDORSSResource> parserRSSNew(mbuilderRSSNew.createParserResource<T>(*m_parser, mbuilderRSSPrevIt->getID()));
		if (!parserRSSNew.get())
			return false;
		if (!mbuilderRSSPrevIt->fillParserResourceParams(parserRSSNew.get()))
			return false;
		parserRSSNew.get()->setTrace(parserRSSPrev->getTrace());
		mbuilderRSSNew.m_exist = true;
		m_list.push_back(mbuilderRSSNew);

		//! ������ ������
		m_parser->removeRSSResource(parserRSSPrev);
		m_list.erase(mbuilderRSSPrevIt);

		//! ������� ������� �� ������-������, ����� �� �� �������� ��� ������
		parserRSSNew.release();
		return true;
	}
};

} // rdoMBuilder

#endif // RDO_MB_RESOURCES_H
