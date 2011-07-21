/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      searchtrace.inl
 * @author    ����� �������
 * @date      22.07.2011
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDODPTSearchTrace
// ********************************************************************************
RDODPTSearchTrace::RDODPTSearchTrace(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent)
	: RDODPTSearch      (pRuntime, parent)
	, RDOTraceableObject(false           )
	, calc_cnt          (0               )
	, calc_res_found_cnt(0               )
{
	traceFlag = DPT_no_trace;
}

// ********************************************************************************
// ******************** TreeRootTrace
// ********************************************************************************
TreeRootTrace::TreeRootTrace(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP)
	: TreeRoot(pRuntime, pDP)
{}

// ********************************************************************************
// ******************** TreeNodeTrace
// ********************************************************************************
TreeNodeTrace::TreeNodeTrace(CREF(LPRDORuntime) pRuntime, TreeNode* i_parent, TreeRoot* i_root, LPIDPTSearchActivity i_activity, double cost, int cnt)
	: TreeNode(pRuntime, i_parent, i_root, i_activity, cost, cnt)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
