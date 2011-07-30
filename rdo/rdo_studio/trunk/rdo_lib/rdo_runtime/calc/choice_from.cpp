/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      choice_from.cpp
 * @authors   ���� ��������, ������ ������, ����� �������
 * @date      unknown
 * @brief     RDOCalc ��� ������� ����������� �������� � �������� ��������
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
#include <limits>
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/choice_from.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOSelectResourceNonExistCalc
// ********************************************************************************
REF(RDOValue) RDOSelectResourceNonExistCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(rel_res_id, -1);
	return m_value;
}

// ********************************************************************************
// ******************** RDOCalcCreateNumberedResource
// ********************************************************************************
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: m_pType    (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{}

REF(RDOValue) RDOCalcCreateNumberedResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// ********************************************************************************
// ******************** RDOCalcCreateProcessResource
// ********************************************************************************
RDOCalcCreateProcessResource::RDOCalcCreateProcessResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: m_pType    (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{}

REF(RDOValue) RDOCalcCreateProcessResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	NEVER_REACH_HERE;
	return m_value;
}

// ********************************************************************************
// ******************** RDOCalcCreateResource
// ********************************************************************************
RDOCalcCreateResource::RDOCalcCreateResource(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID)
	: m_pResType     (pType        )
	, m_traceFlag    (traceFlag    )
	, m_permanentFlag(permanentFlag)
	, m_relResID     (relResID     )
{
	m_paramsCalcs.insert(m_paramsCalcs.begin(), rParamsCalcs.begin(), rParamsCalcs.end());
	/// @todo ���������� �� ASSERT
	if (m_permanentFlag && m_relResID > 0) NEVER_REACH_HERE; //������� ��������� ���������� ������� �����������
}

REF(RDOValue) RDOCalcCreateResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_pResType->createRes(pRuntime, m_paramsCalcs, m_traceFlag, m_permanentFlag);
	if (m_relResID)
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, pResource->getTraceID());
	return m_value; // just to return something
}

// ********************************************************************************
// ******************** RDOSelectResourceCalc
// ********************************************************************************
RDOSelectResourceCalc::RDOSelectResourceCalc(int _rel_res_id, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type)
	: rel_res_id (_rel_res_id )
	, choice_calc(_choice_calc)
	, order_calc (_order_calc )
	, order_type (_order_type )
{}

// ********************************************************************************
// ******************** RDOSelectResourceDirectCalc
// ********************************************************************************
REF(RDOValue) RDOSelectResourceDirectCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
	if (choice_calc && !choice_calc->calcValue(pRuntime).getAsBool())
	{
		pRuntime->getCurrentActivity()->setRelRes(rel_res_id, -1);
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

// ********************************************************************************
// ******************** RDOSelectResourceByTypeCalc
// ********************************************************************************
REF(RDOValue) RDOSelectResourceByTypeCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it && (*it)->checkType(resType))
		{
			int res_id = (*it)->getTraceID();

			switch (order_type)
			{
				case order_empty:
				case order_first:
				{
					pRuntime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					m_value = 1;
					return m_value;
				}
				case order_with_min:
				{
					pRuntime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(pRuntime);
					if (tmp < minVal)
					{
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case order_with_max:
				{
					pRuntime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(pRuntime);
					if (tmp > maxVal)
					{
						maxVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
			}
		}
	}

	if (res_minmax_id != -1)
	{
		pRuntime->getCurrentActivity()->setRelRes(rel_res_id, res_minmax_id);
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

// ********************************************************************************
// ******************** RDOSelectResourceCommonCalc
// ********************************************************************************
void RDOSelectResourceCommonCalc::getBest(REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(pRuntime))
			{
				return; // state not valid
			}
		}
		RDOValue newVal = const_cast<PTR(RDOSelectResourceCommonCalc)>(this)->choice_calc->calcValue(pRuntime);
		if (!hasBest || (useCommonWithMax && (newVal > bestVal)) ||
		   (!useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for (ruint i = 0; i < resSelectors.size(); i++)
			{
				res.at(i) = pRuntime->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, pRuntime, hasBest);
	}
}

rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(pRuntime))
			{
				return false;
			}
		}
		return true;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		if (getFirst(allNumbs, level+1, pRuntime)) return true;
	}
	return false;
}

//rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, int level,CREF(LPRDORuntime) pRuntime) const
//{
//	if (level <= 0) {
//		for (int i = 0; i < resSelectors.size(); i++) {
//			if (!resSelectors.at(i)->callChoice(pRuntime)) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		REF(std::vector<int>) ourLevel = allNumbs.at(level);
//		for (int i = 0; i < ourLevel.size(); i++) {
//			pRuntime->setRelRes(level, ourLevel.at(i));
//			if (getFirst(allNumbs, level, pRuntime)) return true;
//		}
//	}
//	return false;
//}

REF(RDOValue) RDOSelectResourceCommonCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	std::vector< std::vector<int> > allNumbs;
	std::vector<int> res;
	for (ruint i = 0; i < resSelectors.size(); i++)
	{
		allNumbs.push_back(resSelectors.at(i)->getPossibleNumbers(pRuntime));
		res.push_back(pRuntime->getCurrentActivity()->getResByRelRes(i));
	}
	if (!choice_calc)
	{
		// first
//		if (getFirst(allNumbs, allNumbs.size(), pRuntime)) {
//			return true;
//		}
		if (getFirst(allNumbs, 0, pRuntime))
		{
			m_value = 1;
			return m_value;
		}
	}
	else
	{
		// with_min / with_max
		RDOValue bestVal = 0;
		rbool found = false;
		getBest(allNumbs, 0, res, bestVal, pRuntime, found);
		if (found)
		{
			for (ruint i = 0; i < res.size(); i++)
			{
				pRuntime->getCurrentActivity()->setRelRes(i, res.at(i));
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
}

// ********************************************************************************
// ******************** RDOSelectResourceDirectCommonCalc
// ********************************************************************************
std::vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const
{
	std::vector<int> res;	
	res.push_back(res_id);
	return res;
}

rbool RDOSelectResourceDirectCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	return (choice_calc && !const_cast<PTR(RDOSelectResourceDirectCommonCalc)>(this)->choice_calc->calcValue(pRuntime).getAsBool()) ? false : true;
}

RDOSelectResourceDirectCommonCalc::~RDOSelectResourceDirectCommonCalc()
{}

// ********************************************************************************
// ******************** RDOSelectResourceByTypeCommonCalc
// ********************************************************************************
std::vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const
{
	std::vector<int> res;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(resType))
			continue;

		res.push_back((*it)->getTraceID());
	}
	return res;
}

rbool RDOSelectResourceByTypeCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	return (choice_calc && !const_cast<PTR(RDOSelectResourceByTypeCommonCalc)>(this)->choice_calc->calcValue(pRuntime).getAsBool()) ? false : true;
}

RDOSelectResourceByTypeCommonCalc::~RDOSelectResourceByTypeCommonCalc()
{}

// ********************************************************************************
// ******************** IRDOSelectResourceCommon
// ********************************************************************************
IRDOSelectResourceCommon::IRDOSelectResourceCommon()
{}

IRDOSelectResourceCommon::~IRDOSelectResourceCommon()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
