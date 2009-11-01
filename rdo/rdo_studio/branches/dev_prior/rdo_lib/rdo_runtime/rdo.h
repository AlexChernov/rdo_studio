/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo.h
 * author   : 
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_H_
#define _RDO_H_

namespace rdoRuntime
{
class RDOSimulator;
};

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
	// ������������ ��� IE � GENERATE, ����� ������ ����� ������� ������� �������
	virtual void onStart(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ��������� ��������, ��������, ������� DPT-some ���������� � true �� false
	virtual void onStop(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� �������� ������������ ��������
	virtual rbool onCheckCondition(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ���������� ������/��������
	virtual BOResult onDoOperation(PTR(rdoRuntime::RDOSimulator) sim) = 0;

	// ���������� ��� ��������������� � �������� �������: IE, operation_end, keyboard_end
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

#endif // _RDO_H_
