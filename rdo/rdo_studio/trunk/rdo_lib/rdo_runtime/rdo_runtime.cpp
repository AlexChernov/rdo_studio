/**
 @file    rdo_runtime.cpp
 @authors ������ ������, ����� �������
 @date    unknown
 @brief   RDORuntime implementation
 @indent  4T
 */

// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
#include <limits>
#include <iomanip>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdopokaz.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_runtime/rdodptrtime.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/relres.h"
#include "rdo_common/rdodebug.h"
#include "rdo_kernel/rdothread.h"
// ===============================================================================

#pragma warning(disable : 4786)  

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
RDORuntime::RDORuntime()
	: RDOSimulatorTrace   (                   )
	, m_currActivity      (NULL               )
	, results             (NULL               )
	, results_info        (NULL               )
	, lastActiveBreakPoint(NULL               )
	, whyStop             (rdoSimulator::EC_OK)
	, key_found           (false              )
	, m_currentTerm       (0                  )
	, m_funBreakFlag      (FBF_CONTINUE       )
	, m_pStudioThread     (NULL               )
{
	pTerminateIfCalc = NULL;
	m_pMemoryStack   = rdo::Factory<RDOMemoryStack>::create();
}

RDORuntime::~RDORuntime()
{}

void RDORuntime::init()
{
	memory_insert(sizeof(rdoRuntime::RDORuntime));
}

void RDORuntime::deinit()
{
	m_connected.clear();
	onDestroy();
}

void RDORuntime::connect(PTR(INotify) to, ruint message)
{
	Connected::iterator it = m_connected.find(message);
	while (it != m_connected.end())
	{
		if (it->second == to)
			break;
		it++;
	}
	if (it == m_connected.end())
	{
		m_connected.insert(Connected::value_type(message, to));
	}
}

void RDORuntime::disconnect(PTR(INotify) to)
{
	Connected::iterator it = m_connected.begin();
	while (it != m_connected.end())
	{
		if (it->second == to)
		{
			it = m_connected.erase(it);
			if (it == m_connected.end()) break;
		}
		it++;
	}
}

void RDORuntime::disconnect(PTR(INotify) to, ruint message)
{
	Connected::iterator it = m_connected.find( message );
	while (it != m_connected.end())
	{
		if (it->second == to)
		{
			it = m_connected.erase(it);
			if (it == m_connected.end()) break;
		}
		it++;
	}
}

void RDORuntime::fireMessage(ruint message, PTR(void) param)
{
	Connected::iterator it = m_connected.find(message);
	while (it != m_connected.end())
	{
		it->second->notify(message, param);
		it++;
	}
}

void RDORuntime::setStudioThread(PTR(RDOThread) pStudioThread)
{
	m_pStudioThread = pStudioThread;
}

bool RDORuntime::endCondition()
{
	if ( !pTerminateIfCalc )
	{
		return false;	// forever
	}
	return fabs(pTerminateIfCalc->calcValue(this).getDouble()) > DBL_EPSILON;
}

void RDORuntime::setTerminateIf(CREF(LPRDOCalc) _pTerminateIfCalc)
{
	pTerminateIfCalc = _pTerminateIfCalc;
}

bool RDORuntime::breakPoints()
{
	STL_FOR_ALL_CONST(breakPointsCalcs, it)
	{
		if ((*it)->getCalc()->calcValue(this).getAsBool())
		{
			lastActiveBreakPoint = *it;
			return true;
		}
	}
	return false;
}

void RDORuntime::insertBreakPoint(const std::string& name, CREF(LPRDOCalc) pCalc)
{
	breakPointsCalcs.push_back(rdo::Factory<BreakPoint>::create(name, pCalc));
}

LPRDOCalc RDORuntime::findBreakPoint(const std::string& name)
{
	STL_FOR_ALL_CONST(breakPointsCalcs, it)
	{
		if ((*it)->getName() == name)
		{
			return (*it)->getCalc();
		}
	}
	return NULL;
}

std::string RDORuntime::getLastBreakPointName() const
{
	return lastActiveBreakPoint ? lastActiveBreakPoint->getName() + _T(": ") + lastActiveBreakPoint->getCalc()->src_text() : _T("");
}

void RDORuntime::setConstValue(ruint numberOfConst, RDOValue value)
{
	if (allConstants.size() <= numberOfConst)
	{
		allConstants.resize(numberOfConst + 1);
	}
	allConstants.at(numberOfConst) = value;
}

RDOValue RDORuntime::getConstValue(int numberOfConst)
{
	return allConstants.at(numberOfConst);
}

#ifdef _DEBUG
bool RDORuntime::checkState()
{
	if (state.empty())
	{
		std::vector<RDOValue> res;
		res.push_back(1);
		res.push_back(4);
		state.push_back(res);
		res[0] = 2;
		res[1] = 1;
		state.push_back(res);
		res[0] = 3;
		res[1] = 3;
		state.push_back(res);
		res[0] = 4;
		res[1] = 2;
		state.push_back(res);
		res[0] = 5;
		res[1] = 6;
		state.push_back(res);
		res.clear();
		res.push_back(5);
		state.push_back(res);
	}
	if (state.size() != allResourcesByID.size()) return false;
	for (ruint i = 0; i < state.size(); i++)
	{
		if (state[i].size() != allResourcesByID[i]->paramsCount()) return false;
		for (ruint j = 0; j < allResourcesByID[i]->paramsCount(); j++)
		{
			if (state[i][j] != allResourcesByID[i]->getParam(j)) return false;
		}
	}
	return true;
}

void RDORuntime::showResources(int node) const
{
	TRACE1(_T("------------- %d:\n"), node);
	int index = 0;
	std::vector<LPRDOResource>::const_iterator it = allResourcesByID.begin();
	while (it != allResourcesByID.end())
	{
		if (*it)
		{
			TRACE1(_T("%d. "), index);
			for (ruint i = 0; i < (*it)->paramsCount(); i++)
			{
				TRACE1(_T("%s "), (*it)->getParam(i).getAsString().c_str());
			}
			TRACE(_T("\n"));
		}
		else
		{
			TRACE1(_T("%d. NULL\n"), index);
		}
		index++;
		it++;
	}
}
#endif

void RDORuntime::onEraseRes(const int res_id, CREF(LPRDOEraseResRelCalc) pCalc)
{
	LPRDOResource res = allResourcesByID.at(res_id);
	if (!res)
	{
		error(rdo::format("��������� ������ ��� ������. ��������, �� ������ ����� � ���� �� �������. ��� ������������ �������: %s", pCalc ? pCalc->getName().c_str() : "����������� ���"), pCalc);
	}
	if (!res->canFree())
	{
		error("���������� ������� ������, �.�. �� ��� ������������", pCalc);
//		error("Try to erase used resource", fromCalc);
	}
	else
	{
		LPIPokazWatchValueList::iterator it = m_pokazWatchValueList.begin();
		while (it != m_pokazWatchValueList.end())
		{
			(*it)->checkResourceErased(res);
			it++;
		}
		allResourcesByID.at(res_id) = NULL;
		// ���������� ������� ���������� � std::list::erase, ������� ���������� �� std::list::remove
		allResourcesByTime.remove(res);
	}
}

void RDORuntime::insertNewResource(CREF(LPRDOResource) pResource)
{
	ASSERT(pResource);
	if (pResource->getTraceID() >= allResourcesByID.size())
	{
		allResourcesByID.resize(pResource->getTraceID() + 1, NULL);
		allResourcesByID.at(pResource->getTraceID()) = pResource;
	}
	else
	{
		if (allResourcesByID.at(pResource->getTraceID()) == NULL)
		{
			allResourcesByID.at(pResource->getTraceID()) = pResource;
		}
		else
		{
			error("���������� ������: insertNewResource");
		}
	}
	allResourcesByTime.push_back(pResource);
}

void RDORuntime::insertNewResourceBeforeSim(CREF(LPRDOResource) pResource)
{
	ASSERT(pResource);
	allResourcesBeforeSim.push_back(pResource);
}

void RDORuntime::addRuntimeEvent(LPIBaseOperationContainer logic, CREF(LPIEvent) ev)
{
	appendBaseOperation(logic, ev);
}

void RDORuntime::addRuntimeRule(LPIBaseOperationContainer logic, CREF(LPIRule) rule)
{
	appendBaseOperation(logic, rule);
}

void RDORuntime::addRuntimeOperation(LPIBaseOperationContainer logic, CREF(LPIOperation) opration)
{
	appendBaseOperation(logic, opration);
}

void RDORuntime::addRuntimePokaz(CREF(LPIPokaz) pPokaz)
{
	m_pokazAllList.push_back(pPokaz);
	LPIPokazTrace pokazTrace = pPokaz;
	LPITrace      trace      = pPokaz;
	if (pokazTrace && trace && trace->traceable())
	{
		m_pokazTraceList.push_back(pokazTrace);
	}
	if (pPokaz.query_cast<IPokazWatchValue>())
	{
		m_pokazWatchValueList.push_back(pPokaz);
	}
}

void RDORuntime::addRuntimeFrame(CREF(LPRDOFRMFrame) pFrame)
{ 
	allFrames.push_back(pFrame);
}

LPRDOFRMFrame RDORuntime::lastFrame() const
{
	return !allFrames.empty() ? allFrames.front() : NULL;
}

bool RDORuntime::keyDown(ruint scan_code)
{
	// ���� ������ VK_SHIFT ��� VK_CONTROL, �� ������� ����� ����������
	if (scan_code == VK_SHIFT || scan_code == VK_CONTROL)
	{
		std::list<ruint>::iterator it = keysDown.begin();
		while (it != keysDown.end())
		{
			if (*it != VK_SHIFT && *it != VK_CONTROL)
			{
				it = keysDown.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	// ���������� ������� ��� ������� ��� � ������
	int cnt = 0;
	std::list<ruint>::iterator it = keysDown.begin();
	while (it != keysDown.end())
	{
		if (*it == scan_code)
		{
			cnt++;
		}
		it++;
	}
	// ������� ������� � �����
	if (cnt < 4)
	{
		keysDown.push_back(scan_code);
	}
	if (cnt == 0) key_found = true;
	return cnt > 0;
}

void RDORuntime::keyUp(ruint scan_code)
{
	// ���� ������ VK_SHIFT ��� VK_CONTROL, �� ������� ������ �� �� ������
	//if (scan_code == VK_SHIFT || scan_code == VK_CONTROL)
	//{
		std::list<ruint>::iterator it = keysDown.begin();
		while (it != keysDown.end())
		{
			if (*it == scan_code)
			{
				it = keysDown.erase(it);
			}
			else
			{
				it++;
			}
		}
	//}
}

bool RDORuntime::checkKeyPressed(ruint scan_code, bool shift, bool control)
{
	if (scan_code == 0) return false;
	bool shift_found   = false;
	bool control_found = false;
	// ������ VK_SHIFT �/��� VK_CONTROL � ������
	std::list<ruint>::iterator it = keysDown.begin();
	while (it != keysDown.end())
	{
		if (*it == VK_SHIFT)
		{
			shift_found = true;
			if (shift_found && control_found) break;
		}
		if (*it == VK_CONTROL)
		{
			control_found = true;
			if (shift_found && control_found) break;
		}
		it++;
	}
	// ������ ������ ���� ������� � ������
	// ������ � �� ������ ����� �������
	if (shift_found == shift && control_found == control)
	{
		std::list<ruint>::iterator it = keysDown.begin();
		while (it != keysDown.end())
		{
			if (*it == scan_code)
			{
				keysDown.erase(it);
				key_found = true;
				return true;
			}
			it++;
		}
	}
	key_found = false;
	return false;
}

bool RDORuntime::checkAreaActivated(const std::string& oprName)
{
	std::vector<std::string>::iterator it = std::find(activeAreasMouseClicked.begin(), activeAreasMouseClicked.end(), oprName);
	if (it == activeAreasMouseClicked.end())
	{
		return false;
	}
	activeAreasMouseClicked.erase(it);
	return true;
}

bool RDORuntime::isKeyDown()
{
	return key_found || !activeAreasMouseClicked.empty();
}

void RDORuntime::rdoInit(RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo, CREF(LPIThreadProxy) pThreadProxy)
{
	ASSERT(pThreadProxy);

	m_tracer       = tracer;
	results        = customResults;
	results_info   = customResultsInfo;
	currFuncTop    = 0;
	m_pThreadProxy = pThreadProxy;
	Parent::rdoInit();
}

void RDORuntime::onInit()
{
	STL_FOR_ALL(initCalcs, calcIt)
		(*calcIt)->calcValue(this);

	std::vector<LPRDOResource>::const_iterator it = allResourcesByID.begin();
	while (it != allResourcesByID.end())
	{
		allResourcesByTime.push_back(*it);
		it++;
	}
	//std::copy(allResourcesByID.begin(), allResourcesByID.end(), allResourcesByTime.begin());
}

void RDORuntime::onDestroy()
{
	//! TODO: ����, �������� ���������� �����������, ������ � allResourcesByTime ����������� ������� ?
	//! TODO: ����, ������ � ������� ������� ������� �� 1 ������, ��� � ������ ?
	allResourcesBeforeSim.clear();
	allResourcesByTime.clear();
	allResourcesByID.clear();

	if (results)
	{
		delete results;
		results = NULL;
	}
	if (results_info)
	{
		delete results_info;
		results_info = NULL;
	}
}

RDOValue RDORuntime::getFuncArgument(int numberOfParam)
{
	return funcStack.at(currFuncTop + numberOfParam);
}

LPRDORuntime RDORuntime::clone() const
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	ASSERT(pRuntime);
	pRuntime->m_sizeofSim = sizeof(RDORuntime);

	LPRDORuntime pThis(const_cast<PTR(RDORuntime)>(this));
	pRuntime->copyFrom(pThis);

	return pRuntime;
}

void RDORuntime::copyFrom(CREF(LPRDORuntime) pOther)
{
	ASSERT(pOther);

	ruint size = pOther->allResourcesByID.size();
	for (ruint i = 0; i < size; i++)
	{
		if (pOther->allResourcesByID.at(i) == NULL)
		{
			allResourcesByID.push_back(NULL);
		}
		else
		{
			// ������� ������� � ��������� allResourcesByID ������ RDORuntime ���������� � ��� ������������
			pOther->allResourcesByID.at(i)->clone(this);
			m_sizeofSim += sizeof(RDOResource) + sizeof(void*) * 2;
		}
	}
	allConstants      = pOther->allConstants;
	patternParameters = pOther->patternParameters;
	results           = pOther->results;
	m_pThreadProxy    = pOther->m_pThreadProxy;
	setCurrentTime(pOther->getCurrentTime());

	Parent::copyFrom(pOther.object_parent_cast<Parent>());
}

rbool RDORuntime::equal(CREF(LPRDORuntime) pOther) const
{
	if (pOther->allResourcesByID.size() != allResourcesByID.size()) return false;

	ruint size = allResourcesByID.size();
	for (ruint i = 0; i < size; i++)
	{
		if (allResourcesByID.at(i) == NULL && pOther->allResourcesByID.at(i) != NULL) return false;
		if (allResourcesByID.at(i) != NULL && pOther->allResourcesByID.at(i) == NULL) return false;
		if (allResourcesByID.at(i) == NULL && pOther->allResourcesByID.at(i) == NULL) continue;
		if (pOther->allResourcesByID.at(i) != allResourcesByID.at(i)) return false;
	}
	return true;
}

void RDORuntime::onResetPokaz()
{
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
	{
		(*it)->resetPokaz(this);
		it++;
	}
}

void RDORuntime::onCheckPokaz()
{
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
	{
		(*it)->checkPokaz(this);
		it++;
	}
}

void RDORuntime::onAfterCheckPokaz()
{
	LPIPokazTraceList::iterator it = m_pokazTraceList.begin();
	while (it != m_pokazTraceList.end())
	{
		(*it)->tracePokaz();
		it++;
	}
}

void RDORuntime::error(CREF(tstring) message, CREF(LPRDOCalc) pCalc)
{
	if (!message.empty())
	{
		errors.push_back(rdoSimulator::RDOSyntaxError(rdoSimulator::RDOSyntaxError::UNKNOWN, rdo::format("��������� �����: %f. %s", getTimeNow(), message.c_str()), pCalc ? pCalc->src_pos().m_last_line : 0, pCalc ? pCalc->src_pos().m_last_pos : 0, pCalc ? pCalc->src_filetype() : rdoModelObjects::PAT));
	}
	throw RDORuntimeException("");
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for (ruint i = 0; i < allResourcesByID.size(); i++)
	{
		if (allResourcesByID.at(i))
		{
			allResourcesByID.at(i)->makeTemporary(false);
		}
	}
}

void RDORuntime::writeExitCode()
{
	switch (whyStop)
	{
	case rdoSimulator::EC_OK:
		getTracer()->writeStatus(this, "NORMAL_TERMINATION");
		break;
	case rdoSimulator::EC_NoMoreEvents:
		getTracer()->writeStatus(this, "NO_MORE_EVENTS");
		break;
	case rdoSimulator::EC_RunTimeError:
		getTracer()->writeStatus(this, "RUN_TIME_ERROR");
		break;
	case rdoSimulator::EC_UserBreak:
		getTracer()->writeStatus(this, "USER_BREAK");
		break;
	}
}

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
	{
		try
		{
			(*it)->calcStat(this, getResults().getOStream());
		}
		catch (REF(RDORuntimeException))
		{}
		it++;
	}

	try
	{
		Parent::postProcess();
		writeExitCode();
		getTracer()->stopWriting();
	}
	catch (REF(RDORuntimeException) e)
	{
		writeExitCode();
		getTracer()->stopWriting();
		throw e;
	}
}

LPRDOMemoryStack RDORuntime::getMemoryStack()
{
	return m_pMemoryStack;
}

void RDORuntime::setFunBreakFlag(CREF(FunBreakFlag) flag)
{
	m_funBreakFlag = flag;
}

CREF(RDORuntime::FunBreakFlag) RDORuntime::getFunBreakFlag() const
{
	return m_funBreakFlag;
}

RDORuntime::RDOHotKeyToolkit::RDOHotKeyToolkit()
{														   
	m_keys.insert(KeySet::value_type("ESCAPE", VK_ESCAPE));
	m_keys.insert(KeySet::value_type("TAB", VK_TAB));
	m_keys.insert(KeySet::value_type("SHIFT", VK_SHIFT));
	m_keys.insert(KeySet::value_type("CONTROL", VK_CONTROL));
	m_keys.insert(KeySet::value_type("BACK", VK_BACK));
	m_keys.insert(KeySet::value_type("RETURN", VK_RETURN));
	m_keys.insert(KeySet::value_type("INSERT", VK_INSERT));
	m_keys.insert(KeySet::value_type("HOME", VK_HOME));
	m_keys.insert(KeySet::value_type("PRIOR", VK_PRIOR));
	m_keys.insert(KeySet::value_type("DELETE", VK_DELETE));
	m_keys.insert(KeySet::value_type("END", VK_END));
	m_keys.insert(KeySet::value_type("NEXT", VK_NEXT));
	m_keys.insert(KeySet::value_type("UP", VK_UP));
	m_keys.insert(KeySet::value_type("LEFT", VK_LEFT));
	m_keys.insert(KeySet::value_type("DOWN", VK_DOWN));
	m_keys.insert(KeySet::value_type("RIGHT", VK_RIGHT));
	m_keys.insert(KeySet::value_type("DIVIDE", VK_DIVIDE));
	m_keys.insert(KeySet::value_type("MULTIPLY", VK_MULTIPLY));
	m_keys.insert(KeySet::value_type("SUBTRACT", VK_SUBTRACT));
	m_keys.insert(KeySet::value_type("ADD", VK_ADD));
	m_keys.insert(KeySet::value_type("CLEAR", VK_CLEAR));
	m_keys.insert(KeySet::value_type("SPACE", VK_SPACE));
	m_keys.insert(KeySet::value_type("DECIMAL", VK_DECIMAL));
	m_keys.insert(KeySet::value_type("F2", VK_F2));
	m_keys.insert(KeySet::value_type("F3", VK_F3));
	m_keys.insert(KeySet::value_type("F4", VK_F4));
	m_keys.insert(KeySet::value_type("F5", VK_F5));
	m_keys.insert(KeySet::value_type("F6", VK_F6));
	m_keys.insert(KeySet::value_type("F7", VK_F7));
	m_keys.insert(KeySet::value_type("F8", VK_F8));
	m_keys.insert(KeySet::value_type("F9", VK_F9));
	m_keys.insert(KeySet::value_type("F10", VK_F10));
	m_keys.insert(KeySet::value_type("F11", VK_F11));
	m_keys.insert(KeySet::value_type("F12", VK_F12));
	m_keys.insert(KeySet::value_type("NUMPAD0", VK_NUMPAD0));
	m_keys.insert(KeySet::value_type("NUMPAD1", VK_NUMPAD1));
	m_keys.insert(KeySet::value_type("NUMPAD2", VK_NUMPAD2));
	m_keys.insert(KeySet::value_type("NUMPAD3", VK_NUMPAD3));
	m_keys.insert(KeySet::value_type("NUMPAD4", VK_NUMPAD4));
	m_keys.insert(KeySet::value_type("NUMPAD5", VK_NUMPAD5));
	m_keys.insert(KeySet::value_type("NUMPAD6", VK_NUMPAD6));
	m_keys.insert(KeySet::value_type("NUMPAD7", VK_NUMPAD7));
	m_keys.insert(KeySet::value_type("NUMPAD8", VK_NUMPAD8));
	m_keys.insert(KeySet::value_type("NUMPAD9", VK_NUMPAD9));
	m_keys.insert(KeySet::value_type("NOKEY", 0));

	for (char i = '0'; i <= '9'; i++)
	{
		m_keys.insert(KeySet::value_type(std::string(1, i), (KeyCode)i));
	}
	for (char i = 'A'; i <= 'Z'; i++)
	{
		m_keys.insert(KeySet::value_type(std::string(1, i), (KeyCode)i));
	}
}

RDORuntime::RDOHotKeyToolkit::KeyCode RDORuntime::RDOHotKeyToolkit::codeFromString(const std::string& key) const
{
	CIterator it = m_keys.find(key);
	if (it == m_keys.end())
	{
		return UNDEFINED_KEY;
//		throw RDORuntimeException("Unknown key name: " + key);
	}
	return (*it).second;
}

CLOSE_RDO_RUNTIME_NAMESPACE
