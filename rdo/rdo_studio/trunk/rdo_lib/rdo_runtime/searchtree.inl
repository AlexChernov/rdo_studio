/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      searchtree.inl
 * @author    ����� �������
 * @date      22.07.2011
 * @brief     ��������� IDPTSearchTraceStatistics
 * @indent    4T
 *********************************************************************************/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TreeRoot
// --------------------------------------------------------------------------------
inline TreeRoot::~TreeRoot()
{}

inline int TreeRoot::getNodesCound() const
{
	return m_nodesCount;
}

inline int TreeRoot::getNewNodeNumber()
{
	return ++m_nodesCount;
}

// --------------------------------------------------------------------------------
// -------------------- TreeRoot
// --------------------------------------------------------------------------------
inline void TreeNode::onSearchOpenNode(CREF(LPRDORuntime) pRuntime)
{}

inline void TreeNode::onSearchNodeInfoDeleted(CREF(LPRDORuntime) pRuntime)
{}

inline void TreeNode::onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime)
{}

inline void TreeNode::onSearchNodeInfoNew(CREF(LPRDORuntime) pRuntime)
{}

/******************************************************************************//**
 * @fn      compareNodes
 * @brief   ������� ��������� ������ ����� ��� ���������� ������ OPEN
 *********************************************************************************/
inline rbool compareNodes(const TreeNode* tn1, const TreeNode* tn2)
{
	if (fabs(tn1->m_costRest - tn2->m_costRest) > 0.0000001)
	{
		return (tn1->m_costRest < tn2->m_costRest);
	}
	else
	{
		return (tn1->m_number < tn2->m_number);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
