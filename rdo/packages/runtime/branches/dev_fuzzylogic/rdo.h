/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ����������� ���������� IBaseOperation
  \details   ����� ���� �������������� ������� �������� �������� ���� ��������� ���
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RDO_H_
#define _LIB_RUNTIME_RDO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/// @todo ���� �� ������� IBaseOperation ������� ��� ���� ��������� �����

/*!
  \interface IBaseOperation
  \brief     ��������� ��� ���������, ��������� � ������ ��������
*/
class IBaseOperation
{
public:
	//! �����c ��������� ��������
	enum BOResult
	{
		BOR_cant_run       = 0, //!< �������� �� ����� ���� ��������
		BOR_planned_and_run,    //!< �������� ������������� � �����������
		BOR_must_continue  ,    //!< �������� ����� ���� ����������
		BOR_done			    //!< �������� ���������
	};

	/*!
	  \fn      virtual void onStart(CREF(rdo::runtime::LPRDORuntime) pRuntime)
	  \brief   ������
	  \details ���������� ����� ������� ������� � ����� ������������� ��������
	           ��������, ������� DPT-some ���������� � false �� true
	           ������������ ��� GENERATE, ����� ������ ����� ������� ������� �������
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual void onStart(CREF(rdo::runtime::LPRDORuntime) pRuntime) = 0;

	/*!
	  \fn      virtual void onStop(CREF(rdo::runtime::LPRDORuntime) pRuntime)
	  \brief   ���������
	  \details ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual void onStop(CREF(rdo::runtime::LPRDORuntime) pRuntime) = 0;

	/*!
	  \fn      virtual rbool onCheckCondition(CREF(rdo::runtime::LPRDORuntime) pRuntime)
	  \brief   ��������
	  \details ���������� ��� �������� ������������ ��������
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual rbool onCheckCondition(CREF(rdo::runtime::LPRDORuntime) pRuntime) = 0;

	/*!
	  \fn      virtual BOResult onDoOperation(CREF(rdo::runtime::LPRDORuntime) pRuntime)
	  \brief   ����������
	  \details ���������� ��� ���������� ������/��������
	  \return  BOResult ��������� ���������� ��������
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual BOResult onDoOperation(CREF(rdo::runtime::LPRDORuntime) pRuntime) = 0;

	/*!
	  \fn      virtual void onMakePlaned(CREF(rdo::runtime::LPRDORuntime) pRuntime, PTR(void) pParam)
	  \brief   ������������
	  \details ���������� ��� ��������������� � �������� �������: event, operation_end, keyboard_end
	           ����� �� ��������������, ��������, ��� rule
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual void onMakePlaned(CREF(rdo::runtime::LPRDORuntime) pRuntime, PTR(void) pParam) = 0;

	/*!
	  \fn      virtual BOResult onContinue(CREF(rdo::runtime::LPRDORuntime) pRuntime)
	  \brief   �����������
	  \details ���������� ��� ����������� ������ ��������, ��������, DPT search
	  \return  BOResult ��������� ���������� ��������
	  \param   pRuntime ����������� ������ �� ��������� �� �������
	*/
	virtual BOResult onContinue(CREF(rdo::runtime::LPRDORuntime) pRuntime) = 0;
};

#define DECLARE_IBaseOperation                                                                      \
	virtual void     onStart         (CREF(rdo::runtime::LPRDORuntime) pRuntime);                   \
	virtual void     onStop          (CREF(rdo::runtime::LPRDORuntime) pRuntime);                   \
	virtual rbool    onCheckCondition(CREF(rdo::runtime::LPRDORuntime) pRuntime);                   \
	virtual BOResult onDoOperation   (CREF(rdo::runtime::LPRDORuntime) pRuntime);					    \
	virtual void     onMakePlaned    (CREF(rdo::runtime::LPRDORuntime) pRuntime, PTR(void) pParam); \
	virtual BOResult onContinue      (CREF(rdo::runtime::LPRDORuntime) pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);

/*!
  \typedef BaseOperationList
  \brief   ��������� ����� ���������� �� IBaseOperation
*/
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif // _LIB_RUNTIME_RDO_H_
