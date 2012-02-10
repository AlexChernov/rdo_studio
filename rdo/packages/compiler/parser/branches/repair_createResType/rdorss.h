/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDORSS_RSS_H_
#define _RDORSS_RSS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/context/context_switch_i.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_PARSE_NAMESPACE

int  rssparse(PTR(void)    lexer);
int  rsslex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rsserror(PTR(char)    message);

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
CLASS(RDORSSResource):
	    INSTANCE_OF      (RDOParserSrcInfo  )
	AND INSTANCE_OF      (Context           )
	AND INSTANCE_OF      (boost::noncopyable)
	AND IMPLEMENTATION_OF(IContextSwitch    )
{
DECLARE_FACTORY(RDORSSResource);
public:
	class Param
	{
	public:
		explicit Param(CREF(LPRDOValue) pValue)
			: m_pValue(pValue)
		{}

		CREF(LPRDOValue) param() const
		{
			return m_pValue;
		}

	private:
		//! \todo ������������ RDOCalc ������ RDOValue
		LPRDOValue m_pValue;
	};
	typedef std::vector<Param> ParamList;
	enum { UNDEFINED_ID = ~0 };

	virtual rdoRuntime::LPRDOCalc createCalc() const;

	CREF(tstring)    name   () const { return src_info().src_text(); }
	LPRDORTPResType  getType() const { return m_pResType;            }

	ruint            getID  () const { return m_id;                  }

	CREF(ParamList)  params () const { return m_paramList;           }

	void  addParam(CREF(LPRDOValue) pParam);
	rbool getTrace() const      { return trace;  }
	void  setTrace(rbool value) { trace = value; }
	rbool defined () const;
	void  end     ();

	void writeModelStructure(REF(std::ostream) stream) const;

protected:
	RDORSSResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, ruint id = UNDEFINED_ID);
	virtual ~RDORSSResource();

	LPRDORTPResType m_pResType;
	ruint           m_id;        //! in system
	ParamList       m_paramList;
	rbool           trace;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;

	DECLARE_IContextSwitch;
};
DECLARE_POINTER(RDORSSResource);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
DECLARE_FACTORY(RDOPROCResource);
private:
	RDOPROCResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual ~RDOPROCResource();

	virtual rdoRuntime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCResource);

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
class RDOPROCTransact: public RDORSSResource
{
	DECLARE_FACTORY(RDOPROCTransact);
private:
	RDOPROCTransact(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual ~RDOPROCTransact();

	virtual rdoRuntime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCTransact);

CLOSE_RDO_PARSE_NAMESPACE

#endif // _RDORSS_RSS_H_
