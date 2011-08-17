/*
 * copyright: (c) RDO-Team, 2011
 * file     : rdo_value.h
 * author   : ������ ������
 * date     : 
 * brief    : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_VALUE_H_
#define _RDOPARSER_VALUE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/type/info.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

PREDECLARE_POINTER(RDOArrayValue);

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue: public RDOParserSrcInfo
{
public:
	explicit RDOValue(CREF(rsint)           value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(ruint)           value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(double)          value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(tstring)         value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(LPRDOArrayValue) pValue);

	explicit RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType);
	         RDOValue(CREF(RDOValue) value);
	         RDOValue(CREF(LPTypeInfo) pType);
	// ��� t_identificator �������� ������ ���, �� �� ���
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// �������������� ���
	         RDOValue();

	CREF(LPTypeInfo)              typeInfo() const;
	rdoRuntime::RDOType::TypeID     typeID() const;
	CREF(rdoRuntime::RDOValue)       value() const;
	CPTR(rdoRuntime::RDOValue) operator-> () const;
	CREF(LPRDOArrayValue)         getArray() const;

	void operator= (CREF(RDOValue) value);

	rbool defined () const;
	rbool constant() const;

	static RDOValue getIdentificator(CREF(tstring) identificator);

private:
	rdoRuntime::RDOValue m_value;
	LPRDOArrayValue      m_pArray;
	LPTypeInfo           m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_value.inl"

#endif //! _RDOPARSER_VALUE_H_
