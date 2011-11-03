/*!
  \copyright (c) RDO-Team, 2011
  \file      context_find_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      03.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_FIND_I_H_
#define _RDOPARSER_CONTEXT_FIND_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextFind
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextFind)
{
DECLARE_FACTORY(IContextFind)
public:
	virtual LPContext onFindContext(CREF(LPRDOValue) pValue) const = 0;

protected:
	IContextFind()
	{}
	virtual ~IContextFind()
	{}
};
#define DECLARE_IContextFind \
	LPContext onFindContext(CREF(LPRDOValue) pValue) const;

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_CONTEXT_FIND_I_H_
