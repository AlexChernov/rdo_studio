/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_H_
#define _RDOPARSER_H_

// ====================================================================== INCLUDES
#include <algorithm>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdosingletone.h"
#include "rdo_common/rdoindexedstack.h"
#include "rdo_common/rdosmart_ptr_wrapper.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdoparser_base.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDORuntime;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

class RDORTPParam;
class RDORTPResType;
class RDORSSResource;
class RDOPATPattern;
class RDOOPROperation;
class RDOFRMFrame;
class RDOFUNFunction;
class RDOFUNSequence;
class RDOFUNConstant;
class RDOFUNGroup;
class RDOSMR;
class RDOOperations;
class RDODPTSearch;
class RDODPTSome;
class RDODPTPrior;
class RDODPTFree;
class RDODPTFreeActivity;
class RDOPMDPokaz;
class RDOPROCProcess;
class RDOTypeParam;
//class RDOEvent;

class RDORTPFuzzyParam;

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
#define DEFINE_OBJECT_CONTAINER_NONAME(Name) \
public: \
	typedef std::vector<PTR(RDO##Name)> Name##List; \
	void                insert##Name (PTR(RDO##Name) value); \
	PTR(RDO##Name)      getLast##Name()       { return !m_all##Name.empty() ? m_all##Name.back() : NULL; } \
	CREF(Name##List)    get##Name##s () const { return m_all##Name; } \
private: \
	Name##List m_all##Name;

#define DEFINE_OBJECT_CONTAINER(Name) \
DEFINE_OBJECT_CONTAINER_NONAME(Name) \
public: \
	CPTR(RDO##Name) find##Name  (CREF(tstring) name) const; \
	rbool           remove##Name(CPTR(RDO##Name) item);


class RDOParser
{
public:
DEFINE_OBJECT_CONTAINER(PATPattern     );
DEFINE_OBJECT_CONTAINER(RTPResType     );
DEFINE_OBJECT_CONTAINER(RSSResource    );
DEFINE_OBJECT_CONTAINER(OPROperation   );
DEFINE_OBJECT_CONTAINER(FRMFrame       );
DEFINE_OBJECT_CONTAINER(FUNConstant    );
DEFINE_OBJECT_CONTAINER(FUNFunction    );
DEFINE_OBJECT_CONTAINER(FUNSequence    );
DEFINE_OBJECT_CONTAINER(DPTSearch      );
DEFINE_OBJECT_CONTAINER(DPTSome        );
DEFINE_OBJECT_CONTAINER(DPTPrior       );
DEFINE_OBJECT_CONTAINER(DPTFreeActivity);
DEFINE_OBJECT_CONTAINER(PMDPokaz       );
//DEFINE_OBJECT_CONTAINER(Event          );

DEFINE_OBJECT_CONTAINER_NONAME(FUNGroup   );
DEFINE_OBJECT_CONTAINER_NONAME(DPTFree    );
DEFINE_OBJECT_CONTAINER_NONAME(PROCProcess);
DEFINE_OBJECT_CONTAINER_NONAME(Operations );

public:
	RDOParser();
	virtual ~RDOParser();

	PTR(rdoRuntime::RDORuntime) runtime() { return &m_runtime; }

	void insertDeletables(PTR(RDODeletable) value)
	{
		ASSERT(value);
		m_allDeletables.push_back(value);
	}
	void removeDeletables(PTR(RDODeletable) value)
	{
		ASSERT(value);
		m_allDeletables.erase(std::find(m_allDeletables.begin(), m_allDeletables.end(), value));
	}

	PTR(RDOParserObject) getLastParsingObject() { return m_parsing_object; }
	REF(FUNGroupList)    getFUNGroupStack    () { return m_allFUNGroup;    }

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
	ruint getPMD_id     () const { return m_allPMDPokaz.size()    + 1; }
	ruint getFUNCONST_id() const { return m_allFUNConstant.size() + 0; }

	tstring getModelStructure();
	tstring getChanges       () const;

	PTR(RDOSMR) getSMR()                const { return m_smr;                }
	void        setSMR(PTR(RDOSMR) smr)       { m_smr = smr;                 }
	rbool       hasSMR()                const { return m_smr ? true : false; }

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
		IndexedStack::ID push(CREF(rdo::smart_ptr<T>) pObject)
		{
			rdo::LPISmartPtrWrapper pWrapper = new rdo::smart_ptr_wrapper<T>(pObject);
			return IndexedStack::push(pWrapper);
		}
		template <class T>
		rdo::smart_ptr<T> pop(IndexedStack::ID id)
		{
			rdo::LPISmartPtrWrapper pWrapper = IndexedStack::pop(id);
			ASSERT(pWrapper);
			rdo::smart_ptr<T> pObject = *reinterpret_cast<PTR(rdo::smart_ptr<T>)>(pWrapper->getSmartPtr());
			pWrapper->destroy();
			return pObject;
		}
	private:
		void clear()
		{
			STL_FOR_ALL(IndexedStack::Stack, m_stack, it)
			{
				it->second->destroy();
			}
		}
	};

	REF(Stack) stack()
	{
		return m_movementObjectList;
	}

	typedef std::vector<LPRDOTypeParam> PreCastTypeList;
	CREF(PreCastTypeList) getPreCastTypeList() const
	{
		return m_preCastTypeList;
	}
	void insertPreCastType(CREF(LPRDOTypeParam) type)
	{
		m_preCastTypeList.push_back(type);
	}

	static rdoModelObjects::RDOFileType getFileToParse();
	static ruint                        lexer_loc_line();
	static ruint                        lexer_loc_pos ();
	static PTR(RDOParser)               s_parser      ();

protected:
	PTR(RDOParserItem) m_parser_item;

	virtual PTR(RDOParserContainer) getContainer() = 0;

	RDOParserContainer::CIterator begin()
	{
		return getContainer()->begin();
	}
	RDOParserContainer::CIterator end()
	{
		return getContainer()->end();
	}
	RDOParserContainer::CIterator find(ruint index)
	{
		return getContainer()->find(index);
	}

	typedef std::vector<PTR(RDODeletable)> DeletableList;
	typedef std::vector<PTR(RDOValue)>     ValueList;

	DeletableList          m_allDeletables;
	ValueList              m_allValues;
	rdoRuntime::RDORuntime m_runtime;

	void parse(rdoModelObjects::RDOParseType file);

private:
	PTR(RDOParserObject)  m_parsing_object;
	PTR(RDOSMR)           m_smr;
	rbool                 m_have_kw_Resources;
	rbool                 m_have_kw_ResourcesEnd;
	Error                 m_error;
	Stack                 m_movementObjectList;
	PreCastTypeList       m_preCastTypeList;

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

	typedef std::list<PTR(RDOParser)> ParserList;
	static ParserList s_parserStack;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserTemplate
// ----------------------------------------------------------------------------
template <class Container>
class RDOParserTemplate: public RDOParser
{
public:
	RDOParserTemplate()
		: RDOParser()
	{}

private:
	rdo::SingleTone<Container> m_container;

	virtual PTR(RDOParserContainer) getContainer()
	{
		return m_container.instance(this);
	}
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
