#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "rdo_lib/rdo_runtime/rdo_logic_dptsearch.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity_i.h"

namespace rdoRuntime
{

class TreeNode;

// ----------------------------------------------------------------------------
// ---------- TreeRoot - ������ ������ DPT
// ----------------------------------------------------------------------------
// � ������� createRootTreeNode ������� �������� ����-������
// ----------------------------------------------------------------------------
class TreeRoot
{
public:
	virtual ~TreeRoot() {}

	virtual void createRootTreeNode(CREF(LPRDORuntime) pRuntime) = 0;

	std::vector< TreeNode* > m_OPEN;
	RDODPTSearch*            m_dp;
	TreeNode*                m_rootNode;
	TreeNode*                m_targetNode;
	LPRDORuntime             m_theRealSimulator;
	int                      m_nodesInGraphCount;
	int                      m_expandedNodesCount;
	int                      m_fullNodesCount;
	SYSTEMTIME               m_systime_begin;
	unsigned int             m_sizeof_dpt;

	int getNodesCound() const { return m_nodesCount; }
	int getNewNodeNumber() {
		return ++m_nodesCount;
	}

protected:
	TreeRoot(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP);

private:
	int m_nodesCount;
};

// ----------------------------------------------------------------------------
// ---------- TreeNode - ���� ����� DPT
// ----------------------------------------------------------------------------
class TreeNode
{
public:
	virtual ~TreeNode();

	LPRDORuntime             m_pRuntime;
	std::vector< TreeNode* > m_children;
	TreeNode*                m_parent;
	TreeRoot*                m_root;
	LPIDPTSearchActivity     m_activity; // ���������� (currAct), ������� �������� ������ ��� �������� this
	double                   m_costRule;
	double                   m_costPath;
	double                   m_costRest;
	int                      m_number; // ����� ����

	void ExpandChildren();

	enum NodeFoundInfo {
		nfi_notfound     = 0,
		nfi_found_better = 1,
		nfi_found_loser  = 2
	};

	NodeFoundInfo CheckIfExistBetter(CREF(LPRDORuntime) pChildRuntime, double useCost, TreeNode** better ); // return 0 - no such simulator, 1 - exist better, 2 - exist not better
	void ReCostSubTree( double cost );

protected:
	TreeNode(CREF(LPRDORuntime) pRuntime, PTR(TreeNode) pParent, PTR(TreeRoot) pRoot, LPIDPTSearchActivity pActivity, double cost, int cnt);

	LPIDPTSearchActivity  m_currAct; // ������� �������� ��������� ��������� ����������
	LPRDORuntime          m_pChildRuntime;

	double m_newCostPath;
	double m_newCostRest;
	double m_newCostRule;

	virtual void          onSearchOpenNode        (CREF(LPRDORuntime) pRuntime) {}
	virtual void          onSearchNodeInfoDeleted (CREF(LPRDORuntime) pRuntime) {}
	virtual void          onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime) {}
	virtual void          onSearchNodeInfoNew     (CREF(LPRDORuntime) pRuntime) {}
	virtual PTR(TreeNode) createChildTreeNode     ();
};

// ----------------------------------------------------------------------------
// ---------- compareNodes
// ----------------------------------------------------------------------------
// ������� ��������� ������ ����� ��� ���������� ������ OPEN
// ----------------------------------------------------------------------------
inline bool compareNodes( const TreeNode* tn1, const TreeNode* tn2 )
{
	if ( fabs(tn1->m_costRest - tn2->m_costRest) > 0.0000001 ) {
		return ( tn1->m_costRest < tn2->m_costRest );
	} else {
		return ( tn1->m_number < tn2->m_number );
	}
}

} // namespace rdoRuntime

#endif // SEARCHTREE_H
