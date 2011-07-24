/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo.h
 * @authors   ������ ������
 * @date      11.06.2006
 * @brief     ����������� ���������� IBaseOperation
 * @details   ����� ���� �������������� ������� �������� �������� ���� ��������� ���
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_RDO_H_
#define _LIB_RUNTIME_RDO_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ********************************************************************************

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
	 */
	enum BOResult
	{
		BOR_cant_run       = 0,
		BOR_planned_and_run,
		BOR_must_continue  ,
		BOR_done
	};

	// ���������� ����� ������� ������� � ����� ������������� ��������
	// ��������, ������� DPT-some ���������� � false �� true
	// ������������ ��� GENERATE, ����� ������ ����� ������� ������� �������
	virtual void onStart(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	virtual void onStop(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// ���������� ��� �������� ������������ ��������
	virtual rbool onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// ���������� ��� ���������� ������/��������
	virtual BOResult onDoOperation(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;

	// ���������� ��� ��������������� � �������� �������: event, operation_end, keyboard_end
	// ����� �� ��������������, ��������, ��� rule
	virtual void onMakePlaned(CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param) = 0;

	// ���������� ��� ����������� ������ ��������, ��������, DPT search
	virtual BOResult onContinue(CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IBaseOperation
 * @brief   unknown
 *********************************************************************************/
#define DECLARE_IBaseOperation                                                                   \
	virtual void     onStart         (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onStop          (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual rbool    onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual BOResult onDoOperation   (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onMakePlaned    (CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param); \
	virtual BOResult onContinue      (CREF(rdoRuntime::LPRDORuntime) pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif // _LIB_RUNTIME_RDO_H_
