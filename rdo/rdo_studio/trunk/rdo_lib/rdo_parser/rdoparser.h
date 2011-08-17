/*
 * copyright: (c) RDO-Team, 2011
 * file     : rdoparser.h
 * author   : ���� ���������, ������ ������
 * date     : 
 * brief    : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_H_
#define _RDOPARSER_H_

// ====================================================================== INCLUDES
#include <algorithm>
#include <stack>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdoindexedstack.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/rdosmart_ptr_wrapper.h"
#include "rdo_common/rdoidgenerator.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdoparser_base.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdopatpreparse.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdopatpreparse.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdopmd.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/context/context_find_i.h"
#include "rdo_lib/rdo_parser/context/stack.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOTypeParam;
class RDOArrayType;

class RDORTPFuzzyParam;

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
#define DEFINE_OBJECT_CONTAINER_MINIMUM(TYPE, NAME) \
public: \
	typedef std::vector<TYPE> NAME##List; \
	void                insert##NAME (TYPE value); \
	TYPE                getLast##NAME()       { return !m_all##NAME.empty() ? m_all##NAME.back() : NULL; } \
	CREF(NAME##List)    get##NAME##s () const { return m_all##NAME; } \
private: \
	NAME##List m_all##NAME;

#define DEFINE_OBJECT_CONTAINER_WITHNAME(TYPE, NAME) \
public: \
	const TYPE find##NAME  (CREF(tstring) name) const; \
	rbool      remove##NAME(const TYPE item);

#define DEFINE_OBJECT_CONTAINER_NONAME(NAME) \
DEFINE_OBJECT_CONTAINER_MINIMUM(LPRDO##NAME, NAME)

#define DEFINE_OBJECT_CONTAINER_NONAME_LP(NAME) \
DEFINE_OBJECT_CONTAINER_MINIMUM(LPRDO##NAME, NAME)

#define DEFINE_OBJECT_CONTAINER(NAME) \
DEFINE_OBJECT_CONTAINER_MINIMUM(LPRDO##NAME, NAME) \
DEFINE_OBJECT_CONTAINER_WITHNAME(LPRDO##NAME, NAME)

PREDECLARE_POINTER(RDOParser);

CLASS(RDOParser):
	    INSTANCE_OF      (Context                 )
	AND IMPLEMENTATION_OF(IContextFind            )
	AND IMPLEMENTATION_OF(IContextCreateExpression)
{
DECLARE_FACTORY(RDOParser);

public:
DEFINE_OBJECT_CONTAINER(PATPattern     );
DEFINE_OBJECT_CONTAINER(RTPResType     );
DEFINE_OBJECT_CONTAINER(RSSResource    );
DEFINE_OBJECT_CONTAINER(FRMFrame       );
DEFINE_OBJECT_CONTAINER(FUNConstant    );
DEFINE_OBJECT_CONTAINER(FUNFunction    );
DEFINE_OBJECT_CONTAINER(FUNSequence    );
DEFINE_OBJECT_CONTAINER(DPTSearch      );
DEFINE_OBJECT_CONTAINER(DPTSome        );
DEFINE_OBJECT_CONTAINER(DPTPrior       );
DEFINE_OBJECT_CONTAINER(DPTFreeActivity);
DEFINE_OBJECT_CONTAINER(Event          );
DEFINE_OBJECT_CONTAINER(ResultGroup    );
DEFINE_OBJECT_CONTAINER(PROCProcess    );

DEFINE_OBJECT_CONTAINER_NONAME(FUNGroup   );
DEFINE_OBJECT_CONTAINER_NONAME(DPTFree    );

public:
	virtual void init  ();
	virtual void deinit();

	CREF(rdoRuntime::LPRDORuntime) runtime() const { return m_pRuntime; }

	rbool             isPattern       () const { return m_pattern;     }
	REF(FUNGroupList) getFUNGroupStack()       { return m_allFUNGroup; }

	void  checkFunctionName    (CREF(RDOParserSrcInfo) src_info);
	void  checkActivityName    (CREF(RDOParserSrcInfo) src_info);
	void  checkDPTName         (CREF(RDOParserSrcInfo) src_info);

	void  insertChanges        (CREF(tstring) name, CREF(tstring) value);

	rbool isHaveKWResources    ()            const { return m_have_kw_Resources;     }
	void  setHaveKWResources   (rbool value)       { m_have_kw_Resources = value;    }
	rbool isHaveKWResourcesEnd ()            const { return m_have_kw_ResourcesEnd;  }
	void  setHaveKWResourcesEnd(rbool value)       { m_have_kw_ResourcesEnd = value; }

	rbool isCurrentDPTSearch   ();
	rbool isCurrentDPTPrior    ();

	ruint getRTP_id     () const { return m_allRTPResType.size()  + 1; }
	ruint getRSS_id     () const { return m_allRSSResource.size() + 0; }
	ruint getPAT_id     () const { return m_allPATPattern.size()  + 0; }
	ruint getPMD_id     ()       { return m_resultGeneratorID.get();   }
	ruint getFUNCONST_id() const { return m_allFUNConstant.size() + 0; }

	tstring getModelStructure();
	tstring getChanges       () const;

	LPRDOSMR getSMR() const              { return m_pSMR;                }
	void     setSMR(CREF(LPRDOSMR) pSMR) { m_pSMR = pSMR;                }
	rbool    hasSMR() const              { return m_pSMR ? true : false; }

	void parse();
	void parse(REF(std::istream) stream);

	CREF(Error) error() const { return m_error; }
	 REF(Error) error()       { return m_error; }

	PTR(RDOValue) addValue(PTR(RDOValue) value)
	{
		runtime()->memory_insert(sizeof(rdoParse::RDOValue));
		m_allValues.push_back(value);
		return value;
	}

	class Stack: private rdo::IndexedStack<rdo::LPISmartPtrWrapper>
	{
	friend class RDOParser;
	public:
		typedef rdo::IndexedStack<rdo::LPISmartPtrWrapper> IndexedStack;

		template <class T>
		IndexedStack::ID push(CREF(rdo::intrusive_ptr<T>) pObject)
		{
			rdo::LPISmartPtrWrapper pWrapper = new rdo::smart_ptr_wrapper<T>(pObject);
			return IndexedStack::push(pWrapper);
		}
		template <class T>
		rdo::intrusive_ptr<T> pop(IndexedStack::ID id)
		{
			rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
			ASSERT(pWrapper);
			rdo::intrusive_ptr<T> pObject = *reinterpret_cast<PTR(rdo::intrusive_ptr<T>)>(pWrapper->getSmartPtr());
			pWrapper->destroy();
			return pObject;
		}

		rdo::LPISmartPtrWrapper raw_pop(IndexedStack::ID id)
		{
			rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
			ASSERT(pWrapper);
			return pWrapper;
		}

	private:
		void clear()
		{
			STL_FOR_ALL(m_stack, it)
			{
				it->second->destroy();
			}
		}
	};

	REF(Stack) stack()
	{
		return m_movementObjectList;
	}

	typedef std::vector<LPTypeInfo> PreCastTypeList;
	CREF(PreCastTypeList) getPreCastTypeList() const
	{
		return m_preCastTypeList;
	}
	void insertPreCastType(CREF(LPTypeInfo) pType)
	{
		m_preCastTypeList.push_back(pType);
	}

	LPContextStack contextStack();
	LPContext      context     () const;

	static rdoModelObjects::RDOFileType getFileToParse();
	static ruint                        lexer_loc_line();
	static ruint                        lexer_loc_pos ();
	static LPRDOParser                  s_parser      ();

protected:
	RDOParser();
	virtual ~RDOParser();

	LPRDOParserItem m_parser_item;

	virtual CREF(LPRDOParserContainer) getContainer() const = 0;

	RDOParserContainer::Iterator begin()
	{
		return getContainer()->begin();
	}
	RDOParserContainer::Iterator end()
	{
		return getContainer()->end();
	}
	RDOParserContainer::Iterator find(ruint index)
	{
		return getContainer()->find(index);
	}

	typedef std::vector<PTR(RDOValue)> ValueList;

	ValueList                m_allValues;
	rdoRuntime::LPRDORuntime m_pRuntime;

	void parse(rdoModelObjects::RDOParseType file);

private:
	LPRDOSMR              m_pSMR;
	rbool                 m_have_kw_Resources;
	rbool                 m_have_kw_ResourcesEnd;
	Error                 m_error;
	Stack                 m_movementObjectList;
	PreCastTypeList       m_preCastTypeList;
	LPContextStack        m_pContextStack;
	rbool                 m_pattern;
	rdo::IDGenerator      m_resultGeneratorID;

	template <class T>
	void howIsIt()
	{
		m_pattern = false;
	}

	template <>
	void howIsIt<LPRDOFUNGroup>()
	{}

	template <>
	void howIsIt<LPRDOPATPattern>()
	{
		m_pattern = true;
	}

	struct Changes
	{
		tstring m_name;
		tstring m_value;
		Changes(CREF(tstring) name, CREF(tstring) value)
			: m_name (name )
			, m_value(value)
		{}
	};
	typedef std::vector<Changes> ChangesList;
	ChangesList m_changes;

	typedef std::list<LPRDOParser> ParserList;
	static ParserList s_parserStack;

	DECLARE_IContextFind;
	DECLARE_IContextCreateExpression;
};
DECLARE_POINTER(RDOParser);

// ----------------------------------------------------------------------------
// ---------- RDOParserTemplate
// ----------------------------------------------------------------------------
template <class Container>
class RDOParserTemplate: public RDOParser
{
DECLARE_FACTORY(RDOParserTemplate<Container>);

private:
	typedef  RDOParser  parent_type;

	RDOParserTemplate()
		: RDOParser()
	{}
	virtual ~RDOParserTemplate()
	{}

	virtual void init()
	{
		m_pContainer = rdo::Factory<Container>::create();
		ASSERT(m_pContainer);
		parent_type::init();
	}

	virtual void deinit()
	{
		ASSERT(m_pContainer)
		m_pContainer->clear();
		parent_type::deinit();
	}

	virtual CREF(LPRDOParserContainer) getContainer() const
	{
		return m_pContainer;
	}

	LPRDOParserContainer m_pContainer;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserModel
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerModel> RDOParserModel;

// ----------------------------------------------------------------------------
// ---------- RDOParserSMRInfo
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerSMRInfo> RDOParserSMRInfo;

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbar
// ----------------------------------------------------------------------------
typedef RDOParserTemplate<RDOParserContainerCorba> RDOParserCorba;

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_H_
