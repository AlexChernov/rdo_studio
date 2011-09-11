/*!
  \copyright (c) RDO-Team, 2011
  \file      param.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

#ifndef _PARSER_PARAM_H_
#define _PARSER_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParam
// --------------------------------------------------------------------------------
OBJECT(RDOParam) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOParam)
public:
	CREF(tstring)  name       () const { return src_info().src_text(); }
	LPTypeInfo     getTypeInfo() const { return m_pType;               }
	CREF(RDOValue) getDefault () const { return m_default;             }

protected:
	RDOParam(CREF(tstring)          name,     CREF(LPTypeInfo) pType, RDOValue default = RDOValue());
	RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType, RDOValue default = RDOValue());
	virtual ~RDOParam();

private:
	LPTypeInfo m_pType;
	RDOValue   m_default;

	void checkDefault();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _PARSER_PARAM_H_
