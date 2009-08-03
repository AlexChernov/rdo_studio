#ifndef RDO_H
#define RDO_H

#include "rdoruntime_object.h"

namespace rdoRuntime
{
class RDOSimulator;
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseOperation - ������� ����� ��� ���������, ��������� � ������ ��������
// ----------------------------------------------------------------------------
// ���� �� ������� ��� ������� ��� ���� ��������� �����
// ----------------------------------------------------------------------------
class IBaseOperation
{
public:
	enum BOResult
	{
		BOR_cant_run = 0,
		BOR_planned_and_run,
		BOR_must_continue,
		BOR_done
	};

	// ���������� ����� ������� ������� � ����� ������������� ��������
	// ��������, ������� DPT-some ���������� � false �� true
	// ������������ ��� IE � GENERATE, ����� ������ ����� ������� ������� �������
	virtual void onStart( rdoRuntime::RDOSimulator* sim ) = 0;

	// ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	virtual void onStop( rdoRuntime::RDOSimulator* sim ) = 0;

	// ���������� ��� �������� ������������ ��������
	virtual bool onCheckCondition( rdoRuntime::RDOSimulator* sim ) = 0;

	// ���������� ��� ���������� ������/��������
	virtual BOResult onDoOperation( rdoRuntime::RDOSimulator* sim ) = 0;

	// ���������� ��� ��������������� � �������� �������: IE, operation_end, keyboard_end
	// ����� �� ��������������, ��������, ��� rule
	virtual void onMakePlaned( rdoRuntime::RDOSimulator* sim, void* param) = 0;

	// ���������� ��� ����������� ������ ��������, ��������, DPT search
	virtual BOResult onContinue( rdoRuntime::RDOSimulator* sim ) = 0;
};

#define DECLARE_IBaseOperation \
	virtual void     onStart         ( rdoRuntime::RDOSimulator* sim ); \
	virtual void     onStop          ( rdoRuntime::RDOSimulator* sim ); \
	virtual bool     onCheckCondition( rdoRuntime::RDOSimulator* sim ); \
	virtual BOResult onDoOperation   ( rdoRuntime::RDOSimulator* sim ); \
	virtual void     onMakePlaned    ( rdoRuntime::RDOSimulator* sim, void* param); \
	virtual BOResult onContinue      ( rdoRuntime::RDOSimulator* sim );

#endif // RDO_H
