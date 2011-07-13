/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsome.inl
 * author   : ����� �������
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
inline RDODPTSome::RDODPTSome(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent)
	: RDOLogicSimple(pRuntime, parent)
{
	pRuntime->getFreeDPTId();
}

inline RDODPTSome::~RDODPTSome()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
