#include "pch.h"
#include "searchtree.h"
#include "searchtrace.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- TreeRoot - ������ ������ DPT
// ----------------------------------------------------------------------------
TreeRoot::TreeRoot( RDOSimulator* sim, RDODecisionPoint* _dp ):
	dp( _dp ),
	rootNode( NULL ),
	targetNode( NULL ),
	theRealSimulator( sim ),
	nodesCount( 0 ),
	nodesInGraphCount( 1 ), // ���� ��������� �������
	expandedNodesCount( 0 ),
	fullNodesCount( 0 ),
	sizeof_dpt( 0 )
{
	::GetSystemTime( &systime_begin );
}

// ----------------------------------------------------------------------------
// ---------- TreeNode - ���� ����� DPT
// ----------------------------------------------------------------------------
TreeNode::TreeNode( RDOSimulator* _sim, TreeNode* _parent, TreeRoot* _root, RDOActivity* _activity, double cost, int cnt ):
	currAct( NULL ),
	childSim( NULL ),
	newCostPath( 0 ),
	newCostRest( 0 ),
	newCostRule( 0 ),
	sim( _sim ),
	parent( _parent ),
	root( _root ),
	activity( _activity ),
	costRule( 0 ),
	costPath( cost ),
	costRest( 0 ),
	number( cnt )
{
	sim->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
	for( std::vector<TreeNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
		delete (*i);
		root->sizeof_dpt -= sizeof(TreeNode);
	}
	root->sizeof_dpt -= sim->sizeof_sim;
	delete sim;
}

int TreeNode::getActivityID() const
{
	return ((RDOActivityTrace*)activity)->getTraceID();
}

void TreeNode::ExpandChildren()
{
	root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * children.size();
	DeleteAllObjects( children );

	// ������ ����������
	onSearchOpenNode( root->theRealSimulator );

	// ��������� �� �������� �������
	// TODO: ��������, ���� ��������� ��� ������� � ������ OPEN
	if ( root->dp->TermCondition(sim) ) {
		root->targetNode = this;
#ifdef _DEBUG
		static_cast<RDORuntime*>(sim)->showResources(number);
#endif
		return;
	}

	// ������ ��� ����������
	root->expandedNodesCount++;

	// ������ �� ���� ����������� ����� �����
	for ( std::list< RDOActivity* >::iterator i = root->dp->activities.begin(); i != root->dp->activities.end(); i++ ) {
		currAct  = (*i);
		childSim = sim->createCopy();
#ifdef _DEBUG
		if ( static_cast<RDORuntime*>(childSim)->checkState() ) {
			TRACE( "���������, node = %d\n", number );
		}
#endif
		root->sizeof_dpt += childSim->sizeof_sim;
		currAct->rule->onBeforeChoiceFrom( childSim );
		if ( !currAct->rule->choiceFrom( childSim ) ) {
			// �� ������ Choice from, ������� ��������� � ��������� � ������ ����������.
			// TODO: � ����� ������� ���������, ���� �� �� ���������� ?
			// ����� ����� ��������, ���� ��� ���������� ���������� �������� �����
			// ���������� calc, �� ������� ����� ����������� �� ������ ��� �������
			// �������, ������� ����� �������� ��������� ��������
			// ���� ����� ������� �������������� � ������� ���.
			root->sizeof_dpt -= childSim->sizeof_sim;
			delete childSim;
			childSim = NULL;
		} else {
			// ������ ��� ����������
			root->fullNodesCount++;
			// ��������� ��������� ���������� ������� (value before)
			if ( !currAct->valueAfter ) {
				newCostRule = currAct->costOfRule( childSim );
			}
			// ��������� ���� ������� (�������� �������)
			currAct->rule->onBeforeRule( childSim );
			currAct->rule->convertRule( childSim );
			currAct->rule->onAfterRule( childSim, true );

			// ��������� ��������� ���������� ������� (value after)
			if ( currAct->valueAfter ) {
				newCostRule = currAct->costOfRule( childSim );
			}
			// ��������� ��������� ���� �� ������� �������
			newCostPath = costPath + newCostRule;
			// ��������� ��������� ���� �� ���� � ������ ������
			// ������ �� ����� �������� � ������ ��������������� ������ OPEN
			newCostRest = newCostPath + root->dp->EvaluateBy( childSim );

			// ���� �� ���������� ������� (���������� ��� ����������)
			// �������� ����� ��������
			if ( root->dp->NeedCompareTops() ) {
				TreeNode* loser = NULL;
				NodeFoundInfo res = root->rootNode->CheckIfExistBetter( childSim, newCostPath, &loser );
				if ( res == nfi_found_better ) {
					// � ����� ���� ����� ������ �������, �.�. ������� ����������
					// ���������� ���� (������) �� ������ � ���� �� ���������.
					// ���� ������������� �� ����.
					// ������ ����������� ��������� �������.
					onSearchNodeInfoDeleted( root->theRealSimulator );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					// ��������� � ��������� ����������
					continue;
				} else if ( res == nfi_found_loser ) {
#ifdef _DEBUG
					if ( number == 294 ) {
						TRACE( "loser->number = %d\n", loser->number );
						TRACE( "loser->parent->number = %d\n", loser->parent->number );
						TRACE( "loser->parent->children.size() = %d\n", loser->parent->children.size() );
						static_cast<RDORuntime*>(loser->sim)->showResources(loser->number);
					}
#endif
					// ����� ��������
					// � ����� ������� ������� (loser) � ����� ������ ���������� ����,
					// �.�. ������� ���������� ���������� ����� (�������).
					// ���������� ����������� ����
					// ������� �������� ��� ����� �� ������� �������� loser->parent
					loser->parent->children.erase( std::find(loser->parent->children.begin(), loser->parent->children.end(), loser) );
#ifdef _DEBUG
					if ( number == 294 ) {
						TRACE( "loser->parent->children.size() after erase = %d\n", loser->parent->children.size() );
					}
#endif
					// ������ ������������� ��������� ���� ������� � ���� � ��������
					// todo: � ��� �� ��� ������, �.�. costPath � ������ �������,
					// ������������ ��� costPath + newCostRule, ����� �� ��������
					// ��-�� newCostRule ��-�� ������ ���������� ���������� ������,
					// �.�. ������ ���� �������� loser->costRule (��. ����), � �����
					// �������� loser->ReCostSubTree( costPath )
					loser->ReCostSubTree( costPath );
					// ������ �������� �� ������� �������
					loser->parent   = this;
					// ����������� �� ����� �����
					loser->number   = root->getNewNodeNumber();
					// ��������� ���������
					loser->costPath = newCostPath;
					loser->costRest = newCostRest;
					loser->costRule = newCostRule;
					// ������ ��������� �� ����� ����������
					loser->activity = currAct;
					// ����, �� ������ ��� ����������� ������ ����������
					loser->currAct  = currAct;
					// ������� ����������� �� ����� ��������
					loser->onSearchNodeInfoReplaced( root->theRealSimulator );
					// ��������� � ������ �������� �������
					children.push_back( loser );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
#ifdef _DEBUG
					if ( number == 294 ) {
						static_cast<RDORuntime*>(loser->sim)->showResources( loser->number );
					}
#endif
					// ��������� � ��������� ����������
					continue;
				}
			}

			// �� ����� � ����� ������ ���������
			// ��� ���� ���������. �������� ��� ����� ����� �������
			TreeNode* tn = createChildTreeNode();
			root->nodesInGraphCount++;
			tn->costPath = newCostPath;
			tn->costRest = newCostRest;
			tn->costRule = newCostRule;
			tn->currAct  = currAct;
			// ������� �����������
			tn->onSearchNodeInfoNew( root->theRealSimulator );
			// ������� � ������ �������� �������
			children.push_back(tn);
/*
			if(!root->dp->TermCondition(childSim))
				root->OPEN.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// ������� � ������ OPEN
			root->OPEN.push_back( tn );
		}
	}
	// ������ ������� ������� �� ������ OPEN
	root->OPEN.erase( std::find(root->OPEN.begin(), root->OPEN.end(), this) );

	// ����������� ��� ������� � ������ OPEN
	std::sort( root->OPEN.begin(), root->OPEN.end(), compareNodes );
}

TreeNode::NodeFoundInfo TreeNode::CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** loser )
{
	if ( *childSim == *sim ) {
		if ( costPath <= useCost ) {
			return nfi_found_better;
		} else {
			*loser = this;
			return nfi_found_loser;
		}
	}

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		NodeFoundInfo res = (*i)->CheckIfExistBetter( childSim, useCost, loser );
		if ( res != nfi_notfound ) return res;
	}

	return nfi_notfound;
}

void TreeNode::ReCostSubTree( double cost )
{
	costPath = cost + costRule;

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		(*i)->ReCostSubTree( costPath );
	}
}

TreeNode* TreeNode::createChildTreeNode()
{
	root->sizeof_dpt += sizeof( TreeNode );
	return new TreeNode(childSim, this, root, currAct, costPath, root->getNewNodeNumber());
}

} // namespace rdoRuntime;
