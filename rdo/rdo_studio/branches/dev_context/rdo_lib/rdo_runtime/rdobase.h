#ifndef RDOBASE_H
#define RDOBASE_H

#include <map>
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_common/rdocommon.h"

#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif

namespace rdoRuntime
{

class RDOSimulatorBase: public RDORuntimeParent
{
public:
	// ��������� ������ ���������� �����������
	virtual void rdoInit();
	virtual rbool rdoNext();
	virtual void rdoPostProcess();

	void setStartTime( double value )          { m_startTime = value;  }
	double getCurrentTime() const              { return m_currentTime; }

	

	RunTimeMode getMode() const                { return m_mode;        }
	void setMode( RunTimeMode _mode );

	double getSpeed() const                    { return m_speed;       }
	void setSpeed( double persent );

	double getShowRate() const                 { return m_showRate;    }
	void setShowRate( double value );

	void addTimePoint   (double timePoint, CREF(LPIBaseOperation) opr, void* param = NULL);
	void removeTimePoint(CREF(LPIBaseOperation) opr);

	void inc_cnt_events()      { m_cnt_events++;      }
	void inc_cnt_choice_from() { m_cnt_choice_from++; }

	ruint get_cnt_events()      { return m_cnt_events;      }
	ruint get_cnt_choice_from() { return m_cnt_choice_from; }
	ruint get_cnt_calc_arithm() const;
	ruint get_cnt_calc_logic()  const;

	static ruint getMSec(CREF(SYSTEMTIME) systime)
	{
		return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
	}

protected:
	RDOSimulatorBase();
	virtual ~RDOSimulatorBase() {}

	struct BOPlanned
	{
		LPIBaseOperation  m_opr;
		PTR(void)         m_param;
		
		BOPlanned()
			: m_opr  (NULL)
			, m_param(NULL)
		{}
		BOPlanned(CREF(BOPlanned) copy)
			: m_opr  (copy.m_opr  )
			, m_param(copy.m_param)
		{}
		BOPlanned(LPIBaseOperation opr, PTR(void) param = NULL)
			: m_opr  (opr  )
			, m_param(param)
		{}
	};
	typedef  std::list<BOPlanned>                  BOPlannedItem;
	typedef  std::map<double, PTR(BOPlannedItem)>  BOPlannedMap;

	BOPlannedMap m_timePoints;
	rbool        m_checkOperation;

	void setCurrentTime(double value) { m_currentTime = value; }

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

	// ���������� ��� ���������� ���������� �������
	virtual void onNewTimeNow() {};

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

} // namespace rdoRuntime;

#endif // RDOBASE_H
