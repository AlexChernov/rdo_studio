/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_H_
#define _RDOPOKAZ_H_

// =========================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdotrace_interface.h"
#include "rdo_lib/rdo_runtime/rdopokaz_interface.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOPokazTrace, public IName
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPokazTrace)
	QUERY_INTERFACE(IName)
QUERY_INTERFACE_END

protected:
	RDOPMDPokaz( RDORuntime* sim, const std::string& name, bool trace );
	virtual ~RDOPMDPokaz() {}
	DECLARE_IName;

private:
	std::string m_name;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz, public IPokaz, public IModelStructure, public INotify
{
DEFINE_IFACTORY(RDOPMDWatchPar);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz)
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(INotify)
QUERY_INTERFACE_END

private:
	RDOPMDWatchPar( RDORuntime* sim, const std::string& name, bool trace, const std::string& resName, const std::string& parName, int resNumber, int parNumber );

	int       m_resNumber;
	int       m_parNumber;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	double    m_timeErase;

	DECLARE_INotify;
	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz, public IPokaz, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchState);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz)
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
QUERY_INTERFACE_END

private:
	RDOPMDWatchState( RDORuntime* sim, const std::string& name, bool trace, CREF(LPRDOCalc) pLogic );

	LPRDOCalc m_pLogicCalc;

	int       m_watchNumber;
	bool      m_currValue;
	double    m_sum;
	double    m_sumSqr;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz, public IPokaz, public IPokazWatchQuant, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchQuant);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz)
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(IPokazWatchQuant)
	QUERY_INTERFACE(IModelStructure)
QUERY_INTERFACE_END

private:
	RDOPMDWatchQuant( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );

	LPRDOCalc m_pLogicCalc;
	int       m_rtp_id;

	int       m_watchNumber;
	int       m_currValue;
	double    m_sum;
	double    m_sumSqr;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazWatchQuant;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz, public IPokaz, public IPokazWatchValue, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz)
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(IPokazWatchValue)
	QUERY_INTERFACE(IModelStructure)
QUERY_INTERFACE_END

private:
	RDOPMDWatchValue( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );

	LPRDOCalc m_pLogicCalc;
	LPRDOCalc m_pArithmCalc;
	int       m_rtp_id;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazWatchValue;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz, public IPokaz, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDGetValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz)
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
QUERY_INTERFACE_END

private:
	RDOPMDGetValue( RDORuntime* sim, const std::string& name, CREF(LPRDOCalc) pArithm );

	LPRDOCalc m_pArithmCalc;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOPOKAZ_H_
