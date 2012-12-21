/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/type/runtime_wrapper_type.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      06.12.2012
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_
#define _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/type.h"
#include "simulator/runtime/headers/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RuntimeWrapperType: public RDOType
{
DECLARE_FACTORY(RuntimeWrapperType)
public:
	virtual CREF(rdo::runtime::LPRDOType)    type() const;
	virtual rdo::runtime::RDOType::TypeID  typeID() const;

	virtual rdo::runtime::LPRDOCalc calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;

protected:
	RuntimeWrapperType();
	RuntimeWrapperType(CREF(rdo::runtime::LPRDOType) pType);
	virtual ~RuntimeWrapperType();

	virtual int selfInsertToDB(REF(InterfaceDB) db) const;

	rdo::runtime::LPRDOType m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_RUNTIME_WRAPPER_TYPE_H_
