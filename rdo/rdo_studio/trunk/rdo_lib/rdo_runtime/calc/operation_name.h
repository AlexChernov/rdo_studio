/*
 * copyright: (c) RDO-Team, 2011
 * filename : operation_name.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_OPERATION_NAME_H_
#define _RDOCALC_OPERATION_NAME_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- OperatorName
// ----------------------------------------------------------------------------
template <typename ret_type>
class OperatorName
{
public:
	typedef ret_type (RDOValue::*opr_type)(CREF(RDOValue) rdovalue) const;

	static tstring name(CREF(opr_type) pOperator);

private:
	struct OprItem
	{
		opr_type m_pOperator;
		tstring  m_name;

		OprItem(CREF(opr_type) pOperator, CREF(tstring) name);

		rbool operator== (CREF(opr_type) pOperator) const;
	};
	typedef std::list<OprItem> NameList;

	static CREF(NameList) getList();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/operation_name.inl"

#endif //! _RDOCALC_OPERATION_NAME_H_
