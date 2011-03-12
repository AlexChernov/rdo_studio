/*
 * copyright: (c) RDO-Team, 2011
 * filename : calc_process_control.cpp
 * author   : ����� �������
 * date     : 10.03.2011
 * bref     : RDOCalc ��� �������� ���������� � ������� �� �� � �������
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc_process_control.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl()
{
//������� �� ���������-������� �������� � ������������������� ��� ������ ������ ��������
}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) runtime)
{
	LPIBaseOperation process = *(runtime->m_pMetaLogic->begin());
	if (process->query_cast<IPROCProcess>())
	{
		LPIBaseOperation block = dynamic_cast<RDOPROCProcess>(process)->begin();
		if (dynamic_cast<LPIPROCBlock>(block))
		{
			new RDOPROCTransact( runtime, block );
		}

	}
//	LPIPROCBlock   block     = dynamic_cast<LPIPROCBlock>(process-begin());
//	RDOLogicSimple process = std::find(metaLogic->begin(), metaLogic->end(), 'Process');
//	process->begin();//������ ����
//� ���������� ������ (��� ��), ������ � ����� ��������� ������ ��������� �������� �����:
//	new RDOPROCTransact( runtime, block );
}

CLOSE_RDO_RUNTIME_NAMESPACE
