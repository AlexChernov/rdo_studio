#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

#include "rdo.h"
#include "rdoprocess.h"

bool CheckOperations::operator()(RDOBaseOperation *op)
{
	return op->checkOperation(sim) != RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODecisionPoint::checkOperation(RDOSimulator *sim)
{
	if ( Condition(sim) ) {
		if ( RunSearchInTree(sim) ) {
			return RDOBaseOperation::BOR_can_run;
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDOOperation::checkOperation( RDOSimulator* sim )
{
	// ���� �������� ����� ��������, �� ������� � ���� � ��������� ��� � ������
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		RDOOperation* newOp = clone( sim );
		newOp->onBeforeOperationBegin( sim );
		newOp->convertBegin( sim );
		sim->addTimePoint( newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()), this, newOp );
		newOp->onAfterOperationBegin( sim );
		newOp->convert_end = true;
		operation_clone.push_back( newOp );
		return RDOBaseOperation::BOR_planned_and_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

void RDOOperation::makePlanned( RDOSimulator* sim, void* param )
{
	// ��������� ������� ����� ��������-�����
	RDOOperation* opr = static_cast<RDOOperation*>(param);
	opr->onBeforeOperationEnd( sim );
	opr->convertEnd( sim );
	opr->onAfterOperationEnd( sim );
	operation_clone.remove( opr );
	delete opr;
}

void RDOIE::init( RDOSimulator* sim )
{
	onBeforeIrregularEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
}

RDOBaseOperation::BOResult RDOIE::checkOperation(RDOSimulator *sim)
{
	return RDOBaseOperation::BOR_cant_run;
}

void RDOIE::makePlanned( RDOSimulator* sim, void* param )
{
	onBeforeIrregularEvent( sim );
	convertEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
	onAfterIrregularEvent( sim );
}

RDOBaseOperation::BOResult RDORule::checkOperation( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	if ( choiceFrom(sim) ) {
		onBeforeRule( sim );
		convertRule( sim );
		onAfterRule( sim );
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

bool RDOSimulator::doOperation()
{
	onCheckPokaz();
	onAfterCheckPokaz();

	// ���������� ��� ��������������� �� ������ ������ �������
	if ( !check_operation && !timePointList.empty() ) {
		check_operation = true;
		double newTime = timePointList.begin()->first;
		if ( getCurrentTime() >= newTime ) {
			std::list< BOPlanned >* list = timePointList.begin()->second;
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
					delete timePointList.begin()->second;
					timePointList.erase( timePointList.begin() );
				}
				opr->makePlanned( this, param );
				return true;
			}
		}
	}
	// �� ����� ��������������� �������
	// ��������� ��� ��������� ������� � ��������, ������� ������, ������� ����� ���� �������
	bool res = std::find_if( haveBaseOperations.begin(), haveBaseOperations.end(), CheckOperations(this) ) != haveBaseOperations.end();
	if ( !res ) check_operation = false;
	return res;
}

void RDOSimulator::rdoDestroy()
{
	RDOSimulatorBase::rdoDestroy();
}

void RDOSimulator::rdoInit()
{
	RDOSimulatorBase::rdoInit();
}

void RDOSimulator::preProcess()
{
	for ( std::vector< RDOBaseOperation* >::const_iterator it = haveBaseOperations.begin(); it != haveBaseOperations.end(); it++ ) {
		(*it)->init( this );
	}
	onResetPokaz();
}

RDODecisionPoint::~RDODecisionPoint()
{
	DeleteAllObjects( activities );
}

RDOActivity::~RDOActivity()
{
	delete rule;
}

TreeRoot *RDODecisionPoint::createTreeRoot(RDOSimulator *sim)
{
	return new TreeRoot( sim, this );
}

void RDODecisionPoint::addActivity(RDOActivity *act)
{
	activities.push_back(act); 
}

RDOSimulator *RDOSimulator::createCopy()
{
	RDOSimulator* res = clone();
	res->setCurrentTime(getCurrentTime());
	return res;
}
