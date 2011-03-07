/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_activity_i.h
 * author   : ������ ������
 * date     : 01.08.2009
 * bref     : ��������� IActivity
 * indent   : 4T
 */

#ifndef _RDO_ACTIVITY_I_H_
#define _RDO_ACTIVITY_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

class IActivity
{
public:
	virtual void addParamCalc  (CREF(rdoRuntime::LPRDOCalc) pCalc) = 0;
	virtual int  getResByRelRes(ruint rel_res_id) const            = 0;
	virtual void setRelRes     (ruint rel_res_id, ruint res_id)    = 0;
};
#define DECLARE_IActivity \
	virtual void addParamCalc  (CREF(rdoRuntime::LPRDOCalc) pCalc); \
	virtual int  getResByRelRes(ruint rel_res_id) const;            \
	virtual void setRelRes     (ruint rel_res_id, ruint res_id);

class IActivityTrace
{
public:
	virtual tstring traceResourcesList       (char prefix, PTR(rdoRuntime::RDOSimulatorTrace) sim)             = 0;
	virtual tstring traceResourcesListNumbers(PTR(rdoRuntime::RDOSimulatorTrace) sim, rbool show_create_index) = 0;
};
#define DECLARE_IActivityTrace \
	virtual tstring traceResourcesList       (char prefix, PTR(rdoRuntime::RDOSimulatorTrace) sim); \
	virtual tstring traceResourcesListNumbers(PTR(rdoRuntime::RDOSimulatorTrace) sim, rbool show_create_index);

class IActivityPatternTrace
{
public:
	virtual CREF(tstring) tracePatternId() const = 0;
};
#define DECLARE_IActivityPatternTrace \
	virtual CREF(tstring) tracePatternId() const;

#endif //! _RDO_ACTIVITY_I_H_
