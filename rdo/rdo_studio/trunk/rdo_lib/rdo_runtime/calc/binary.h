/*
 * copyright: (c) RDO-Team, 2011
 * filename : binary.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_BINARY_H_
#define _RDOCALC_BINARY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const>
class RDOCalcBinary: public RDOCalc
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator> >;
public:
	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;

private:
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/binary.inl"

#endif //! _RDOCALC_BINARY_H_
