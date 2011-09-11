/*!
  \copyright (c) RDO-Team, 2011
  \file      counter_reference.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      12.06.2010
  \brief     
  \indent    4T
*/

#ifndef _LIB_COMMON_SMART_PTR_COUNTER_REFERENCE_H_
#define _LIB_COMMON_SMART_PTR_COUNTER_REFERENCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class counter_reference
{
template<class T> friend class intrusive_ptr;
template<class T> friend class CounterReferenceReal;
public:
	//! �������������� ������� ����
	counter_reference();

private:
	ruint m_intrusive_counter;
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/smart_ptr/counter_reference.inl"

#endif // _LIB_COMMON_SMART_PTR_COUNTER_REFERENCE_H_
