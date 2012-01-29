/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_logic.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_LOGIC_H_
#define _RDOPARSER_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/rdo_logic_i.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogic
// --------------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogic: public RDOParserSrcInfo
{
public:
	typedef  rdo::intrusive_ptr<Activity> LPActivity;
	typedef  std::vector<LPActivity>      ActivityList;

	RDOLogic(CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
	{}
	virtual ~RDOLogic()
	{}

	CREF(tstring) name() const { return src_info().src_text(); }

	LPActivity addNewActivity(CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	{
		LPActivity pAactivity = rdo::Factory<Activity>::create(m_pRuntimeLogic, activity_src_info, pattern_src_info);
		ASSERT(pAactivity);
		m_activityList.push_back(pAactivity);
		return pAactivity;
	}

	LPActivity getLastActivity() const
	{
		return !m_activityList.empty() ? m_activityList.back() : LPActivity(NULL);
	}
	CREF(ActivityList) getActivities() const { return m_activityList; }

	rbool setPrior(REF(LPRDOFUNArithm) pPrior)
	{
		LPIPriority pPriority = m_pRuntimeLogic;
		if (pPriority)
		{
			return pPriority->setPrior(pPrior->createCalc());
		}
		return false;
	}

	rbool getMultithreading () const
	{
		return m_Multithreading;
	}

	void setMultithreading (rbool Multithreading = false)
	{
		m_Multithreading = Multithreading;
	}


protected:
	LPILogic     m_pRuntimeLogic;
	rbool        m_Multithreading;

private:
	ActivityList m_activityList;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_LOGIC_H_
