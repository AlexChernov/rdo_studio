/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_exception.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     ����������� ����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_EXCEPTION_H_
#define _LIB_RUNTIME_EXCEPTION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOException
  \brief     ������
*/
class RDOException
{
public:
	RDOException(CREF(tstring) message);
	virtual tstring getType() const = 0;
	
	CREF(tstring) message() const;

private:
	tstring m_message;
};

/*!
  \class     RDORuntimeException
  \brief     ������ ������� ����������
*/
class RDORuntimeException: public RDOException
{
public:
	RDORuntimeException(CREF(tstring) message);
	virtual tstring getType() const;
};

/*!
  \class     RDOTypeException
  \brief     ������ � ����
*/
class RDOTypeException: public RDORuntimeException
{
public:
	RDOTypeException(CREF(tstring) message = _T(""));
	virtual tstring getType() const;
};

/*!
  \class     RDOValueException
  \brief     ������ � ��������
*/
class RDOValueException: public RDORuntimeException
{
public:
	RDOValueException(CREF(tstring) message = _T(""));
	virtual tstring getType() const;
};

/*!
  \class     RDOValueException
  \brief     ������ - �������� �� ����������
*/
class RDOUndefinedException: public RDORuntimeException
{
public:
	RDOUndefinedException();
	virtual tstring getType() const;
};

/*!
  \class     RDOInternalException
  \brief     ���������� ������
*/
class RDOInternalException: public RDORuntimeException
{
public:
	RDOInternalException(CREF(tstring) message);
	virtual tstring getType() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_exception.inl"

#endif // _LIB_RUNTIME_EXCEPTION_H_
