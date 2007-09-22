#include "pch.h"
#include "rdobase.h"
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

RDOSimulatorBase::RDOSimulatorBase( RDORuntimeParent* _runtime ):
	RDORuntimeParent( _runtime ),
	startTime( 0 ),
	currentTime( 0 ),
	nextTime( 0 ),
	mode( rdoRuntime::RTM_MaxSpeed ),
	speed( 1 ),
	speed_range_max( 500000 ),
	next_delay_count( 0 ),
	next_delay_current( 0 ),
	showRate( 60 ),
	msec_wait( 0 ),
	msec_prev( 0 ),
	check_operation( true )
{
}

void RDOSimulatorBase::rdoInit()
{
	currentTime     = startTime;
	nextTime        = currentTime;
	check_operation = true;
	onInit();

	timePointList.clear();
	timePointList[currentTime] = NULL;
	preProcess();

	speed              = 1;
	next_delay_count   = 0;
	next_delay_current = 0;
	showRate           = 60;
	msec_wait          = 0;

	onNewTimeNow();
}

void RDOSimulatorBase::rdoDestroy()
{
	onDestroy();
}

bool RDOSimulatorBase::rdoNext()
{
	if ( mode == rdoRuntime::RTM_Pause || mode == rdoRuntime::RTM_BreakPoint ) {
		::Sleep( 1 );
		return true;
	}
	// ���� ������ ������� ��� �������� �������, �� �������� ���� ����������
	bool keyboard = isKeyDown();
	if ( !keyboard ) {
		// �������� ����� �������� �������������
		// ��� mode == rdoRuntime::RTM_Jump || mode == rdoRuntime::RTM_Sync
		if ( mode != rdoRuntime::RTM_MaxSpeed && next_delay_count ) {
			next_delay_current++;
			if ( next_delay_current < next_delay_count ) return true;
			next_delay_current = 0;
		}
		// �������� ���������� �������� ������������� (������������ ��������)
		// ��� �� ���� ��������� mode == rdoRuntime::RTM_Sync, �.�. ��� ��� �������� � msec_wait,
		// ������� ������������ � setMode � �� ���������� �����.
		if ( msec_wait > 1 ) {
			SYSTEMTIME systime_current;
			::GetSystemTime( &systime_current );
			unsigned int msec_curr = getMSec( systime_current );
			unsigned int msec_delta;
			// ����������� ��������� � ������ �����, �� ��� ����� ����� ���� ������������ �� ����,
			// � ������� ����� � ������������ ���������� ������ �����������. ��������� ���� ������
			// ����� ����� ELSE. ������ ������� ������ ������ ������� �� ���� ���� ������ �����
			// ����� ��������� ��������� �������, �� �� ������ ������ �� ��� � ����� ���. ���
			// �������� ��������������� �������� (����� ���������� ������� ��� ���), �� ��������
			// ����� ������������� � ������� ����������� ��������� �������, ������� �������� �
			// �������������� ������, ����������� ���������� � ������������. ��� ��� ������ �� �����.
			// ��� ������� - ����� �� ���������� ������ � ����� ��������, � ���� ������ ��������, ���
			// ������� �� ������ � ���������� ����, ������� � ���. SYSTEMTIME �������� ����� ����������.
			if ( msec_curr >= msec_prev ) {
				msec_delta = msec_curr - msec_prev;
			} else {
				msec_delta = UINT_MAX - msec_prev + msec_curr;
			}
			if ( msec_delta <= msec_wait ) return true;
			msec_wait -= msec_delta;
		}
	}
	// ��������� ������������� - ��������� ������� �����
	if ( endCondition() ) {
		onEndCondition();
		return false;
	}
	if ( currentTime != nextTime ) {
		currentTime = nextTime;
		onNewTimeNow();
	}
	// ���������� ��������
	if ( doOperation() ) {
		if ( breakPoints() ) {
			setMode( RTM_BreakPoint );
		}
		return true;
	} else {
		// ������� � ��������� ��������
		if ( !timePointList.empty() ) {
			double newTime = timePointList.begin()->first;
			std::list< BOPlanned >* list = timePointList.begin()->second;
			if ( !list || list->empty() ) {
				delete timePointList.begin()->second;
				timePointList.erase( timePointList.begin() );
			}
//			timePointList.pop_front();
			if ( currentTime > newTime ) {
				newTime = currentTime;
			}
			if ( mode == rdoRuntime::RTM_Sync ) {
				msec_wait += (newTime - nextTime) * 3600.0 * 1000.0 / showRate;
				if ( msec_wait > 0 ) {
					if ( nextTime != startTime ) {
						if ( speed > DBL_MIN ) {
							msec_wait = msec_wait / speed;
						} else {
							msec_wait = msec_wait / DBL_MIN;
						}
						SYSTEMTIME systime_current;
						::GetSystemTime( &systime_current );
						msec_prev = getMSec( systime_current );
					} else {
						msec_wait = 0;
					}
				}
			}
			nextTime = newTime;
			return true;
		} else {
			// ��������� ������������� - ��� ������ �������
			onNothingMoreToDo();
			return false;
		}
	}
	return true;
}

void RDOSimulatorBase::setMode( rdoRuntime::RunTimeMode _mode )
{
	if ( mode == rdoRuntime::RTM_Pause ) {
		// ����� ����� ������� � ��������� �������� ����� ����� � ����� '�� ������'
		next_delay_current = next_delay_count;
		msec_wait          = 0;
	}
	mode = _mode;
	if ( mode == rdoRuntime::RTM_MaxSpeed || mode == rdoRuntime::RTM_Jump ) {
		// ����� ����� ������� � ��������� ��������
		next_delay_current = next_delay_count;
		msec_wait          = 0;
	}
}

void RDOSimulatorBase::setSpeed( double value )
{
	if ( value < 0 ) value = 0;
	if ( value > 1 ) value = 1;
	speed = value;
	next_delay_count = (1 - speed) * speed_range_max;
	// ����� ����� ������� � ��������� ��������
	next_delay_current = next_delay_count;
	msec_wait          = 0;
}

void RDOSimulatorBase::setShowRate( double value )
{
	if ( value < DBL_MIN ) value = DBL_MIN;
	if ( value > DBL_MAX ) value = DBL_MAX;
	showRate = value;
	// ����� ����� ������� � ��������� ��������
	next_delay_current = next_delay_count;
	msec_wait          = 0;
}

void RDOSimulatorBase::rdoPostProcess()
{
	postProcess();
	while ( !timePointList.empty() ) {
		delete timePointList.begin()->second;
		timePointList.erase( timePointList.begin() );
	}
}

void RDOSimulatorBase::addTimePoint( double timePoint, RDOBaseOperation* opr, void* param )
{
	std::list< BOPlanned >* list = NULL;
	if ( opr && (timePointList.find( timePoint ) == timePointList.end() || timePointList[timePoint] == NULL) ) {
		list = new std::list< BOPlanned >();
		timePointList[timePoint] = list;
	}
	if ( opr ) {
		timePointList[timePoint]->push_back( BOPlanned(opr, param) );
	}
}

} // namespace rdoRuntime;
