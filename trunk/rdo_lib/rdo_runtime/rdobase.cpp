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

RDOSimulatorBase::RDOSimulatorBase():
	RDORuntimeParent( NULL ),
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
	cnt_events( 0 ),
	cnt_choice_from( 0 ),
	cnt_calc_arithm( 0 ),
	cnt_calc_logic( 0 ),
	check_operation( true )
{
}

void RDOSimulatorBase::rdoInit()
{
	currentTime     = startTime;
	nextTime        = currentTime;
	check_operation = true;
	onInit();

	m_timePoints.clear();
	m_timePoints[currentTime] = NULL;
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
		if ( !m_timePoints.empty() ) {
			double newTime = m_timePoints.begin()->first;
			BOPlannedItem* list = m_timePoints.begin()->second;
			if ( !list || list->empty() ) {
				delete m_timePoints.begin()->second;
				m_timePoints.erase( m_timePoints.begin() );
			}
//			m_timePoints.pop_front();
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
	while ( !m_timePoints.empty() ) {
		delete m_timePoints.begin()->second;
		m_timePoints.erase( m_timePoints.begin() );
	}
}

void RDOSimulatorBase::addTimePoint( double timePoint, RDOBaseOperation* opr, void* param )
{
	BOPlannedItem* list = NULL;
	if ( opr && (m_timePoints.find( timePoint ) == m_timePoints.end() || m_timePoints[timePoint] == NULL) ) {
		list = new BOPlannedItem();
		m_timePoints[timePoint] = list;
	}
	if ( opr ) {
		m_timePoints[timePoint]->push_back( BOPlanned(opr, param) );
	}
}

void RDOSimulatorBase::removeTimePoint( const RDOBaseOperation* opr )
{
	BOPlannedMap::iterator it = m_timePoints.begin();
	while ( it != m_timePoints.end() )
	{
		BOPlannedItem* list = it->second;
		BOPlannedItem::iterator item = list->begin();
		while ( item != list->end() )
		{
			// ������ �������� �� ������ ���������������
			if ( item->opr == opr )
			{
				item = list->erase( item );
				continue;
			}
			item++;
		}
		// ���� ������ �������������� ������, �� ������ � ���
		if ( list->empty() )
		{
			it = m_timePoints.erase( it );
			continue;
		}
		else
		{
			it++;
		}
	}
}

} // namespace rdoRuntime;
