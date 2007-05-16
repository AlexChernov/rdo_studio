#include "pch.h"
#include "searchtree.h"

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
	nodeCount( 1 ),
	nodesInGraphCount( 1 ),
	expandedNodesCount( 0 ),
	fullNodesCount( 0 ),
	sizeof_dpt( 0 )
{
	::GetSystemTime( &systime_begin );
}

// ----------------------------------------------------------------------------
// ---------- TreeNode - ���� ����� DPT
// ----------------------------------------------------------------------------
TreeNode::TreeNode( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, RDOActivity* i_activity, double cost, int cnt ):
	currAct( NULL ),
	childSim( NULL ),
	newCostPath( 0 ),
	newCostRest( 0 ),
	newCostRule( 0 ),
	sim( i_sim ),
	parent( i_parent ),
	root( i_root ),
	activity( i_activity ),
	costRule( 0 ),
	costPath( cost ),
	costRest( 0 ),
	count( cnt )
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

void TreeNode::ExpandChildren()
{
	root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * children.size();
	DeleteAllObjects( children );

	// ������ ����������
	onSearchOpenNode( root->theRealSimulator );

	// ��������� �� �������� �������
	// todo: ��������, ���� ��������� ��� ������� � ������ OPEN
	if ( root->dp->TermCondition(sim) ) {
		root->targetNode = this;
		return;
	}

	// ������ ��� ����������
	root->expandedNodesCount++;

	// ������ �� ���� ����������� ����� �����
	for ( std::list< RDOActivity* >::iterator i = root->dp->activities.begin(); i != root->dp->activities.end(); i++ ) {
		currAct  = (*i);
		childSim = sim->createCopy();
		root->sizeof_dpt += childSim->sizeof_sim;
		currAct->rule->onBeforeChoiceFrom( childSim );
		if ( !currAct->rule->choiceFrom( childSim ) ) {
			// �� ������ Choice from, ������� ��������� � ��������� � ������ ����������.
			// todo: � ����� ������� ���������, ���� �� �� ���������� ?
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
				TreeNode* better = NULL;
				int res = root->rootNode->CheckIfExistBetter( childSim, newCostPath, &better );
				if ( res == 1 ) {
					// � ����� ���� ����� ������ �������, �.�. ������� ����������
					// ���������� ���� (������) �� ������ � ���� �� ���������.
					// ���� ������������� �� ����.
					// ������ ����������� ��������� �������.
					onSearchNodeInfoDeleted( root->theRealSimulator );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					// ��������� � ��������� ����������
					continue;
				} else if ( res == 2 ) {
					// ����� ��������
					// � ����� ������� ������� (better) � ����� ������ ���������� ����,
					// �.�. ������� ���������� ���������� ����� (�������).
					// ���������� ����������� ����
					// ������� �������� ��� ����� �� ������� �������� better->parent
					better->parent->children.erase( std::find(better->parent->children.begin(), better->parent->children.end(), better) );
					root->sizeof_dpt -= sim->sizeof_sim + sizeof(TreeNode);
					// ������ ������������� ��������� ���� ������� � ���� � ��������
					// todo: � ��� �� ��� ������, �.�. costPath � ������ �������,
					// ������������ ��� costPath + newCostRule, ����� �� ��������
					// ��-�� newCostRule ��-�� ������ ���������� ���������� ������,
					// �.�. ������ ���� �������� better->costRule (��. ����), � �����
					// �������� better->ReCostSubTree( costPath )
					better->ReCostSubTree( costPath );
					// ������ �������� �� ������� �������
					better->parent   = this;
					// ����������� � ����� �����
					better->count    = root->nodeCount++;
					// ��������� ���������
					better->costPath = newCostPath;
					better->costRest = newCostRest;
					better->costRule = newCostRule;
					// ������ ��������� �� ����� ����������
					better->currAct  = currAct;
					// ������� ����������� �� ����� ��������
					better->onSearchNodeInfoReplaced( root->theRealSimulator );
					// ��������� � ������ �������� �������
					children.push_back( better );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
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
				root->allLeafs.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// ������� � ������ OPEN
			root->allLeafs.push_back( tn );
		}
	}
	// ������ ������� ������� �� ������ OPEN
	root->allLeafs.erase( std::find(root->allLeafs.begin(), root->allLeafs.end(), this) );
	root->sizeof_dpt -= sim->sizeof_sim + sizeof(TreeNode);

	// ����������� ��� ������� � ������ OPEN
	std::sort( root->allLeafs.begin(), root->allLeafs.end(), compareNodes );
}

int TreeNode::CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** better )
{
	if ( *childSim == *sim ) {
		if ( costPath <= useCost ) {
			return 1;
		} else {
			// ������ 
			*better = this;
			return 2;
		}
	}

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		int res = (*i)->CheckIfExistBetter( childSim, useCost, better );
		if ( res != 0 ) return res;
	}

	return 0;
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
	return new TreeNode(childSim, this, root, currAct, costPath, root->nodeCount++);
}

} // namespace rdoRuntime;
