#include "pch.h"
#include "rdo.h"
#include "rdoprocess.h"
#include "searchtree.h"
#include "rdo_rule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::~RDODPTSearch()
{
//qq	DeleteAllObjects( activities );
}

bool RDODPTSearch::onCheckCondition( RDOSimulator* sim )
{
	return Condition(sim);
}

RDOBaseOperation::BOResult RDODPTSearch::onDoOperation( RDOSimulator* sim )
{
	return RunSearchInTree( sim );
}

RDOBaseOperation::BOResult RDODPTSearch::onContinue( RDOSimulator* sim )
{
	DWORD time_begin = ::GetTickCount();
	while ( true ) {
		// ������ ��� ��������� ������ ������� �� ������ OPEN
		TreeNode* curr = *(treeRoot->OPEN.begin());
		curr->ExpandChildren();
		if ( treeRoot->OPEN.empty() || treeRoot->targetNode ) break;

		DWORD time_current = ::GetTickCount();
		if ( time_current - time_begin > 1000 / 40 ) {
			return BOR_must_continue;
		}
	}

	bool success = treeRoot->targetNode ? true : false;
	if ( success ) {
		// ����� �������, ������� ����
		std::list< TreeNode* > bestPath;
		TRACE( "�������... \n" );
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
#ifdef _DEBUG
			static_cast<RDORuntime*>(i->sim)->showResources(i->number);
#endif
			bestPath.push_front(i);
		}
		TRACE( "�������... done\n" );
		// ���������� ��������������� ��������: ������ �����������
		onSearchDecisionHeader( treeRoot->theRealSimulator );
		// ���������� ����
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->activity->rule()->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule()->choiceFrom( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule()->convertRule( static_cast<RDORuntime*>(treeRoot->theRealSimulator) );
			node->activity->rule()->onAfterRule( treeRoot->theRealSimulator, true );
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
	return success ? RDOBaseOperation::BOR_done : RDOBaseOperation::BOR_cant_run;
}

RDOBaseOperation::BOResult RDODPTSearch::RunSearchInTree( RDOSimulator* sim )
{
	// ������ ������: ������ ����������, �������� ����������
	onSearchBegin( sim );
	treeRoot = createTreeRoot( sim );
	treeRoot->createRootTreeNode( sim->createCopy() );

	return onContinue( sim );
}

void RDODPTSearch::addActivity( Activity* act )
{
	// ������� ���������� �� activities �� ����, �.�. ��� ������ ������-��������
	act->reparent( this );
	activities.push_back( act ); 
}

RDODPTSearch::Activity::Activity( RDORule* rule, ValueTime valueTime ):
	RDORuntimeObject( rule ),
	m_rule( rule ),
	m_valueTime( valueTime )
{
}

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

} // namespace rdoRuntime
