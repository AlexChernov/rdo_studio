/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.07.2009
  \brief     ���� ��� RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

using namespace rdoRuntime;

void main()
{
	RDOValue value1("qqq");
	RDOValue value2 = value1;
	value2 += "aaa";
	int i = 1;
}
