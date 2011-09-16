/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      07.06.2011
  \brief     ���� �������� � rdoRuntime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_H_
#define _LIB_RUNTIME_RES_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
class RDOResource;
class RDOPROCTransact;

//! @todo ������� ������ \ref RDOResourceType, \ref RDOResourceTypeTransact � \ref RDOResourceTypeProccess ����������

/*!
  \class   RDOResourceType
  \brief   ��� �������� ��� "�������" �������� ���
  \details ������� �������, ������� ����� ���� ���������� ����������� �
 * ��������, �� �� ����� �������������� � ���������
*/
OBJECT(RDOResourceType)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
friend class RDOCalcCreateResource;
friend class RDOPROCGenerate;
private:
	/*!
	  \brief �����������
	  \param number - ������������� �������������
	*/
	RDOResourceType(ruint number);
	virtual ~RDOResourceType();

	DECLARE_IResourceType;
};

/*!
  \class   RDOResourceTypeTransact
  \brief   ��� �������� ��� "����������" �������� ���
  \details ������� �������, ������� ����� ���� ���������� ����������� � 
  \details ��������, � ����� ����������� ��������� � ���������
*/
OBJECT(RDOResourceTypeTransact)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
private:
	/*!
	  \brief �����������
	  \param number - ������������� �������������
	*/
	RDOResourceTypeTransact(ruint number);
	virtual ~RDOResourceTypeTransact();

	DECLARE_IResourceType;
};

/*!
  \class   RDOResourceTypeProccess
  \brief   ��� �������� ��� "�����������" �������� ���
  \details ������� �������, ������� ����� ���� ���������� ����������� � 
  \details ��������, � ����� ����������� ����������� � ���������
*/
OBJECT(RDOResourceTypeProccess)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
private:
	/*!
	  \brief �����������
	  \param number - ������������� �������������
	*/
	RDOResourceTypeProccess(ruint number);
	virtual ~RDOResourceTypeProccess();

	DECLARE_IResourceType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_H_
