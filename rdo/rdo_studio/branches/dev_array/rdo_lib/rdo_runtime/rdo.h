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
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;

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
	virtual void onStart(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	virtual void onStop(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� �������� ������������ ��������
	virtual rbool onCheckCondition(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ���������� ������/��������
	virtual BOResult onDoOperation(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ��������������� � �������� �������: event, operation_end, keyboard_end
	// ����� �� ��������������, ��������, ��� rule
	virtual void onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) = 0;

	// ���������� ��� ����������� ������ ��������, ��������, DPT search
	virtual BOResult onContinue(PTR(rdoRuntime::RDOSimulator) sim) = 0;
};

#define DECLARE_IBaseOperation \
	virtual void     onStart         (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void     onStop          (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual rbool    onCheckCondition(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual BOResult onDoOperation   (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void     onMakePlaned    (PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param); \
	virtual BOResult onContinue      (PTR(rdoRuntime::RDOSimulator) sim);

INTERFACE_PREDECLARATION(IBaseOperation);
typedef std::vector<LPIBaseOperation> BaseOperationList;

#endif //! _RDO_H_
