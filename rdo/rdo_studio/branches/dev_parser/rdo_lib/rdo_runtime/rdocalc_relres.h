/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc_relres.h
 * author   : �������� ����, ������ ������
 * date     : 28.12.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_RELRES_H_
#define _RDOCALC_RELRES_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
enum EqualType
{
	ET_UNDEFINED,
	ET_NOCHANGE,
	ET_EQUAL,
	ET_PLUS,
	ET_MINUS,
	ET_MULTIPLY,
	ET_DIVIDE,
	ET_INCR,
	ET_DECR
};

template <EqualType equalType>
CALC(RDOSetRelParamCalc)
{
public:
	RDOSetRelParamCalc(PTR(RDORuntimeParent) parent, int relNumb, int parNumb, CREF(LPRDOCalc) pCalc)
		: RDOCalc  (parent )
		, m_relNumb(relNumb)
		, m_parNumb(parNumb)
		, m_pCalc  (pCalc  )
	{
		m_value = 1;
		if (m_pCalc)
			setSrcInfo(m_pCalc->src_info());
	}

protected:
	int        m_relNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_EQUAL>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->setResParamVal(resID, m_parNumb, m_pCalc->calcValue(runtime));
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_PLUS>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) += m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_MINUS>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) -= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_MULTIPLY>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) *= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_DIVIDE>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) /= m_pCalc->calcValue(runtime);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_INCR>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) += RDOValue(1);
	return m_value;
}

template <>
inline REF(RDOValue) RDOSetRelParamCalc<ET_DECR>::doCalc(PTR(RDORuntime) runtime)
{
	ruint resID = runtime->getCurrentActivity()->getResByRelRes(m_relNumb);
	runtime->getResParamValRaw(resID, m_parNumb) -= RDOValue(1);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_RELRES_H_
