/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ENUM_H_
#define _RDOPARSER_TYPE_ENUM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/compiler/parser/type/runtime_wrapper_type.h"
#include "simulator/compiler/parser/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOEnumType);

class RDOEnumType: public RuntimeWrapperType
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(CREF(LPRDOValue) pNext);

	rdo::runtime::LPRDOEnumType getEnums() const;

	rbool operator== (CREF(RDOEnumType) pEnumType) const;
	rbool operator!= (CREF(RDOEnumType) pEnumType) const;

#ifdef SERIALIZE_IN_DB_MAINSTREAM
	virtual void serializeInDB(REF(IDB) db) const;
#endif

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOEnumType         ();
	RDOEnumType         (CREF(rdo::runtime::LPRDOEnumType) pEnumType);
	virtual ~RDOEnumType();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ENUM_H_
