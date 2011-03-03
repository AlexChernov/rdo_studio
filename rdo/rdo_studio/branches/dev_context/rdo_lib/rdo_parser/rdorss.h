/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdorss.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDORSS_RSS_H_
#define _RDORSS_RSS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/context/context_find_i.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSER_NAMESPACE

int  rssparse(PTR(void)    lexer);
int  rsslex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rsserror(PTR(char)    message);

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
CLASS(RDORSSResource):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDORSSResource);
public:
	class Param
	{
	public:
		explicit Param(CREF(RDOValue) value)
			: m_value(value)
		{}

		CREF(RDOValue) param() const
		{
			return m_value;
		}

	private:
		RDOValue m_value;
	};
	typedef std::vector<Param> ParamList;
	enum { UNDEFINED_ID = ~0 };

	virtual rdoRuntime::LPRDOCalc createCalc() const;

	CREF(tstring)    name   () const { return src_info().src_text(); }
	LPRDORTPResType  getType() const { return m_pResType;            }

	int              getID  () const { return m_id;                  }

	CREF(ParamList)  params () const { return m_paramList;           }

	void  addParam(CREF(RDOValue) param);
	rbool getTrace() const      { return trace;  }
	void  setTrace(rbool value) { trace = value; }
	rbool defined () const;

	void writeModelStructure(REF(std::ostream) stream) const;

protected:
	RDORSSResource(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual ~RDORSSResource();

	LPRDORTPResType m_pResType;
	const int       m_id;        //! in system
	ParamList       m_paramList;
	rbool           trace;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDORSSResource);

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
DECLARE_FACTORY(RDOPROCResource);
private:
	RDOPROCResource(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual ~RDOPROCResource();

	virtual rdoRuntime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCResource);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORSS_RSS_H_
