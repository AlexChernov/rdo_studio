/*
  \copyright (c) RDO-Team, 2011
  \file      context_find_i.h
  \author    ������ ������
  \date      03.03.2011
  \brief     
  \indent    4T
 */

#ifndef _RDOPARSER_CONTEXT_FIND_I_H_
#define _RDOPARSER_CONTEXT_FIND_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextFind
// ----------------------------------------------------------------------------
OBJECT_INTERFACE(IContextFind)
{
DECLARE_FACTORY(IContextFind)
public:
	virtual LPContext onFindContext(CREF(RDOValue) value) const = 0;

protected:
	IContextFind()
	{}
	virtual ~IContextFind()
	{}
};
#define DECLARE_IContextFind \
	LPContext onFindContext(CREF(RDOValue) value) const;

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_FIND_I_H_
