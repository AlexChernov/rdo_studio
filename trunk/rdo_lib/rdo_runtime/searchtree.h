#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "rdo.h"

namespace rdoRuntime
{

class TreeNode;
class RDOSimulator;
class RDOActivity;
class RDODecisionPoint;

// ----------------------------------------------------------------------------
// ---------- TreeRoot - ������ ������ DPT
// ----------------------------------------------------------------------------
// � ������� createRootTreeNode ������� �������� ����-������
// ----------------------------------------------------------------------------
class TreeRoot
{
private:
	int nodesCount;

protected:
	TreeRoot( RDOSimulator* sim, RDODecisionPoint* _dp);

public:
	virtual ~TreeRoot() {}

	virtual void createRootTreeNode( RDOSimulator* sim ) = 0;

	std::vector< TreeNode* > OPEN;
	RDODecisionPoint* dp;
	TreeNode*         rootNode;
	TreeNode*         targetNode;
	RDOSimulator*     theRealSimulator;       // all others are copy
	int               nodesInGraphCount;
	int               expandedNodesCount;
	int               fullNodesCount;
	SYSTEMTIME        systime_begin;
	unsigned int      sizeof_dpt;

	int getNodesCound() const { return nodesCount; }
	int getNewNodeNumber() {
		return ++nodesCount;
	}

};

// ----------------------------------------------------------------------------
// ---------- TreeNode - ���� ����� DPT
// ----------------------------------------------------------------------------
class TreeNode
{
protected:
	TreeNode( RDOSimulator* _sim, TreeNode* _parent, TreeRoot* _root, RDOActivity* _activity, double cost, int cnt );

	RDOActivity*  currAct; // ������� �������� ��������� ��������� ����������
	RDOSimulator* childSim;

	double newCostPath; 
	double newCostRest;
	double newCostRule;

	virtual void onSearchOpenNode( RDOSimulator* sim )         {}
	virtual void onSearchNodeInfoDeleted( RDOSimulator* sim )  {}
	virtual void onSearchNodeInfoReplaced( RDOSimulator* sim ) {}
	virtual void onSearchNodeInfoNew( RDOSimulator* sim )      {}
	virtual TreeNode* createChildTreeNode();

public:
	virtual ~TreeNode();

	RDOSimulator* sim;
	std::vector< TreeNode* > children;
	TreeNode*    parent;
	TreeRoot*    root;
	RDOActivity* activity; // ���������� (currAct), ������� �������� ������ ��� �������� this
	double costRule;
	double costPath;
	double costRest;
	int number; // ����� ����

public:
	void ExpandChildren();

	enum NodeFoundInfo {
		nfi_notfound     = 0,
		nfi_found_better = 1,
		nfi_found_loser  = 2
	};

	NodeFoundInfo CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** better ); // return 0 - no such simulator, 1 - exist better, 2 - exist not better
	void ReCostSubTree( double cost );

	int getActivityID() const;
};

// ----------------------------------------------------------------------------
// ---------- compareNodes
// ----------------------------------------------------------------------------
// ������� ��������� ������ ����� ��� ���������� ������ OPEN
// ----------------------------------------------------------------------------
inline bool compareNodes( const TreeNode* tn1, const TreeNode* tn2 ) 
{
	if ( fabs(tn1->costRest - tn2->costRest) > 0.0000001 ) {
		return ( tn1->costRest < tn2->costRest );
	} else {
		return ( tn1->number < tn2->number );
	}
}

} // namespace rdoRuntime

#endif // SEARCHTREE_H
