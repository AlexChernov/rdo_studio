/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

REF(PTR(void)) RDOValue::__voidPtrV()
{
	return __get<PTR(void)>();
}

CREF(PTR(void)) RDOValue::__voidPtrV() const
{
	return *reinterpret_cast<CPTR(PTR(void))>(&const_cast<PTR(RDOValue)>(this)->m_value);
}

REF(PTR(RDOValue::smart_string)) RDOValue::__stringPtrV()
{
	return __get<PTR(RDOValue::smart_string)>();
}

CREF(PTR(RDOValue::smart_string)) RDOValue::__stringPtrV() const
{
	return *reinterpret_cast<CPTR(PTR(smart_string))>(&const_cast<PTR(RDOValue)>(this)->m_value);
}

CLOSE_RDO_RUNTIME_NAMESPACE
