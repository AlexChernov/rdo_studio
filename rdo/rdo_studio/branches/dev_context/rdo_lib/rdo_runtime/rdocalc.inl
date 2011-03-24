/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.inl
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 11.07.2009
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
inline RDOCalcConst::RDOCalcConst(CREF(RDOValue) value)
{
	m_value = value;
}

inline CREF(RDOValue) RDOCalcConst::getValue() const
{
	return m_value;
}

inline REF(RDOValue) RDOCalcConst::doCalc(PTR(RDORuntime) runtime)
{
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
