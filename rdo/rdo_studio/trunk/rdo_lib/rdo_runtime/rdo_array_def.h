/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_array_def.h
 * @authors   ������ ������
 * @date      14.12.2009
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_ARRAY_DEF_H_
#define _LIB_RUNTIME_ARRAY_DEF_H_

#ifndef _LIB_RUNTIME_VALUE_H_
#error include "rdo_lib/rdo_runtime/rdo_value.h" first (������ "rdo_array_def.h" ���� ��������� "rdo_array.h")
#endif

//
// �������� ���� ���� ��������� �� ����, ������
// #include "rdo_lib/rdo_runtime/rdo_array.h"
//

// *********************************************************************** INCLUDES
#include <string>
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOArrayValue
// ********************************************************************************
PREDECLARE_POINTER(RDOArrayType);

class RDOArrayValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOArrayValue(CREF(LPRDOArrayType) pType);
	RDOArrayValue(CREF(RDOArrayValue)  value);
	~RDOArrayValue();

	CREF(LPRDOArrayType) type() const;

	void insertItem(CREF(RDOValue) pArray);
	Container::iterator containerBegin();
	Container::iterator containerEnd();
	void insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                         );
	CREF(RDOValue) operator[] (CREF(RDOValue) ind) const;

	ruint arraySize() const;

	tstring getAsString() const;
	void setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item);

private:
	Container       m_container;
	LPRDOArrayType  m_pArrayType;
};

// ********************************************************************************
// ******************** RDOArrayIterator
// ********************************************************************************
class RDOArrayIterator
{
public:
	typedef RDOArrayValue::Container::iterator Iterator;

	RDOArrayIterator(CREF(RDOArrayIterator) iterator);
	RDOArrayIterator(CREF(Iterator)         iterator);

	Iterator getIterator() const;
	Iterator operator+  (rsint num);
	Iterator operator-  (rsint num);
	rbool    operator== (CREF(RDOArrayIterator) iterator) const;

	RDOValue getValue() const;

private:
	Iterator m_iterator;
};

// ********************************************************************************
// ******************** RDOArrayType
// ********************************************************************************
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	typedef LPRDOType LPItemType;

	LPItemType getItemType() const;

private:
	RDOArrayType(CREF(LPItemType) pItemType);

	LPItemType m_pItemType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ARRAY_DEF_H_
