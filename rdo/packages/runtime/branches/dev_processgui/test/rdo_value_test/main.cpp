/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������ (rdo@rk9.bmstu.ru)
 * date     : 13.07.2009
 * bref     : ���� ��� RDOValue
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	RDOValue value1("qqq");
	RDOValue value2 = value1;
	value2 += "aaa";
	int i = 1;
}
