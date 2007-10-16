#include "pch.h"
#include "rdo.h"
#include "rdoprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- CheckOperations - ��������� �������� �� ������ RDOBaseOperation
// ----------------------------------------------------------------------------
bool CheckOperations::operator()( RDOBaseOperation* opr )
{
	RDOBaseOperation::BOResult result = opr->checkOperation( sim );
	if ( result == RDOBaseOperation::BOR_must_continue ) {
		sim->setMustContinueOpr( opr );
	}
	return result != RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOIE - irregular_event
// ----------------------------------------------------------------------------
void RDOIE::init( RDOSimulator* sim )
{
	onBeforeIrregularEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
}

RDOBaseOperation::BOResult RDOIE::checkOperation(RDOSimulator *sim)
{
	return RDOBaseOperation::BOR_cant_run;
}

void RDOIE::makePlaned( RDOSimulator* sim, void* param )
{
	sim->inc_cnt_events();
	onBeforeIrregularEvent( sim );
	convertEvent( sim );
	sim->addTimePoint( time = (getNextTimeInterval(sim) + sim->getCurrentTime()), this );
	onAfterIrregularEvent( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDORule - rule
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDORule::checkOperation( RDOSimulator* sim )
{
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	if ( choiceFrom(sim) ) {
		onBeforeRule( sim );
		convertRule( sim );
		onAfterRule( sim );
		return RDOBaseOperation::BOR_can_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOOperation - operation
// ----------------------------------------------------------------------------
RDOBaseOperation::BOResult RDOOperation::checkOperation( RDOSimulator* sim )
{
	// ���� �������� ����� ��������, �� ������� � ���� � ��������� ��� � ������
	onBeforeChoiceFrom( sim );
	sim->inc_cnt_choice_from();
	if ( choiceFrom(sim) ) {
		RDOOperation* newOp = clone( sim );
		newOp->reparent( &clones );
		newOp->onBeforeOperationBegin( sim );
		newOp->convertBegin( sim );
		sim->addTimePoint( newOp->time = (newOp->getNextTimeInterval(sim) + sim->getCurrentTime()), this, newOp );
		newOp->onAfterOperationBegin( sim );
		newOp->convert_end = true;
		return RDOBaseOperation::BOR_planned_and_run;
	}
	return RDOBaseOperation::BOR_cant_run;
}

void RDOOperation::makePlaned( RDOSimulator* sim, void* param )
{
	// ��������� ������� ����� ��������-�����
	sim->inc_cnt_events();
	RDOOperation* opr = static_cast<RDOOperation*>(param);
	opr->onBeforeOperationEnd( sim );
	opr->convertEnd( sim );
	opr->onAfterOperationEnd( sim );
	delete opr;
}

// ----------------------------------------------------------------------------
// ---------- RDODecisionPoint - DPT
// ----------------------------------------------------------------------------
RDODecisionPoint::~RDODecisionPoint()
{
//qq	DeleteAllObjects( activities );
}

RDOBaseOperation::BOResult RDODecisionPoint::checkOperation( RDOSimulator* sim )
{
	if ( Condition(sim) ) {
		return RunSearchInTree( sim );
	}
	return RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODecisionPoint::continueOperation( RDOSimulator* sim )
{
	DWORD time_begin = ::GetTickCount();
	while ( true ) {
		// ������ ��� ��������� ������ ������� �� ������ OPEN
		TreeNode* curr = *(treeRoot->allLeafs.begin());
		curr->ExpandChildren();
		if ( treeRoot->allLeafs.empty() || treeRoot->targetNode ) break;

		DWORD time_current = ::GetTickCount();
		if ( time_current - time_begin > 1000 / 40 ) {
			return BOR_must_continue;
		}
	}

	bool success = treeRoot->targetNode ? true : false;
	if ( success ) {
		// ����� �������, ������� ����
		std::list< TreeNode* > bestPath;
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
			bestPath.push_front(i);
		}
		// ���������� ��������������� ��������: ������ �����������
		onSearchDecisionHeader( treeRoot->theRealSimulator );
		// ���������� ����
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->activity->rule->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->choiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule->convertRule( treeRoot->theRealSimulator );
			node->activity->rule->onAfterRule( treeRoot->theRealSimulator, true );
			// ���������� ������ �������: ������ �����������
			onSearchDecision( treeRoot->theRealSimulator, node );
		}
		// ���������� ����������� ��������: ������ �����������, �������� ���������� �� ������
		onSearchResultSuccess( treeRoot->theRealSimulator, treeRoot );
	} else {
		// ��������� ���������� ������: ������ ����������
		onSearchResultNotFound( treeRoot->theRealSimulator, treeRoot );
	}
	delete treeRoot->rootNode;
	delete treeRoot;
	treeRoot = NULL;
	return success ? RDOBaseOperation::BOR_can_run : RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODecisionPoint::RunSearchInTree( RDOSimulator* sim )
{
	// ������ ������: ������ ����������, �������� ����������
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return continueOperation( sim );
}

void RDODecisionPoint::addActivity( RDOActivity* act )
{
	// ������� ���������� �� activities �� ����, �.�. ��� ������ ������-��������
	act->reparent( this );
	activities.push_back( act ); 
}

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - ���� �� ������� ������� ��� RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// ���� ��������, ������� ���������� ����������. ������������ � DPT
		RDOBaseOperation::BOResult result = getMustContinueOpr()->continueOperation( this );
		if ( result != RDOBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != RDOBaseOperation::BOR_cant_run;
	} else {
		bool found_planed = false;
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
					opr->makePlaned( this, param );
					found_planed = true;
				}
			}
		}
		res = found_planed;
		if ( !found_planed ) {
			// �� ����� ��������������� �������
			// ��������� ��� ��������� ������� � ��������, ������� ������, ������� ����� ���� �������
			res = std::find_if( haveBaseOperations.begin(), haveBaseOperations.end(), CheckOperations( this ) ) != haveBaseOperations.end();
			if ( !res ) check_operation = false;
		}
	}
	onCheckPokaz();
	onAfterCheckPokaz();
	return res;
}

void RDOSimulator::preProcess()
{
	for ( std::vector< RDOBaseOperation* >::const_iterator it = haveBaseOperations.begin(); it != haveBaseOperations.end(); it++ ) {
		(*it)->init( this );
	}
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}

} // namespace rdoRuntime
