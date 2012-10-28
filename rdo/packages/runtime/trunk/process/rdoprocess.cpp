/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \authors   etc.
  \date      11.06.2006
  \brief     RDOProcess implementation
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess(CREF(tstring) name, CREF(LPRDORuntime) pRuntime)
	: RDOLogicSimple(pRuntime, NULL)
	, m_name        (name          )
{}

void RDOPROCProcess::insertChild(LPIPROCProcess pProcess)
{
	if (pProcess)
	{
		m_child.push_back(pProcess);
		pProcess->setParent(this);
	}
}

void RDOPROCProcess::setParent(LPIPROCProcess pProcess)
{
	m_parent = pProcess;
}

LPIResourceType RDOPROCProcess::getTranType() const
{
	return m_pTransactType;
}

void RDOPROCProcess::next(CREF(LPRDOPROCTransact) pTransact)
{
	if (pTransact->getBlock())
	{
		Iterator it = std::find(begin(), end(), pTransact->getBlock());
		// ���� � ��������� ���� ����
		if (it != end())
		{
			// ����� ���� ����
			LPIPROCBlock block = *it;
			ASSERT(block);
			// ������� ������������ �������� � ������ ��� ����������
			RDOPROCBlock::TransactIt it_res = block->transactFind(pTransact);
			// ���� �������� ������
			// @warning ������ ��� ��������� �������� �� ������������� � ����� GENERATE!!!
			if (it_res != block->transactEnd()) 
			{
				// ������� ��� �� ������ ���������� ����� �����
				block->transactGoOut(*it_res);
			}
			else
			{
				// ������ ����� ����� �� ����� ������, ��������� advance ������� ��������� �� ����
				//	getRuntime()->error( "���������� ������: ���������� �������� (�������� ������� ���� ����)" );
			}
			// ��������� � ���������� �����
			++it;
			// ���� ��������� ���� ����������
			if (it != end())
			{
				// ����� ���� ����
				block = *it;
				ASSERT(block);
				pTransact->setBlock(block);
				// ���������� � ����� ������ ����� ����� ������������ ��������
				block->transactGoIn(pTransact);
			}
			// ���� � �� �������� ����� ���� ����������� �������� ��� ���������
			else 
			{
				//---------���� � ���� ���� ��������, ��� it-1 = ��������� ���� ��� ���������, 
				//---------� ���� ������ ��������� ����� ������� ���� ��������?????????
				//getRuntime()->error( "������ ���������� ��������, ���� � ������� �� ��� - ���������" );
			}
		} 
		else
		{
			//getRuntime()->error( "�� ���������� ������ �����, � �������� �������� ��������" );
		}
	} 
	else 
	{
		//getRuntime()->error( "� ��������� �� �������� ������� ����" );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
RDOPROCTransact::RDOPROCTransact(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary)
	: RDOResource(pRuntime, paramsCalcs, pResType, resID, typeID, trace, temporary)
{
	m_state = RDOResource::CS_Create;
	m_paramList.push_back(pRuntime->getCurrentTime());
}

RDOPROCTransact::~RDOPROCTransact()
{}

LPRDOResource RDOPROCTransact::clone(CREF(LPRDORuntime) pRuntime) const
{
	LPRDOResource pResource = rdo::Factory<RDOPROCTransact>::create(pRuntime, getParamList(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
	ASSERT(pResource);
	pRuntime->insertNewResource(pResource);
	return pResource;
}

void RDOPROCTransact::next()
{
	m_block->getProcess()->next(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary)
	: RDOResource(pRuntime, paramsCalcs, pResType, resID, typeID, trace, temporary)
{}

RDOPROCResource::~RDOPROCResource()
{}

LPRDOResource RDOPROCResource::clone(CREF(LPRDORuntime) pRuntime) const
{
	LPRDOResource pResource = rdo::Factory<RDOPROCResource>::create(pRuntime, getParamList(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
	ASSERT(pResource);
	pRuntime->insertNewResource(pResource);
	return pResource;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlock
// --------------------------------------------------------------------------------
RDOPROCBlock::RDOPROCBlock(LPIPROCProcess pProcess)
	: m_process(pProcess)
{}

rbool RDOPROCBlock::init()
{
	if (!m_process)
		return false;

	m_process.query_cast<IBaseOperationContainer>()->append(this);
	return true;
}

RDOPROCBlock::TransactIt RDOPROCBlock::transactFind(CREF(LPTransact) pTransact)
{
	return std::find(m_transacts.begin(), m_transacts.end(), pTransact);
}

RDOPROCBlock::TransactIt RDOPROCBlock::transactEnd()
{
	return m_transacts.end();
}

void RDOPROCBlock::transactGoIn(CREF(LPTransact) pTransact)
{
	m_transacts.push_back(pTransact);
}

void RDOPROCBlock::transactGoOut(CREF(LPTransact) pTransact)
{
	m_transacts.remove(pTransact);
}

LPIPROCProcess RDOPROCBlock::getProcess() const
{
	return m_process;
}

CLOSE_RDO_RUNTIME_NAMESPACE
