/*!
  \copyright (c) RDO-Team, 2009
  \file      rdo_logic_dptfree.inl
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      07.11.2009
  \brief     DPTFree
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
inline RDODPTFree::RDODPTFree(CREF(LPRDORuntime) pRuntime)
	: RDOLogicSimple(pRuntime, NULL)
{}

inline RDODPTFree::~RDODPTFree()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
