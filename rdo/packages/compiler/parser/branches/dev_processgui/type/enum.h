/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ENUM_H_
#define _RDOPARSER_TYPE_ENUM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/type.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/runtime/rdo_enum.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOEnumType);

class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(CREF(RDOValue) next);

	rdoRuntime::LPRDOEnumType getEnums() const
	{
		return m_pType.object_static_cast<rdoRuntime::RDOEnumType>();
	}

	rbool operator== (CREF(RDOEnumType) pEnumType) const
	{
		return getEnums()->getValues() == pEnumType.getEnums()->getValues();
	}
	rbool operator!= (CREF(RDOEnumType) pEnumType) const
	{
		return !operator==(pEnumType);
	}

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOEnumType         ();
	RDOEnumType         (CREF(rdoRuntime::LPRDOEnumType) pEnumType);
	virtual ~RDOEnumType();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ENUM_H_
