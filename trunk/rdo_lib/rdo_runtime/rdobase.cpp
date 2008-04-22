#include "pch.h"
#include "rdobase.h"
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime
{

RDOSimulatorBase::RDOSimulatorBase():
	RDORuntimeParent( NULL ),
	m_startTime( 0 ),
	m_currentTime( 0 ),
	m_nextTime( 0 ),
	m_mode( rdoRuntime::RTM_MaxSpeed ),
	m_speed( 1 ),
	m_speed_range_max( 500000 ),
	m_next_delay_count( 0 ),
	m_next_delay_current( 0 ),
	m_showRate( 60 ),
	m_msec_wait( 0 ),
	m_msec_prev( 0 ),
	m_cnt_events( 0 ),
	m_cnt_choice_from( 0 ),
	m_cnt_calc_arithm( 0 ),
	m_cnt_calc_logic( 0 ),
	m_check_operation( true )
{
}

void RDOSimulatorBase::rdoInit()
{
	m_currentTime     = m_startTime;
	m_nextTime        = m_currentTime;
	m_check_operation = true;
	onInit();

	m_timePoints.clear();
	m_timePoints[m_currentTime] = NULL;
	preProcess();

	m_speed              = 1;
	m_next_delay_count   = 0;
	m_next_delay_current = 0;
	m_showRate           = 60;
	m_msec_wait          = 0;

	onNewTimeNow();
}

bool RDOSimulatorBase::rdoNext()
{
	if ( m_mode == rdoRuntime::RTM_Pause || m_mode == rdoRuntime::RTM_BreakPoint ) {
		::Sleep( 1 );
		return true;
	}
	// ���� ������ ������� ��� �������� �������, �� �������� ���� ����������
	bool keyboard = isKeyDown();
	if ( !keyboard ) {
		// �������� ����� �������� �������������
		// ��� mode == rdoRuntime::RTM_Jump || mode == rdoRuntime::RTM_Sync
		if ( m_mode != rdoRuntime::RTM_MaxSpeed && m_next_delay_count ) {
			m_next_delay_current++;
			if ( m_next_delay_current < m_next_delay_count ) return true;
			m_next_delay_current = 0;
		}
		// �������� ���������� �������� ������������� (������������ ��������)
		// ��� �� ���� ��������� mode == rdoRuntime::RTM_Sync, �.�. ��� ��� �������� � msec_wait,
		// ������� ������������ � setMode � �� ���������� �����.
		if ( m_msec_wait > 1 ) {
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
			if ( msec_curr >= m_msec_prev ) {
				msec_delta = msec_curr - m_msec_prev;
			} else {
				msec_delta = UINT_MAX - m_msec_prev + msec_curr;
			}
			if ( msec_delta <= m_msec_wait ) return true;
			m_msec_wait -= msec_delta;
		}
	}
	// ��������� ������������� - ��������� ������� �����
	if ( endCondition() ) {
		onEndCondition();
		return false;
	}
	if ( m_currentTime != m_nextTime ) {
		m_currentTime = m_nextTime;
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
			if ( m_currentTime > newTime ) {
				newTime = m_currentTime;
			}
			if ( m_mode == rdoRuntime::RTM_Sync ) {
				m_msec_wait += (newTime - m_nextTime) * 3600.0 * 1000.0 / m_showRate;
				if ( m_msec_wait > 0 ) {
					if ( m_nextTime != m_startTime ) {
						if ( m_speed > DBL_MIN ) {
							m_msec_wait = m_msec_wait / m_speed;
						} else {
							m_msec_wait = m_msec_wait / DBL_MIN;
						}
						SYSTEMTIME systime_current;
						::GetSystemTime( &systime_current );
						m_msec_prev = getMSec( systime_current );
					} else {
						m_msec_wait = 0;
					}
				}
			}
			m_nextTime = newTime;
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
	if ( m_mode == rdoRuntime::RTM_Pause ) {
		// ����� ����� ������� � ��������� �������� ����� ����� � ����� '�� ������'
		m_next_delay_current = m_next_delay_count;
		m_msec_wait          = 0;
	}
	m_mode = _mode;
	if ( m_mode == rdoRuntime::RTM_MaxSpeed || m_mode == rdoRuntime::RTM_Jump ) {
		// ����� ����� ������� � ��������� ��������
		m_next_delay_current = m_next_delay_count;
		m_msec_wait          = 0;
	}
}

void RDOSimulatorBase::setSpeed( double value )
{
	if ( value < 0 ) value = 0;
	if ( value > 1 ) value = 1;
	m_speed = value;
	m_next_delay_count = (1 - m_speed) * m_speed_range_max;
	// ����� ����� ������� � ��������� ��������
	m_next_delay_current = m_next_delay_count;
	m_msec_wait          = 0;
}

void RDOSimulatorBase::setShowRate( double value )
{
	if ( value < DBL_MIN ) value = DBL_MIN;
	if ( value > DBL_MAX ) value = DBL_MAX;
	m_showRate = value;
	// ����� ����� ������� � ��������� ��������
	m_next_delay_current = m_next_delay_count;
	m_msec_wait          = 0;
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
			if ( item->m_opr == opr )
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
