/*
  \copyright (c) RDO-Team, 2011
  \file      rdorss.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
 */

#ifndef _CONVERTOR_RDORSS_RSS_H_
#define _CONVERTOR_RDORSS_RSS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_converter/rdortp.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_NAMESPACE

int  cnv_rssparse(PTR(void)    lexer);
int  cnv_rsslex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_rsserror(PTR(char)    message);

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
OBJECT(RDORSSResource) IS INSTANCE_OF(RDOParserSrcInfo)
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
	RDORSSResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);

	LPRDORTPResType m_pResType;
	const int       m_id;        //! in system
	ParamList       m_paramList;
	rbool           trace;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
DECLARE_FACTORY(RDOPROCResource);
private:
	RDOPROCResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual rdoRuntime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCResource);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDORSS_RSS_H_
