/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_opr.inl
 * author   : ����� �������
 * date     : 05.11.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations(PTR(RDOSimulator) sim)
	: RDOLogicSimple(sim)
{}

RDOOperations::~RDOOperations()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
