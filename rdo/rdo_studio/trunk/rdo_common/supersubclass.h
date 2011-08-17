/*
  \copyright (c) RDO-Team, 2011
  \file      supersubclass.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.03.2010
  \brief     ������� �� ������ Conversion �� �������������
  \indent    4T
 */

#ifndef _SUPERSUBCLASS_H_
#define _SUPERSUBCLASS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template <class T, class U>
class SuperSubClass
{
private:
	typedef char Small;
	class        Big    { char dummy[2]; };

	static Small Test (CREF(U));
	static Big   Test (...);
	static T     MakeT();

public:
	enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
	enum { same   = false };
};

template <class T>
class SuperSubClass<T, T>
{
public:
	enum { exists = true };
	enum { same   = true };
};

#define SUPERSUBCLASS(T, U) (SuperSubClass<CPTR(U), CPTR(T)>::exists)

CLOSE_RDO_NAMESPACE

#endif //! _SUPERSUBCLASS_H_
