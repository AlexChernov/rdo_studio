#ifndef RDOBASE_H
#define RDOBASE_H

#include "rdoruntime_object.h"
#include "rdodefines.h"
#include <rdocommon.h>

namespace rdoRuntime
{

class RDOBaseOperation;

class RDOSimulatorBase: public RDORuntimeParent
{
private:
	double startTime;
	double currentTime;
	double nextTime;

	RunTimeMode  mode;

	double       speed;
	unsigned int speed_range_max;
	unsigned int next_delay_count;
	unsigned int next_delay_current;

	double       showRate;
	double       msec_wait;
	unsigned int msec_prev;

	unsigned int cnt_events;
	unsigned int cnt_choice_from;
	unsigned int cnt_calc_arithm;
	unsigned int cnt_calc_logic;

protected:
	struct BOPlanned {
		RDOBaseOperation* opr;
		void*             param;
		BOPlanned()                                             : opr( NULL )    , param( NULL )       {}
		BOPlanned( const BOPlanned& copy )                      : opr( copy.opr ), param( copy.param ) {}
		BOPlanned( RDOBaseOperation* _opr, void* _param = NULL ): opr( _opr )    , param( _param )     {}
	};
	std::map< double, std::list< BOPlanned >* > timePointList;

	bool check_operation;

	void setCurrentTime( double value ) { currentTime = value; }

	// ���������� ����� �������� (��������, DPT � ��������)
	// ���� ��������� �������� true, �� ���������� ������� doOperation
	// � � ��������� ���/ ��� �������� ���������� ������� ������
	virtual bool doOperation()  = 0;

	// �������� �� ������� ����� �������������
	virtual bool endCondition() = 0;

	// �������� �� ����� ��������
	virtual bool breakPoints() = 0;

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
	virtual bool isKeyDown()   = 0;

	// ���������� ��� ���������� ���������� �������
	virtual void onNewTimeNow() {};

public:
	// ��������� ������ ���������� �����������
	virtual void rdoInit();
	virtual void rdoDestroy();
	virtual bool rdoNext();
	virtual void rdoPostProcess();

	void setStartTime( double value )       { startTime = value;  }
	double getCurrentTime() const           { return currentTime; }

	RunTimeMode getMode() const             { return mode;        }
	void setMode( RunTimeMode _mode );

	double getSpeed() const                 { return speed;       }
	void setSpeed( double persent );

	double getShowRate() const              { return showRate;    }
	void setShowRate( double value );

	void addTimePoint( double timePoint, RDOBaseOperation* opr = NULL, void* param = NULL  );

	RDOSimulatorBase( RDORuntimeParent* _runtime );
	virtual ~RDOSimulatorBase() {}

	void inc_cnt_events()      { cnt_events++;      }
	void inc_cnt_choice_from() { cnt_choice_from++; }
	void inc_cnt_calc_arithm() { cnt_calc_arithm++; }
	void inc_cnt_calc_logic()  { cnt_calc_logic++;  }

	unsigned int get_cnt_events()      { return cnt_events;      }
	unsigned int get_cnt_choice_from() { return cnt_choice_from; }
	unsigned int get_cnt_calc_arithm() { return cnt_calc_arithm; }
	unsigned int get_cnt_calc_logic()  { return cnt_calc_logic;  }

	static unsigned int getMSec( const SYSTEMTIME& systime ) {
		return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
	}
};

} // namespace rdoRuntime;

#endif // RDOBASE_H
