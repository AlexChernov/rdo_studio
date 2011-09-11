/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_matrix_def.h
  \author    ������ ������
  \date      01.10.2010
  \brief     �������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MATRIX_DEF_H_
#define _LIB_RUNTIME_MATRIX_DEF_H_

#ifndef _LIB_RUNTIME_VALUE_H_
#error include "rdo_lib/rdo_runtime/rdo_value.h" first (������ "rdo_matrix_def.h" ���� ��������� "rdo_matrix.h")
#endif

//
// �������� ���� ���� ��������� �� ����, ������
// #include "rdo_lib/rdo_runtime/rdo_matrix.h"
//

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOMatrixType);

/*!
  \class     RDOMatrixValue
  \brief     ������� �������
*/
class RDOMatrixValue
{
public:
	typedef std::vector<RDOValue> Container;

	RDOMatrixValue(CREF(LPRDOMatrixType) pType);
	RDOMatrixValue(CREF(RDOMatrixValue)  value);
	~RDOMatrixValue();

	CREF(LPRDOMatrixType) type() const;

	void insertItem(CREF(RDOValue) pMatrix);
	Container::iterator containerBegin();
	Container::iterator containerEnd();
	void insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst);
	void  eraseItems(Container::iterator itrFst, Container::iterator itrLst                         );
	CREF(RDOValue) operator[] (CREF(RDOValue) ind);

	tstring getAsString() const;

private:
	Container       m_container;
	LPRDOMatrixType m_pMatrixType;
};

/*!
  \class     RDOMatrixIterator
  \brief     �������� �������
*/
class RDOMatrixIterator
{
public:
	typedef RDOMatrixValue::Container::iterator Iterator;

	RDOMatrixIterator(CREF(RDOMatrixIterator) iterator);
	RDOMatrixIterator(CREF(Iterator)         iterator);

	Iterator getIterator() const;
	Iterator operator+  (rsint num);
	Iterator operator-  (rsint num);
	rbool    operator== (CREF(RDOMatrixIterator) iterator) const;

	RDOValue getValue() const;

private:
	Iterator m_iterator;
};

/*!
  \class     RDOMatrixType
  \brief     ��������� ���
*/
class RDOMatrixType: public RDOType
{
DECLARE_FACTORY(RDOMatrixType);
public:
	typedef LPRDOType LPMatrixType;

	LPMatrixType getMatrixType() const;

private:
	RDOMatrixType(CREF(LPMatrixType) pMatrixType);

	LPMatrixType  m_pMatrixType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_MATRIX_DEF_H_
