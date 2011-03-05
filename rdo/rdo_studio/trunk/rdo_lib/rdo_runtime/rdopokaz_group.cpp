/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdopokaz_group.cpp
 * author   : ������ ������
 * date     : 04.03.2011
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdopokaz_group.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_kernel/rdothread.h"
#include "rdo_repository/rdorepository.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokazGroup::RDOPMDPokazGroup(CREF(tstring) name)
	: m_name (name                               )
	, m_state(name.empty() ? RGS_START : RGS_STOP)
{}

RDOPMDPokazGroup::~RDOPMDPokazGroup()
{}

void RDOPMDPokazGroup::resetPokaz(PTR(RDOSimulator) pSimulator)
{
	if (m_state == RGS_STOP)
		return;

	m_timeStart = pSimulator->getCurrentTime();

	if (!m_name.empty())
	{
		if (!m_streamFull.is_open())
		{
			PTR(RDORuntime) pRuntime = dynamic_cast<PTR(RDORuntime)>(pSimulator);
			ASSERT(pRuntime);
			LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
			ASSERT(pThreadProxy);
			rdoRepository::RDOThreadRepository::CreateFileInfo file(rdo::format(_T("- %s - full"), m_name.c_str()), _T("txt"), m_streamFull);
			pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
		}
		if (!m_streamTable.is_open())
		{
			PTR(RDORuntime) pRuntime = dynamic_cast<PTR(RDORuntime)>(pSimulator);
			ASSERT(pRuntime);
			LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
			ASSERT(pThreadProxy);
			rdoRepository::RDOThreadRepository::CreateFileInfo file(rdo::format(_T("- %s - table"), m_name.c_str()), _T("txt"), m_streamTable);
			pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
			if (m_streamTable.is_open())
			{
				STL_FOR_ALL_CONST(m_resultList, it)
				{
					LPIPokazGetValue pGetValue = (*it);
					if (pGetValue)
					{
						LPIName pName = pGetValue;
						ASSERT(pName);
						m_streamTable << pName->name() << _T("\t");
					}
				}
				m_streamTable << std::endl;
			}
		}
	}

	STL_FOR_ALL(m_resultList, it)
	{
		(*it)->resetPokaz(pSimulator);
	}
}

void RDOPMDPokazGroup::checkPokaz(PTR(RDOSimulator) pSimulator)
{
	if (m_state == RGS_STOP)
		return;

	STL_FOR_ALL(m_resultList, it)
	{
		(*it)->checkPokaz(pSimulator);
	}
}

void RDOPMDPokazGroup::calcStat(PTR(RDOSimulator) pSimulator, REF(std::ostream) stream)
{
	if (m_state == RGS_STOP)
		return;

	PTR(RDORuntime) pRuntime = dynamic_cast<PTR(RDORuntime)>(pSimulator);
	if (!m_name.empty())
	{
		double timeStop = pRuntime->getCurrentTime();
		rdo::textstream textStream;
		textStream << rdo::format(_T("---> %s, %f -> %f = %f\n"), m_name.c_str(), m_timeStart, timeStop, timeStop - m_timeStart);
		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}
	}

	rbool tableWrite = false;
	STL_FOR_ALL(m_resultList, it)
	{
		rdo::textstream textStream;

		(*it)->calcStat(pSimulator, textStream);

		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}

		LPIPokazGetValue pGetValue = (*it);
		if (pGetValue)
		{
			if (pGetValue->getValue().typeID() != RDOType::t_real)
			{
				m_streamTable << pGetValue->getValue().getAsString() << _T("\t");
			}
			else
			{
				setlocale(LC_NUMERIC, _T("rus"));
				m_streamTable << rdo::format(_T("%f"), pGetValue->getValue().getDouble()) << _T("\t");
				setlocale(LC_NUMERIC, _T("eng"));
			}
			tableWrite = true;
		}
	}
	if (tableWrite)
	{
		m_streamTable << std::endl;
	}

	if (!m_name.empty())
	{
		rdo::textstream textStream;
		textStream << rdo::format(_T("<--- %s\n"), m_name.c_str());
		stream << textStream.str();
		if (m_streamFull.is_open())
		{
			m_streamFull << textStream.str();
		}
	}
	pRuntime->getResults().flush();
}

void RDOPMDPokazGroup::onStart(PTR(rdoRuntime::RDOSimulator) pSimulator)
{
	m_state = RGS_START;
	resetPokaz(pSimulator);
}

void RDOPMDPokazGroup::onStop(PTR(rdoRuntime::RDOSimulator) pSimulator)
{
	PTR(RDORuntime) pRuntime = dynamic_cast<PTR(RDORuntime)>(pSimulator);
	calcStat(pSimulator, pRuntime->getResults().getOStream());
	m_state = RGS_STOP;
}

void RDOPMDPokazGroup::onAppend(CREF(LPIPokaz) pResult)
{
	ASSERT(pResult);

	m_resultList.push_back(pResult);
}

CLOSE_RDO_RUNTIME_NAMESPACE
