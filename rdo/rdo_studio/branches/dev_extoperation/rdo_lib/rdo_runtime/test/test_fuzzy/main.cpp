/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������
 * date     : 10.05.2009
 * bref     : ���� �������� ������
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_fuzzy.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	RDORuntime runtime;
	PTR(RDOFuzzySetDefinitionRangeDiscret) setDefinition1 = new RDOFuzzySetDefinitionRangeDiscret(&runtime, 0, 50);
	RDOFuzzyType type1(setDefinition1);

	RDOFuzzyValue value1(type1);
	value1.append(1, 1.0)(2, 1.0)(23, 1.0)(4, 1.0)(5, 1.0)(6, 1.0)(7, 1.0)(8, 1.0)(9, 1.0);
	std::cout << "value1: " << value1.getAsString() << std::endl;
	value1[0] = 0.5;
	std::cout << "value1: " << value1.getAsString() << std::endl;
	value1[14] = 0.7;
	std::cout << "value1: " << value1.getAsString() << std::endl;

	RDOFuzzyValue value3(value1.supplement());
	std::cout << "value3: " << value3.getAsString() << std::endl;

	RDOFuzzyValue value2(type1);
	value2.append(10, 1.0);
	std::cout << "value2: " << value2.getAsString() << std::endl;

	value1 = value1 + value2;

	std::cout << value1.getAsString() << std::endl;
	std::cout << value1.defuzzyfication().getAsString() << std::endl;

	int i = 1;
}
