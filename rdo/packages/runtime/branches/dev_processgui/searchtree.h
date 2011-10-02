/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtree.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ���� ��������� DPTSearch ��� ������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SEARCH_TREE_H_
#define _LIB_RUNTIME_SEARCH_TREE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/date_time/posix_time/posix_time.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptsearch.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNode;

/*!
  \class   TreeRoot
  \brief   ������ ������ DPT
  \details � ������� createRootTreeNode ������� �������� ����-������
*/
class TreeRoot
{
public:
	virtual ~TreeRoot();

	virtual void createRootTreeNode(CREF(LPRDORuntime) pRuntime) = 0;

	std::vector<TreeNode*>		m_OPEN;
	RDODPTSearch*				m_dp;
	TreeNode*					m_rootNode;
	TreeNode*					m_targetNode;
	LPRDORuntime				m_theRealSimulator;
	int							m_nodesInGraphCount;
	int							m_expandedNodesCount;
	int							m_fullNodesCount;
	boost::posix_time::ptime	m_ptime;
	unsigned int				m_sizeof_dpt;

	int getNodesCound() const;
	int getNewNodeNumber();

protected:
	TreeRoot(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP);

private:
	int m_nodesCount;
};

/*!
  \class   TreeNode
  \brief   ���� ����� DPT
*/
class TreeNode
{
public:
	virtual ~TreeNode();

	LPRDORuntime           m_pRuntime;
	std::vector<TreeNode*> m_children;
	TreeNode*              m_parent;
	TreeRoot*              m_root;
	LPIDPTSearchActivity   m_activity; // ���������� (currAct), ������� �������� ������ ��� �������� this
	double                 m_costRule;
	double                 m_costPath;
	double                 m_costRest;
	int                    m_number; // ����� ����

	void ExpandChildren();

	/*!
	  \class   NodeFoundInfo
	  \brief   ������ ��������� �������
	*/
	enum NodeFoundInfo
	{
		nfi_notfound     = 0,
		nfi_found_better = 1,
		nfi_found_loser  = 2
	};

	/// @todo ����������������� �������
	// return 0 - no such simulator, 1 - exist better, 2 - exist not better
	NodeFoundInfo CheckIfExistBetter(CREF(LPRDORuntime) pChildRuntime, double useCost, TreeNode** better );
	void ReCostSubTree(double cost);

protected:
	TreeNode(CREF(LPRDORuntime) pRuntime, PTR(TreeNode) pParent, PTR(TreeRoot) pRoot, LPIDPTSearchActivity pActivity, double cost, int cnt);

	LPIDPTSearchActivity  m_currAct; // ������� �������� ��������� ��������� ����������
	LPRDORuntime          m_pChildRuntime;

	double m_newCostPath;
	double m_newCostRest;
	double m_newCostRule;

	virtual void          onSearchOpenNode        (CREF(LPRDORuntime) pRuntime);
	virtual void          onSearchNodeInfoDeleted (CREF(LPRDORuntime) pRuntime);
	virtual void          onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime);
	virtual void          onSearchNodeInfoNew     (CREF(LPRDORuntime) pRuntime);
	virtual PTR(TreeNode) createChildTreeNode     ();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/searchtree.inl"

#endif // _LIB_RUNTIME_SEARCH_TREE_H_
