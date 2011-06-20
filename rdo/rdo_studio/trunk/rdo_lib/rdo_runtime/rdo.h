/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo.h
 * author   : ������ ������
 * date     : 11.06.2006
 * bref     : ����������� ���������� IBaseOperation, ����� ���� �������������� ������� �������� �������� ���� ��������� ���
 * indent   : 4T
 */

#ifndef _RDO_H_
#define _RDO_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IRDOBaseOperation - ��������� ��� ���������, ��������� � ������ ��������
// ----------------------------------------------------------------------------
// ���� �� ������� ��� ������� ��� ���� ��������� �����
// ----------------------------------------------------------------------------
class IBaseOperation
{
public:
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

#define DECLARE_IBaseOperation                                                                   \
	virtual void     onStart         (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onStop          (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual rbool    onCheckCondition(CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual BOResult onDoOperation   (CREF(rdoRuntime::LPRDORuntime) pRuntime);                  \
	virtual void     onMakePlaned    (CREF(rdoRuntime::LPRDORuntime) pRuntime, PTR(void) param); \
	virtual BOResult onContinue      (CREF(rdoRuntime::LPRDORuntime) pRuntime);

INTERFACE_PREDECLARATION(IBaseOperation);
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif //! _RDO_H_
