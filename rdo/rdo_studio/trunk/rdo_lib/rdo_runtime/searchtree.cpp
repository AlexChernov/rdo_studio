/******************************************************************************//**
 * @copyright (c) RDO-Team, unknown
 * @file      searchtree.cpp
 * @authors   unknown
 * @date      unknown
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdodebug.h"
#include "rdo_lib/rdo_runtime/searchtree.h"
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** TreeRoot
// ********************************************************************************
TreeRoot::TreeRoot(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP)
	: m_dp                (pDP     )
	, m_rootNode          (NULL    )
	, m_targetNode        (NULL    )
	, m_theRealSimulator  (pRuntime)
	, m_nodesCount        (0       )
	, m_nodesInGraphCount (1       ) //! ���� ��������� �������
	, m_expandedNodesCount(0       )
	, m_fullNodesCount    (0       )
	, m_sizeof_dpt        (0       )
{
	::GetSystemTime(&m_systime_begin);
}

// ********************************************************************************
// ******************** TreeNode
// ********************************************************************************
TreeNode::TreeNode(CREF(LPRDORuntime) pRuntime, PTR(TreeNode) pParent, PTR(TreeRoot) pRoot, LPIDPTSearchActivity pActivity, double cost, int cnt)
	: m_pRuntime     (pRuntime )
	, m_pChildRuntime(NULL     )
	, m_newCostPath  (0        )
	, m_newCostRest  (0        )
	, m_newCostRule  (0        )
	, m_parent       (pParent  )
	, m_root         (pRoot    )
	, m_activity     (pActivity)
	, m_costRule     (0        )
	, m_costPath     (cost     )
	, m_costRest     (0        )
	, m_number       (cnt      )
{
	m_pRuntime->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
	for(std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); i++)
	{
		delete (*i);
		m_root->m_sizeof_dpt -= sizeof(TreeNode);
	}
	m_root->m_sizeof_dpt -= m_pRuntime->getSizeofSim();
	m_pRuntime = NULL;
}

void TreeNode::ExpandChildren()
{
	m_root->m_sizeof_dpt -= (m_pRuntime->getSizeofSim() + sizeof(TreeNode)) * m_children.size();
	rdo::deleteAllObjects(m_children);

	// ������ ����������
	onSearchOpenNode(m_root->m_theRealSimulator);

	// ��������� �� �������� �������
	/// @todo ��������, ���� ��������� ��� ������� � ������ OPEN
	if (m_root->m_dp->TermCondition(m_pRuntime))
	{
		m_root->m_targetNode = this;
#ifdef _DEBUG
		m_pRuntime->showResources(m_number);
#endif
		return;
	}

	// ������ ��� ����������
	m_root->m_expandedNodesCount++;

	int s = m_root->m_dp->m_activityList.size();

	// ������ �� ���� ����������� ����� �����
	for (RDODPTSearch::ActivityList::iterator i = m_root->m_dp->m_activityList.begin(); i != m_root->m_dp->m_activityList.end(); ++i)
	{
		m_currAct       = *i;
		m_pChildRuntime = m_pRuntime->clone();
#ifdef _DEBUG
		if (m_pChildRuntime->checkState())
		{
			TRACE1("���������, node = %d\n", m_number);
		}
#endif
		m_root->m_sizeof_dpt += m_pChildRuntime->getSizeofSim();
		m_currAct->rule()->onBeforeChoiceFrom(m_pChildRuntime);
		if (!m_currAct->rule()->choiceFrom(m_pChildRuntime))
		{
			// �� ������ Choice from, ������� ��������� � ��������� � ������ ����������.
			/// @todo � ����� ������� ���������, ���� �� �� ���������� ?
			// ����� ����� ��������, ���� ��� ���������� ���������� �������� �����
			// ���������� calc, �� ������� ����� ����������� �� ������ ��� �������
			// �������, ������� ����� �������� ��������� ��������
			// ���� ����� ������� �������������� � ������� ���.
			m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
			m_pChildRuntime = NULL;
		}
		else
		{
			// ������ ��� ����������
			m_root->m_fullNodesCount++;
			// ��������� ��������� ���������� ������� (value before)
			if (m_currAct->valueTime() == IDPTSearchActivity::vt_before)
			{
				m_newCostRule = m_currAct->cost(m_pChildRuntime);
			}
			// ��������� ���� ������� (�������� �������)
			m_currAct->rule()->onBeforeRule(m_pChildRuntime);
			m_currAct->rule()->convertRule(m_pChildRuntime);
			m_currAct->rule()->onAfterRule(m_pChildRuntime, true);

			// ��������� ��������� ���������� ������� (value after)
			if (m_currAct->valueTime() == IDPTSearchActivity::vt_after)
			{
				m_newCostRule = m_currAct->cost(m_pChildRuntime);
			}
			// ��������� ��������� ���� �� ������� �������
			m_newCostPath = m_costPath + m_newCostRule;
			// ��������� ��������� ���� �� ���� � ������ ������
			// ������ �� ����� �������� � ������ ��������������� ������ OPEN
			m_newCostRest = m_newCostPath + m_root->m_dp->EvaluateBy(m_pChildRuntime);

			// ���� �� ���������� ������� (���������� ��� ����������)
			// �������� ����� ��������
			if (m_root->m_dp->NeedCompareTops())
			{
				TreeNode* loser = NULL;
				NodeFoundInfo res = m_root->m_rootNode->CheckIfExistBetter(m_pChildRuntime, m_newCostPath, &loser);
				if (res == nfi_found_better)
				{
					// � ����� ���� ����� ������ �������, �.�. ������� ����������
					// ���������� ���� (������) �� ������ � ���� �� ���������.
					// ���� ������������� �� ����.
					// ������ ����������� ��������� �������.
					onSearchNodeInfoDeleted(m_root->m_theRealSimulator);
					m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
					m_pChildRuntime = NULL;
					// ��������� � ��������� ����������
					continue;
				}
				else if (res == nfi_found_loser)
				{
#ifdef _DEBUG
					if (m_number == 294)
					{
						TRACE1("loser->m_number = %d\n", loser->m_number);
						TRACE1("loser->m_parent->m_number = %d\n", loser->m_parent->m_number);
						TRACE1("loser->m_parent->m_children.size() = %d\n", loser->m_parent->m_children.size());
						loser->m_pRuntime->showResources(loser->m_number);
					}
#endif
					// ����� ��������
					// � ����� ������� ������� (loser) � ����� ������ ���������� ����,
					// �.�. ������� ���������� ���������� ����� (�������).
					// ���������� ����������� ����
					// ������� �������� ��� ����� �� ������� �������� loser->parent
					loser->m_parent->m_children.erase(std::find(loser->m_parent->m_children.begin(), loser->m_parent->m_children.end(), loser));
#ifdef _DEBUG
					if (m_number == 294)
					{
						TRACE1("loser->m_parent->m_children.size() after erase = %d\n", loser->m_parent->m_children.size());
					}
#endif
					// ������ ������������� ��������� ���� ������� � ���� � ��������
					/// @todo: � ��� �� ��� ������, �.�. costPath � ������ �������,
					// ������������ ��� costPath + newCostRule, ����� �� ��������
					// ��-�� newCostRule ��-�� ������ ���������� ���������� ������,
					// �.�. ������ ���� �������� loser->costRule (��. ����), � �����
					// �������� loser->ReCostSubTree( costPath )
					loser->ReCostSubTree(m_costPath);
					// ������ �������� �� ������� �������
					loser->m_parent   = this;
					// ����������� �� ����� �����
					loser->m_number   = m_root->getNewNodeNumber();
					// ��������� ���������
					loser->m_costPath = m_newCostPath;
					loser->m_costRest = m_newCostRest;
					loser->m_costRule = m_newCostRule;
					// ������ ��������� �� ����� ����������
					loser->m_activity = m_currAct;
					// ����, �� ������ ��� ����������� ������ ����������
					loser->m_currAct  = m_currAct;
					// ������� ����������� �� ����� ��������
					loser->onSearchNodeInfoReplaced(m_root->m_theRealSimulator);
					// ��������� � ������ �������� �������
					m_children.push_back(loser);
					m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
					m_pChildRuntime = NULL;
#ifdef _DEBUG
					if (m_number == 294)
					{
						loser->m_pRuntime->showResources(loser->m_number);
					}
#endif
					// ��������� � ��������� ����������
					continue;
				}
			}

			// �� ����� � ����� ������ ���������
			// ��� ���� ���������. �������� ��� ����� ����� �������
			TreeNode* tn = createChildTreeNode();
			m_root->m_nodesInGraphCount++;
			tn->m_costPath = m_newCostPath;
			tn->m_costRest = m_newCostRest;
			tn->m_costRule = m_newCostRule;
			tn->m_currAct  = m_currAct;
			// ������� �����������
			tn->onSearchNodeInfoNew(m_root->m_theRealSimulator);
			// ������� � ������ �������� �������
			m_children.push_back(tn);
/*
			if(!root->m_dp->TermCondition(pChildRuntime))
				root->OPEN.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// ������� � ������ OPEN
			m_root->m_OPEN.push_back(tn);
		}
	}
	// ������ ������� ������� �� ������ OPEN
	m_root->m_OPEN.erase(std::find(m_root->m_OPEN.begin(), m_root->m_OPEN.end(), this));

	// ����������� ��� ������� � ������ OPEN
	std::sort(m_root->m_OPEN.begin(), m_root->m_OPEN.end(), compareNodes);
}

TreeNode::NodeFoundInfo TreeNode::CheckIfExistBetter(CREF(LPRDORuntime) pChildRuntime, double useCost, TreeNode** loser)
{
	ASSERT(pChildRuntime);

	if (pChildRuntime->equal(m_pRuntime))
	{
		if (m_costPath <= useCost)
		{
			return nfi_found_better;
		}
		else
		{
			*loser = this;
			return nfi_found_loser;
		}
	}

	for (std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); ++i)
	{
		NodeFoundInfo res = (*i)->CheckIfExistBetter(pChildRuntime, useCost, loser);
		if (res != nfi_notfound)
			return res;
	}

	return nfi_notfound;
}

void TreeNode::ReCostSubTree(double cost)
{
	m_costPath = cost + m_costRule;

	for (std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); ++i)
	{
		(*i)->ReCostSubTree(m_costPath);
	}
}

TreeNode* TreeNode::createChildTreeNode()
{
	m_root->m_sizeof_dpt += sizeof(TreeNode);
	return new TreeNode(m_pChildRuntime, this, m_root, m_currAct, m_costPath, m_root->getNewNodeNumber());
}

CLOSE_RDO_RUNTIME_NAMESPACE
