/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_dptsearch_activity.h
 * author   : ������ ������
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_DPTSEARCH_ACTIVITY_H_
#define _RDO_DPTSEARCH_ACTIVITY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_rule_interface.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity_interface.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchActivity: public IDPTSearchActivity, public RDOTraceableObject
{
DEFINE_IFACTORY(RDODPTSearchActivity);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOTraceableObject)
QUERY_INTERFACE(IDPTSearchActivity)
QUERY_INTERFACE_END

private:
	RDODPTSearchActivity(CREF(LPIRule) rule, ValueTime valueTime, CREF(LPRDOCalc) pCost);

	LPIRule                        m_rule;
	LPRDOCalc                      m_pCost;
	IDPTSearchActivity::ValueTime  m_valueTime;

	DECLARE_IDPTSearchActivity;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_DPTSEARCH_ACTIVITY_H_
