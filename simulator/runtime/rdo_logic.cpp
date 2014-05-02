/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_logic.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      23.04.2008
  \brief     Логика - контейнер БЗ
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/// @todo не стоит ли здесь перейти на функторы?

// --------------------------------------------------------------------------------
// -------------------- RDOOrderSimple
// --------------------------------------------------------------------------------
LPIBaseOperation RDOOrderSimple::sort(const LPRDORuntime& pRuntime, BaseOperationList& container)
{
	for (auto& operation: container)
	{
		if (operation->onCheckCondition(pRuntime))
		{
			return operation;
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOOrderMeta
// --------------------------------------------------------------------------------
LPIBaseOperation RDOOrderMeta::sort(const LPRDORuntime& pRuntime, BaseOperationList& container)
{
	if (container.empty())
		return NULL;

	for (const auto& operation: container)
	{
		LPIPriority pPattern = operation;
		if (pPattern)
		{
			LPRDOCalc pPriorCalc = pPattern->getPrior();
			if (pPriorCalc)
			{
				RDOValue value = pPriorCalc->calcValue(pRuntime);
				if (value < 0.0 || value > 1.0)
				{
					pRuntime->error().push(rdo::format("Приоритет активности вышел за пределы диапазона [0..1]: %s", value.getAsString().c_str()), pPriorCalc->srcInfo());
				}
			}
		}
	}
	std::sort(container.begin(), container.end(), RDODPTActivityCompare(pRuntime));
	for (auto& operation: container)
	{
		if (operation->onCheckCondition(pRuntime))
		{
			return operation;
		}
	}
	return NULL;
}

CLOSE_RDO_RUNTIME_NAMESPACE
