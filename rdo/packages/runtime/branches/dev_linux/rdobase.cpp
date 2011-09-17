/*!
  \copyright (c) RDO-Team, 2011
  \file      rdobase.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ���������� �����������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdobase.h"
#include "simulator/runtime/calc/operation_type.h"
// --------------------------------------------------------------------------------

#pragma warning(disable : 4786)

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulatorBase
// --------------------------------------------------------------------------------
RDOSimulatorBase::RDOSimulatorBase()
	: m_startTime         (0                       )
	, m_currentTime       (0                       )
	, m_nextTime          (0                       )
	, m_mode              (rdoRuntime::RTM_MaxSpeed)
	, m_speed             (1                       )
	, m_speed_range_max   (500000                  )
	, m_next_delay_count  (0                       )
	, m_next_delay_current(0                       )
	, m_showRate          (60                      )
	, m_msec_wait         (0                       )
	, m_msec_prev         (0                       )
	, m_cnt_events        (0                       )
	, m_cnt_choice_from   (0                       )
	, m_checkOperation    (true                    )
{}

ruint RDOSimulatorBase::get_cnt_calc_arithm() const
{
	return OperatorType::getCalcCounter<OperatorType::OT_ARITHM>();
}

ruint RDOSimulatorBase::get_cnt_calc_logic() const
{
	return OperatorType::getCalcCounter<OperatorType::OT_LOGIC>();
}

void RDOSimulatorBase::rdoInit()
{
	m_currentTime    = m_startTime;
	m_nextTime       = m_currentTime;
	m_checkOperation = true;
	onInit();
	OperatorType::getCalcCounter<OperatorType::OT_ARITHM>() = 0;
	OperatorType::getCalcCounter<OperatorType::OT_LOGIC> () = 0;

	if (m_timePoints.find(m_currentTime) == m_timePoints.end())
		m_timePoints[m_currentTime] = NULL;
	preProcess();

	m_speed              = 1;
	m_next_delay_count   = 0;
	m_next_delay_current = 0;
	m_showRate           = 60;
	m_msec_wait          = 0;

	onNewTimeNow();
}

rbool RDOSimulatorBase::rdoNext()
{
	if (m_mode == rdoRuntime::RTM_Pause || m_mode == rdoRuntime::RTM_BreakPoint)
	{
		::Sleep(1);
		return true;
	}
	// ���� ������ ������� ��� �������� �������, �� �������� ���� ����������
	rbool keyboard = isKeyDown();
	if (!keyboard)
	{
		// �������� ����� �������� �������������
		// ��� mode == rdoRuntime::RTM_Jump || mode == rdoRuntime::RTM_Sync
		if (m_mode != rdoRuntime::RTM_MaxSpeed && m_next_delay_count)
		{
			++m_next_delay_current;
			if (m_next_delay_current < m_next_delay_count) return true;
			m_next_delay_current = 0;
		}
		// �������� ���������� �������� ������������� (������������ ��������)
		// ��� �� ���� ��������� mode == rdoRuntime::RTM_Sync, �.�. ��� ��� �������� � msec_wait,
		// ������� ������������ � setMode � �� ���������� �����.
		if (m_msec_wait > 1)
		{
			boost::posix_time::ptime systime_current = boost::posix_time::microsec_clock::local_time();
			ruint msec_curr = getMSec(systime_current);
			ruint msec_delta;
			// ����������� ��������� � ������ �����, �� ��� ����� ����� ���� ������������ �� ����,
			// � ������� ����� � ������������ ���������� ������ �����������. ��������� ���� ������
			// ����� ����� ELSE. ������ ������� ������ ������ ������� �� ���� ���� ������ �����
			// ����� ��������� ��������� �������, �� �� ������ ������ �� ��� � ����� ���. ���
			// �������� ��������������� �������� (����� ���������� ������� ��� ���), �� ��������
			// ����� ������������� � ������� ����������� ��������� �������, ������� �������� �
			// �������������� ������, ����������� ���������� � ������������. ��� ��� ������ �� �����.
			// ��� ������� - ����� �� ���������� ������ � ����� ��������, � ���� ������ ��������, ���
			// ������� �� ������ � ���������� ����, ������� � ���. SYSTEMTIME �������� ����� ����������.
			if (msec_curr >= m_msec_prev)
			{
				msec_delta = msec_curr - m_msec_prev;
			}
			else
			{
				msec_delta = UINT_MAX - m_msec_prev + msec_curr;
			}
			if (msec_delta <= m_msec_wait) return true;
			m_msec_wait -= msec_delta;
		}
	}
	// ��������� ������������� - ��������� ������� �����
	if (endCondition())
	{
		onEndCondition();
		return false;
	}
	if (m_currentTime != m_nextTime)
	{
		m_currentTime = m_nextTime;
		onNewTimeNow();
	}
	// ���������� ��������
	if (doOperation())
	{
		if (breakPoints())
		{
			setMode(RTM_BreakPoint);
		}
		return true;
	}
	else
	{
		// ������� � ��������� ��������
		if (!m_timePoints.empty())
		{
			double newTime = m_timePoints.begin()->first;
			BOPlannedItem* list = m_timePoints.begin()->second;
			if (!list || list->empty())
			{
				delete m_timePoints.begin()->second;
				m_timePoints.erase(m_timePoints.begin());
			}
			if (m_currentTime > newTime)
			{
				newTime = m_currentTime;
			}
			if (m_mode == rdoRuntime::RTM_Sync)
			{
				m_msec_wait += (newTime - m_nextTime) * 3600.0 * 1000.0 / m_showRate;
				if (m_msec_wait > 0)
				{
					if (newTime != m_startTime)
					{
						if (m_speed > DBL_MIN)
						{
							m_msec_wait = m_msec_wait / m_speed;
						}
						else
						{
							m_msec_wait = m_msec_wait / DBL_MIN;
						}
						boost::posix_time::ptime systime_current = boost::posix_time::microsec_clock::local_time();
						m_msec_prev = getMSec(systime_current);
					}
					else
					{
						m_msec_wait = 0;
					}
				}
			}
			m_nextTime = newTime;
			return true;
		}
		else
		{
			// ��������� ������������� - ��� ������ �������
			onNothingMoreToDo();
			return false;
		}
	}
	return true;
}

void RDOSimulatorBase::setMode(rdoRuntime::RunTimeMode _mode)
{
	if (m_mode == rdoRuntime::RTM_Pause)
	{
		// ����� ����� ������� � ��������� �������� ����� ����� � ����� '�� ������'
		m_next_delay_current = m_next_delay_count;
		m_msec_wait          = 0;
	}
	m_mode = _mode;
	if (m_mode == rdoRuntime::RTM_MaxSpeed || m_mode == rdoRuntime::RTM_Jump)
	{
		// ����� ����� ������� � ��������� ��������
		m_next_delay_current = m_next_delay_count;
		m_msec_wait          = 0;
	}
}

void RDOSimulatorBase::setSpeed( double value )
{
	if (value < 0) value = 0;
	if (value > 1) value = 1;
	m_speed = value;
	m_next_delay_count = (unsigned int)((1 - m_speed) * m_speed_range_max);
	// ����� ����� ������� � ��������� ��������
	m_next_delay_current = m_next_delay_count;
	m_msec_wait          = 0;
}

void RDOSimulatorBase::setShowRate( double value )
{
	if (value < DBL_MIN) value = DBL_MIN;
	if (value > DBL_MAX) value = DBL_MAX;
	m_showRate = value;
	// ����� ����� ������� � ��������� ��������
	m_next_delay_current = m_next_delay_count;
	m_msec_wait          = 0;
}

void RDOSimulatorBase::rdoPostProcess()
{
	postProcess();
	while (!m_timePoints.empty())
	{
		delete m_timePoints.begin()->second;
		m_timePoints.erase(m_timePoints.begin());
	}
}

void RDOSimulatorBase::addTimePoint(double timePoint, CREF(LPIBaseOperation) opr, void* param)
{
	BOPlannedItem* list = NULL;
	if (opr && (m_timePoints.find(timePoint) == m_timePoints.end() || m_timePoints[timePoint] == NULL))
	{
		list = new BOPlannedItem();
		m_timePoints[timePoint] = list;
	}
	if (opr)
	{
		m_timePoints[timePoint]->push_back(BOPlanned(opr, param));
	}
}

void RDOSimulatorBase::removeTimePoint(CREF(LPIBaseOperation) opr)
{
	BOPlannedMap::iterator it = m_timePoints.begin();
	while (it != m_timePoints.end())
	{
		BOPlannedItem* list = it->second;
		BOPlannedItem::iterator item = list->begin();
		while (item != list->end())
		{
			// ������ �������� �� ������ ���������������
			if (item->m_opr == opr)
			{
				item = list->erase(item);
				continue;
			}
			++item;
		}
		// ���� ������ �������������� ������, �� ������ � ���
		if (list->empty())
		{
			it = m_timePoints.erase(it);
			continue;
		}
		else
		{
			++it;
		}
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
