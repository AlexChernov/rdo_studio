/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array_def.h
 * author   : ������ ������
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */
#ifndef _RDO_ARRAY_DEF_H_
#define _RDO_ARRAY_DEF_H_

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (������ "rdo_array_def.h" ���� ��������� "rdo_array.h")
#endif

//
// �������� ���� ���� ��������� �� ����, ������
// #include "rdo_array.h"
//

// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
class RDOArrayType;

class RDOArrayValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOArrayValue(CREF(RDOArrayType)  type );
	RDOArrayValue(CREF(RDOArrayValue) value);
	~RDOArrayValue();

	CREF(RDOArrayType) type() const;

	void insertItem(CREF(RDOValue) pArray);
	Container::iterator m_containerBegin();
	Container::iterator m_containerEnd();
	CREF(RDOValue) extractItem(rsint num) const;
	//rbool comparDim(CREF(RDOArrayValue) a_value);

	CREF(RDOArrayValue) operator+ (CREF(RDOValue) rdovalue);

	tstring getAsString() const;

private:
	Container          m_Container;
	CPTR(RDOArrayType) m_arrayType;
};

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType, public RDORuntimeObject
{
public:
	typedef CREF(RDOType) ArrayType;

	RDOArrayType(PTR(RDORuntimeParent) parent, ArrayType pArrayType);

	ArrayType getArrayType() const;

private:
	ArrayType  m_pArrayType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ARRAY_DEF_H_
