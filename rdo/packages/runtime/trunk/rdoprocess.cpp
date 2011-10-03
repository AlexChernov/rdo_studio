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
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/rdoprocess.h"
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/calc/relres.h"
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
	m_state     = RDOResource::CS_Create;
	m_params.push_back(pRuntime->getCurrentTime());
}

RDOPROCTransact::~RDOPROCTransact()
{}

LPRDOResource RDOPROCTransact::clone(CREF(LPRDORuntime) pRuntime) const
{
	return rdo::Factory<RDOResource>::create(pRuntime, getParams(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
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
	return rdo::Factory<RDOResource>::create(pRuntime, getParams(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
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

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
void RDOPROCGenerate::onStart( CREF(LPRDORuntime) pRuntime )
{
	calcNextTimeInterval(pRuntime);
}

rbool RDOPROCGenerate::onCheckCondition( CREF(LPRDORuntime) pRuntime )
{
	if (m_maxTransCount > 0)
	{
		if (m_TransCount < m_maxTransCount)
		{
			return pRuntime->getCurrentTime() >= timeNext ? true : false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return pRuntime->getCurrentTime() >= timeNext ? true : false;
	}
}

IBaseOperation::BOResult RDOPROCGenerate::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
//	TRACE1( "%7.1f GENERATE\n", pRuntime->getCurrentTime() );
	std::vector<RDOValue> transactParams(1);
	transactParams.push_back(pRuntime->getCurrentTime());
	LPRDOPROCTransact pTransact = m_process->getTranType()->createRes(pRuntime, pRuntime->getResourceId(), transactParams, true, true).object_static_cast<RDOPROCTransact>();
	ASSERT(pTransact);
	LPIPROCBlock pBlock(this);
	pTransact->setBlock(pBlock);
	m_TransCount++;
	PTR(RDOTrace) tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << pTransact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}
	pTransact->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval( CREF(LPRDORuntime) pRuntime )
{
	pRuntime->addTimePoint( timeNext = pTimeCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime(), m_process, this );
}

void RDOPROCGenerate::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCGenerate::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCGenerate::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForQueue::_onStart(CREF(LPRDORuntime) pRuntime)
{
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	LPRDOResource res = pRuntime->getResourceByID(Id_res);
	forRes.Id_param = Id_param;
	forRes.rss = res.object_static_cast<RDOPROCResource>();
	forRes.defaultValue = RDOValue(RDOPROCQueue::getDefaultValue());
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
void RDOPROCQueue::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCQueue::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	if (!m_transacts.empty())
	{
		RDOValue i = forRes.rss->getParam(forRes.Id_param);
		RDOValue j = RDOValue(int (1));
		forRes.rss->setParam(forRes.Id_param, i + j);
		return true;
	}
	else
	{
		return false;
	}
}

IBaseOperation::BOResult RDOPROCQueue::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	TRACE1(_T("%7.1f QUEUE\n"), pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCQueue::onStop (CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCQueue::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCQueue::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
void RDOPROCDepart::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCDepart::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	if (!m_transacts.empty())
	{
		RDOValue i = forRes.rss->getParam(forRes.Id_param);
		RDOValue j = RDOValue(int (1));
		forRes.rss->setParam(forRes.Id_param, i - j);
		return true;
	}
	else
	{
		return false;
	}
}

IBaseOperation::BOResult RDOPROCDepart::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	TRACE1(_T("%7.1f DEPART\n"), pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCDepart::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCDepart::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCDepart::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForSeize::_onStart(CREF(LPRDORuntime) pRuntime)
{
	/// @todo: ���� ����������� ������ �������, ��������� ������� ����� ������ ������ ��,
	// ��� ��� DPT search, �� ������������� ��������� ���� ����� ������ � checkOperation
	std::vector<parser_for_Seize>::iterator it1 = fromParser.begin();
	while (it1 != fromParser.end())
	{
		int Id_res = (*it1).Id_res;
		int Id_param = (*it1).Id_param;
		LPRDOResource res = pRuntime->getResourceByID(Id_res);
		runtime_for_Seize bbb;
		bbb.Id_param = Id_param;
		bbb.rss = res.object_static_cast<RDOPROCResource>();
		bbb.enum_free = RDOValue(bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree());
		bbb.enum_buzy = RDOValue(bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy());
		forRes.push_back(bbb);
		++it1;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
void RDOPROCSeize::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCSeize::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	if (m_transacts.empty())
		return false;

	ruint Size_Seizes = forRes.size();
	for (ruint i = 0; i < Size_Seizes; i++)
	{
		// ���� ��������
		if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_free)
		{
			ruint idBlocksTransact    = m_transacts.front()->getTraceID();
			ruint idResourcesTransact = forRes[i].rss->transacts.front()->getTraceID();
			if (idBlocksTransact != idResourcesTransact)
				return false;

			PTR(RDOTrace) tracer = pRuntime->getTracer();
			forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_buzy);
			TRACE3(_T("%7.1f SEIZES-%d, resId = %d\n"), pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
			if (!tracer->isNull())
				tracer->getOStream() << forRes[i].rss->traceResourceState(_T('\0'), pRuntime) << tracer->getEOL();

			m_transacts.front()->setRes(forRes[i].rss);
			return true;
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCSeize::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCSeize::transactGoIn(PTR(RDOPROCTransact) pTransact)
{
	int sizeSeizes = forRes.size();
	for(int i = 0; i < sizeSeizes; ++i)
	{
		forRes[i].rss->transacts.push_back(pTransact);
	}
	RDOPROCBlockForSeize::transactGoIn(pTransact);
}

void RDOPROCSeize::transactGoOut(RDOPROCTransact* _transact)
{
	int Size_Seizes = forRes.size();
	for(int i = 0; i < Size_Seizes; ++i)
	{
		forRes[i].rss->transacts.remove(_transact);
	}
	RDOPROCBlockForSeize::transactGoOut(_transact);
}

void RDOPROCSeize::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCSeize::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCSeize::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
void RDOPROCRelease::onStart(CREF(LPRDORuntime) pRuntime)
{
	_onStart(pRuntime);
}

rbool RDOPROCRelease::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!m_transacts.empty())
	{
		int Size_Seizes = forRes.size();
		for(int i=0;i<Size_Seizes; i++)
		{
			if(forRes[i].rss == m_transacts.front()->getRes())
			{
				// �����
				if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_buzy)
				{
					RDOTrace* tracer = pRuntime->getTracer();
					TRACE3(_T("%7.1f RELEASES-%d, resId = %d\n"), pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
					forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_free);
					if (!tracer->isNull())
					{
						tracer->getOStream() << forRes[i].rss->traceResourceState('\0', pRuntime) << tracer->getEOL();
					}				
					return true;
				}		
				// �� ����� � �� ��������
				if (forRes[i].rss->getParam(forRes[i].Id_param) != forRes[i].enum_free)
				{
					//������� ��������
					RDOTrace* tracer = pRuntime->getTracer();
					TRACE3(_T("%7.1f RELEASES_Bad-%d, resId = %d\n"), pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
					LPRDOPROCTransact transact = m_transacts.front();
					ASSERT(transact);
					transact->setState(RDOResource::CS_Erase);
					if (!tracer->isNull())
					{
						tracer->getOStream() << transact->traceResourceState('\0', pRuntime) << tracer->getEOL();
					}
					m_transacts.remove(transact);
					forRes[i].rss->transacts.remove(transact);
					pRuntime->onEraseRes(transact->getTraceID(), NULL);
					return false;
				}
			}
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCRelease::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCRelease::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCRelease::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCRelease::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
rbool RDOPROCAdvance::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!m_transacts.empty())
	{
		return true;
	} 
	else
	{
		if (!leave_list.empty())
		{
		double tnow = pRuntime->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
			while (it != leave_list.end())
			{
				if (tnow >= it->timeLeave)
				{
					return true;
				}
			it++;
			}
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCAdvance::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	if (!m_transacts.empty())
	{
//		TRACE1(_T("%7.1f ADVANCE BEGIN\n"), pRuntime->getCurrentTime());
		double timeLeave = pDelayCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime();
		leave_list.push_back(LeaveTr(m_transacts.front(), timeLeave));
		m_transacts.erase(m_transacts.begin());
		pRuntime->addTimePoint(timeLeave, m_process, this);
		return IBaseOperation::BOR_done;
	} 
	else if (!leave_list.empty())
	{
		double tnow = pRuntime->getCurrentTime();
		std::list<LeaveTr>::iterator it = leave_list.begin();
		while (it != leave_list.end())
		{
			if (tnow >= it->timeLeave)
			{
//				TRACE1(_T("%7.1f ADVANCE END\n"), it->timeLeave);
				it->transact->next();
				leave_list.erase(it);
				return IBaseOperation::BOR_planned_and_run;
			}
			it++;
		}
	}
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCAdvance::onStart(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAdvance::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAdvance::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCAdvance::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
rbool RDOPROCTerminate::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return !m_transacts.empty() ? true : false;
}

IBaseOperation::BOResult RDOPROCTerminate::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
//	TRACE1(_T("%7.1f TERMINATE\n"), pRuntime->getCurrentTime());
	LPRDOPROCTransact transact = m_transacts.front();
	ASSERT(transact);
	transact->setState(RDOResource::CS_Erase);
	RDOTrace* tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << transact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}
	pRuntime->onEraseRes(transact->getTraceID(), NULL);
	m_transacts.erase(m_transacts.begin());
	int termNow = pRuntime->getCurrentTerm();
	termNow += getTerm();
	pRuntime->setCurrentTerm(termNow);
	return IBaseOperation::BOR_done;
}

void RDOPROCTerminate::onStart(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCTerminate::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCTerminate::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCTerminate::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
rbool RDOPROCAssign::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return !m_transacts.empty();
}

IBaseOperation::BOResult RDOPROCAssign::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource res = pRuntime->getResourceByID(t_resId);
	res->setParam(t_parId, pParamValue->calcValue(pRuntime));
	TRACE1(_T("%7.1f ASSIGN\n"), pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCAssign::onStart(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAssign::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAssign::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) param)
{
	UNUSED(pRuntime);
	UNUSED(param   );
}

IBaseOperation::BOResult RDOPROCAssign::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
