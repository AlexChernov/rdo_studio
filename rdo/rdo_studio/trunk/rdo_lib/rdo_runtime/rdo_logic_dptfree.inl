/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptfree.inl
 * @authors   ����� �������
 * @date      07.11.09
 * @brief     
 * @indent    4T
 */

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDODPTFree
// ********************************************************************************
inline RDODPTFree::RDODPTFree(CREF(LPRDORuntime) pRuntime)
	: RDOLogicSimple(pRuntime, NULL)
{}

inline RDODPTFree::~RDODPTFree()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
