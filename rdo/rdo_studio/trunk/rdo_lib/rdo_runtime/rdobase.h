/*!
  \copyright (c) RDO-Team, 2006
  \file      rdobase.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ���������� �����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_BASE_H_
#define _LIB_RUNTIME_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_common/rdocommon.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOSimulatorBase
  \brief     ���� �� ������� ������� ��� RDORuntime
*/
OBJECT(RDOSimulatorBase)
{
DECLARE_FACTORY(RDOSimulatorBase)
public:
	// ��������� ������ ���������� �����������
	virtual void  rdoInit();
	virtual rbool rdoNext();
	virtual void  rdoPostProcess();

	void   setStartTime  (double value);
	double getCurrentTime() const;

	RunTimeMode getMode() const;
	void        setMode(RunTimeMode _mode);

	double getSpeed() const;
	void setSpeed(double persent);

	double getShowRate() const;
	void setShowRate(double value);

	void addTimePoint   (double timePoint, CREF(LPIBaseOperation) opr, void* param = NULL);
	void removeTimePoint(CREF(LPIBaseOperation) opr);

	void inc_cnt_events();
	void inc_cnt_choice_from();

	ruint get_cnt_events();
	ruint get_cnt_choice_from();
	ruint get_cnt_calc_arithm() const;
	ruint get_cnt_calc_logic()  const;

	static ruint getMSec(CREF(SYSTEMTIME) systime);

protected:
	RDOSimulatorBase();
	virtual ~RDOSimulatorBase();

	struct BOPlanned
	{
		LPIBaseOperation  m_opr;
		PTR(void)         m_param;
		
		BOPlanned();
		BOPlanned(CREF(BOPlanned) copy);
		BOPlanned(LPIBaseOperation opr, PTR(void) param = NULL);
	};
	typedef  std::list<BOPlanned>                  BOPlannedItem;
	typedef  std::map<double, PTR(BOPlannedItem)>  BOPlannedMap;

	BOPlannedMap m_timePoints;
	rbool        m_checkOperation;

	void setCurrentTime(double value);

	// ���������� ����� �������� (��������, DPT � ��������)
	// ���� ��������� �������� true, �� ���������� ������� doOperation
	// � � ��������� ���/ ��� �������� ���������� ������� ������
	virtual rbool doOperation()  = 0;

	// �������� �� ������� ����� �������������
	virtual rbool endCondition() = 0;

	// �������� �� ����� ��������
	virtual rbool breakPoints() = 0;

	// �������������/������� ����������
	virtual void onInit()    = 0;
	virtual void onDestroy() = 0;

	// ��������� ������� ����� �������������
	// ���������, ���� ��� ������ �������
	virtual void onNothingMoreToDo() = 0;
	// ���������, ���� ��������� ������� ����� �������������
	virtual void onEndCondition()    = 0;
	// ���������, t���� �������� ������ �������
	virtual void onRuntimeError()    = 0;
	// ����������, ���� ������ ����������� �������������
	virtual void onUserBreak()       = 0;

	// ���������� � ����� ������ �������
	virtual void preProcess()  = 0;
	// ���������� ��������������� ����� ���������� �������
	virtual void postProcess() = 0;

	// �������� �� ������� ������� ��� �������� �������
	virtual rbool isKeyDown()   = 0;

	/// @todo �� ��������� �� ��� ���������� ��-���������?
	// ���������� ��� ���������� ���������� �������
	virtual void onNewTimeNow();

private:
	double m_startTime;
	double m_currentTime;
	double m_nextTime;


	RunTimeMode  m_mode;

	double       m_speed;
	ruint        m_speed_range_max;
	ruint        m_next_delay_count;
	ruint        m_next_delay_current;

	double       m_showRate;
	double       m_msec_wait;
	ruint        m_msec_prev;

	ruint        m_cnt_events;
	ruint        m_cnt_choice_from;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdobase.inl"

#endif // _LIB_RUNTIME_BASE_H_
