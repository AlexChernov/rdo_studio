/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdo_simulator.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_SIMULATOR_H_
#define _RDO_SIMULATOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdobase.h"
#include "rdo_lib/rdo_runtime/rdo_logic_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - ���� �� ������� ������� ��� RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
public:
	RDOSimulator();
	virtual ~RDOSimulator();

	void             appendLogic       (CREF(LPIBaseOperation) pLogic, LPIBaseOperationContainer pParent);
	LPIBaseOperation getMustContinueOpr() const                            { return m_pOprMustContinue;       }
	void             setMustContinueOpr(CREF(LPIBaseOperation) pOperation) { m_pOprMustContinue = pOperation; }
	virtual void     onPutToTreeNode   () = 0;

	tstring          writeActivitiesStructure(REF(ruint) counter);

	PTR(RDOSimulator) createCopy();

	// ��� DPT ���������� ��������� ��� ������������� �������:
	// 1. ������� ���� RDOSimulator � ������� ���� ��������, �� �� �����
	virtual PTR(RDOSimulator) clone()                   = 0;
	// 2. ��������� ���� ����������� �� ��������
	virtual rbool operator== (CREF(RDOSimulator) other) = 0;

	ruint getSizeofSim() const
	{
		return m_sizeofSim;
	}

	LPIBaseOperationContainer m_pMetaLogic;

protected:
	void appendBaseOperation(LPIBaseOperationContainer logic, CREF(LPIBaseOperation) op)
	{
		ASSERT(op);
		ASSERT(logic);
		logic->append(op);
	}

	// �������������� ������������ ������� � ����� GENERATE: ������ ����� ������� ������������
	virtual void preProcess();

	virtual void onResetPokaz     () = 0;
	virtual void onCheckPokaz     () = 0;
	virtual void onAfterCheckPokaz() = 0;

	ruint m_sizeofSim;

private:
	LPIBaseOperation m_pOprMustContinue;

	virtual rbool doOperation();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_SIMULATOR_H_
