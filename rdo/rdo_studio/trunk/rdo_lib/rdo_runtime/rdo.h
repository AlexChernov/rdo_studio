/******************************************************************************//**
 * @copyright (c) RDO-Team, 2006
 * @file      rdo.h
 * @author    ������ ������
 * @date      11.06.2006
 * @brief     ����������� ���������� IBaseOperation
 * @details   ����� ���� �������������� ������� �������� �������� ���� ��������� ���
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_RDO_H_
#define _LIB_RUNTIME_RDO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

/// @todo ���� �� ������� IBaseOperation ������� ��� ���� ��������� �����

/******************************************************************************//**
 * @interface IBaseOperation
 * @brief     ��������� ��� ���������, ��������� � ������ ��������
 *********************************************************************************/
class IBaseOperation
{
public:
	/**
	 * @enum  BOResult
	 * @brief �����c ��������� ��������
	 * @var   BOR_cant_run        �������� �� ����� ���� ��������
	 * @var   BOR_planned_and_run �������� ������������� � �����������
	 * @var   BOR_must_continue   �������� ����� ���� ����������
	 * @var   BOR_done            �������� ���������
	 */
	enum BOResult
	{
		BOR_cant_run       = 0,
		BOR_planned_and_run,
		BOR_must_continue  ,
		BOR_done
	};

	/**
	 * @fn      virtual void onStart(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	 * @brief   ������
	 * @details ���������� ����� ������� ������� � ����� ������������� ��������
	 * ��������, ������� DPT-some ���������� � false �� true
	 * ������������ ��� GENERATE, ����� ������ ����� ������� ������� �������
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual void onStart(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	/**
	 * @fn      virtual void onStop(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	 * @brief   ���������
	 * @details ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual void onStop(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	/**
	 * @fn      virtual rbool onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	 * @brief   ��������
	 * @details ���������� ��� �������� ������������ ��������
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual rbool onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	/**
	 * @fn      virtual BOResult onDoOperation(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	 * @brief   ����������
	 * @details ���������� ��� ���������� ������/��������
	 * @return  BOResult ��������� ���������� ��������
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual BOResult onDoOperation(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	/**
	 * @fn      virtual void onMakePlaned(CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param)
	 * @brief   ������������
	 * @details ���������� ��� ��������������� � �������� �������: event, operation_end, keyboard_end
	 * ����� �� ��������������, ��������, ��� rule
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual void onMakePlaned(CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param) = 0;

	/**
	 * @fn      virtual BOResult onContinue(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	 * @brief   �����������
	 * @details ���������� ��� ����������� ������ ��������, ��������, DPT search
	 * @return  BOResult ��������� ���������� ��������
	 * @param   pRuntime ����������� ������ �� ��������� �� �������
	 */
	virtual BOResult onContinue(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IBaseOperation
 * @brief   ���������� ���������� IBaseOperation
 *********************************************************************************/
#define DECLARE_IBaseOperation                                                                   \
	virtual void     onStart         (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onStop          (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual rbool    onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual BOResult onDoOperation   (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onMakePlaned    (CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param); \
	virtual BOResult onContinue      (CREF(rdoRuntime::LPRDORuntime) pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);

/******************************************************************************//**
 * @typedef BaseOperationList
 * @brief   ��������� ����� ���������� �� IBaseOperation
 *********************************************************************************/
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif // _LIB_RUNTIME_RDO_H_
