#ifndef RDO_H
#define RDO_H

#include "rdoruntime_object.h"

namespace rdoRuntime {

class RDOSimulator;

// ----------------------------------------------------------------------------
// ---------- RDOBaseOperation - ������� ����� ��� ���������, ��������� � ������ ��������
// ----------------------------------------------------------------------------
// ���� �� ������� ��� ������� ��� ���� ��������� �����
// ----------------------------------------------------------------------------
class RDOBaseOperation: public RDORuntimeParent
{
public:
	enum BOResult {
		BOR_cant_run = 0,
		BOR_planned_and_run,
		BOR_must_continue,
		BOR_done
	};

	// ���������� ����� ������� ������� � ����� ������������� ��������
	// ��������, ������� DPT-some ���������� � false �� true
	// ������������ ��� IE � GENERATE, ����� ������ ����� ������� ������� �������
	virtual void onStart( RDOSimulator* sim )
	{
	}

	// ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	virtual void onStop( RDOSimulator* sim )
	{
	}

	// ���������� ��� �������� ������������ ��������
	virtual bool onCheckCondition( RDOSimulator* sim ) = 0;

	// ���������� ��� ���������� ������/��������
	virtual BOResult onDoOperation( RDOSimulator* sim ) = 0;

	// ���������� ��� ��������������� � �������� �������: IE, operation_end, keyboard_end
	// ����� �� ��������������, ��������, ��� rule
	virtual void onMakePlaned( RDOSimulator* sim, void* param = NULL )
	{
	}

	// ���������� ��� ����������� ������ ��������, ��������, DPT search
	virtual BOResult onContinue( RDOSimulator* sim )
	{
		return BOR_cant_run;
	}

protected:
	RDOBaseOperation( RDORuntimeParent* parent ): RDORuntimeParent( parent ) {}
	virtual ~RDOBaseOperation() {}
};

} // namespace rdoRuntime

#endif // RDO_H
