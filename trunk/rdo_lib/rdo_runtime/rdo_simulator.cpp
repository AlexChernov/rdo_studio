#include "pch.h"
#include "rdo_simulator.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - ���� �� ������� ������� ��� RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// ���� ��������, ������� ���������� ����������. ������������ � DPT
		RDOBaseOperation::BOResult result = getMustContinueOpr()->onContinue( this );
		if ( result != RDOBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != RDOBaseOperation::BOR_cant_run;
	} else {
		bool found_planed = false;
		// ���������� ��� ��������������� �� ������ ������ �������
		if ( !check_operation && !m_timePoints.empty() ) {
			check_operation = true;
			double newTime = m_timePoints.begin()->first;
			if ( getCurrentTime() >= newTime ) {
				BOPlannedItem* list = m_timePoints.begin()->second;
				if ( list && !list->empty() ) {
#ifdef RDOSIM_COMPATIBLE
					// ���������� ������ ������� ������� LIFO
					RDOBaseOperation* opr   = list->back().opr;
					void*             param = list->back().param;
					list->pop_back();
#else
					// ���������� ������ ������� ������� FIFO
					RDOBaseOperation* opr   = list->front().opr;
					void*             param = list->front().param;
					list->pop_front();
#endif
					if ( list->empty() ) {
						delete list;
//						delete m_timePoints.begin()->second;
						m_timePoints.erase( m_timePoints.begin() );
					}
					opr->onMakePlaned( this, param );
					found_planed = true;
				}
			}
		}
		res = found_planed;
		if ( !found_planed ) {
			// �� ����� ��������������� �������
			// ��������� ��� ��������� ������� � ��������, ������� ������, ������� ����� ���� �������
			res = m_logics.onCheckCondition(this);
			if ( res )
			{
				res = m_logics.onDoOperation(this) != RDOBaseOperation::BOR_cant_run;
			}
			if ( !res ) check_operation = false;
		}
	}
	onCheckPokaz();
	onAfterCheckPokaz();
	return res;
}

void RDOSimulator::preProcess()
{
	m_logics.onStart( this );
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}


std::string writeActivitiesStructureRecurse( RDOBaseLogic* logic, int& counter )
{
	std::stringstream stream;
	RDOBaseLogic::CIterator it = logic->begin();
	while ( it != logic->end() ) {
		RDORule* rule = dynamic_cast<RDORule*>(*it);
		if ( rule ) {
			stream << counter++ << " ";
			rule->writeModelStructure( stream );
		} else {
			RDOOperation* opr = dynamic_cast<RDOOperation*>(*it);
			if ( opr ) {
				stream << counter++ << " ";
				opr->writeModelStructure( stream );
			}
		}
		it++;
	}
#ifdef RDOSIM_COMPATIBLE
#else
	stream << std::endl;
#endif

	int _counter = 1;
	it = logic->begin();
	while ( it != logic->end() ) {
		RDOIrregEvent* ie = dynamic_cast<RDOIrregEvent*>(*it);
		if ( ie ) {
			stream << _counter++ << " ";
			counter++;
			ie->writeModelStructure( stream );
		}
		it++;
	}

	it = logic->begin();
	while ( it != logic->end() ) {
		RDOBaseLogic* logic = dynamic_cast<RDOBaseLogic*>(*it);
		if ( logic )
		{
			stream << writeActivitiesStructureRecurse( logic, counter );
		}
		it++;
	}

	return stream.str();
}

std::string RDOSimulator::writeActivitiesStructure( int& counter )
{
	return writeActivitiesStructureRecurse( &m_logics, counter );
}

} // namespace rdoRuntime
