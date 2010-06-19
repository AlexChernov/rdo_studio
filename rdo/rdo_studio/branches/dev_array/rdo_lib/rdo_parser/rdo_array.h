/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_ARRAY_H_
#define _RDOPARSER_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public rdo::smart_ptr_counter_reference, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOArrayType);
friend class RDOArrayValue;

public:
	CREF(LPRDOType) arrayType() const;
	void            dinamicItemCast(CREF(RDOValue) value);

private:
	RDOArrayType         (CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info, rbool flag);
	virtual ~RDOArrayType();

	PTR(rdoRuntime::RDOArrayType) __array() const 
	{ 
		return static_cast<PTR(rdoRuntime::RDOArrayType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type));
	}

	CREF(LPRDOType) getFirstType();

	LPRDOType m_pType;
	rbool     plc_call;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(RDOArrayType)

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayValue
{
	DECLARE_FACTORY(RDOArrayValue);
public:
	RDOArrayValue         (LPRDOArrayType pArrayType);

	void insert_array(CREF(RDOValue) pArray);
	CREF(LPRDOArrayType) getArrayType() const;
	rdoRuntime::RDOValue getRArray   () const;

private:
	typedef std::vector<RDOValue> ArrayValue;

	virtual ~RDOArrayValue();

	ArrayValue         m_arrayValue;
	LPRDOArrayType     m_arrayType;
};
DECLARE_POINTER(RDOArrayValue);


CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ARRAY_H_