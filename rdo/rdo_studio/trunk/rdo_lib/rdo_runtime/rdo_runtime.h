/*!
  \copyright (c) RDO-Team, 2007
  \file      rdo_runtime.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     RDORuntime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RUNTIME_H_
#define _LIB_RUNTIME_RUNTIME_H_

#pragma warning(disable : 4786)

// ----------------------------------------------------------------------- INCLUDES
#include <time.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdocommon.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/thread_proxy_i.h"
// --------------------------------------------------------------------------------

class RDOThread;

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOResults
  \brief     ���������� �������������
  \todo      ��� ���?
*/
class RDOResults
{
public:
	RDOResults();
	virtual ~RDOResults();

	void width(ruint w);

	template<class T> REF(RDOResults) operator<< (CREF(T) value);

	virtual void              flush     () = 0;
	virtual REF(std::ostream) getOStream() = 0;
};

class RDOEvent;
class RDORule;
class RDOOperation;
class RDOPROCProcess;
class RDOPMDPokaz;
PREDECLARE_POINTER(RDOFRMFrame);
class RDOCalcCreateResource;
PREDECLARE_POINTER(RDOEraseResRelCalc);

/*!
  \class     RDORuntime
  \brief     RDORuntime
*/
CLASS(RDORuntime): INSTANCE_OF(RDOSimulatorTrace)
{
DECLARE_FACTORY(RDORuntime);
public:
	void init  ();
	void deinit();

	typedef  std::vector<LPIPokaz>           LPIPokazList;
	typedef  std::vector<LPIPokazTrace>      LPIPokazTraceList;
	typedef  std::vector<LPIPokazWatchValue> LPIPokazWatchValueList;

	/*!
	  \enum      Messages
	  \brief     ������ � �������������
	*/
	enum Messages
	{
		RO_BEFOREDELETE = 0
	};

	void connect    (PTR(INotify) to, ruint message);
	void disconnect (PTR(INotify) to               );
	void disconnect (PTR(INotify) to, ruint message);
	void fireMessage(ruint message, PTR(void) param);

	std::vector<rdoSimulator::RDOSyntaxError> errors;
	void error(CREF(tstring) message, CREF(LPRDOCalc) pCalc = NULL);

	LPRDORuntime clone   () const;
	void         copyFrom(CREF(LPRDORuntime) pOther);
	rbool        equal   (CREF(LPRDORuntime) pOther) const;

	/*!
	  \class     RDOHotKeyToolkit
	  \brief     ������� �������
	*/
	class RDOHotKeyToolkit
	{
	public:
		typedef unsigned int               KeyCode;
		typedef std::map<tstring, KeyCode> KeySet;
		typedef KeySet::const_iterator     CIterator;
		enum    { UNDEFINED_KEY = ~0 };

		RDOHotKeyToolkit();
		KeyCode codeFromString(CREF(tstring) key) const;

	private:
		KeySet m_keys;
	};

	RDOHotKeyToolkit rdoHotKeyToolkit;

	std::vector<unsigned int> using_scan_codes;
	rbool keyDown(unsigned int scan_code);
	void keyUp(unsigned int scan_code);
	rbool checkKeyPressed(unsigned int scan_code, rbool shift, rbool control);
	rbool checkAreaActivated(CREF(tstring) oprName);

	void setConstValue(unsigned int numberOfConst, RDOValue value);
	RDOValue getConstValue(int numberOfConst);
	void rdoInit(PTR(RDOTrace) tracer, PTR(RDOResults) customResults, PTR(RDOResults) customResultsInfo, CREF(LPIThreadProxy) pThreadProxy);

	REF(RDOResults) getResults();
	REF(RDOResults) getResultsInfo();

	double getTimeNow();
	double getSeconds();
	
	unsigned int getCurrentTerm() const;
	void setCurrentTerm(unsigned int value);

	REF(LPIActivity) getCurrentActivity();
	void             setCurrentActivity(CREF(LPIActivity) activity);

	void addRuntimeEvent    (LPIBaseOperationContainer logic, CREF(LPIEvent)      ev      );
	void addRuntimeRule     (LPIBaseOperationContainer logic, CREF(LPIRule)       rule    );
	void addRuntimeOperation(LPIBaseOperationContainer logic, CREF(LPIOperation)  opration);
	void addRuntimePokaz    (CREF(LPIPokaz)      pPokaz);
	void addRuntimeFrame    (CREF(LPRDOFRMFrame) pFrame);
	
	LPRDOFRMFrame lastFrame() const;

	CREF(LPIPokazList) getPokaz() const;

	void addInitCalc(CREF(LPRDOCalc) initCalc);

	// ��������� �������
	RDOValue getResParamVal(ruint resID, ruint paramID) const;
	REF(RDOValue) getResParamValRaw(ruint resID, ruint paramID);
	void setResParamVal(ruint resID, ruint paramID, CREF(RDOValue) value);

#ifdef _DEBUG
	std::vector<std::vector<RDOValue> > state;
	rbool checkState();
	void showResources(int node) const;
#endif

	void onEraseRes(const int res_id, CREF(LPRDOEraseResRelCalc) pCalc);
	LPRDOResource createNewResource(unsigned int type, PTR(RDOCalcCreateResource) calc);
	LPRDOResource createNewResource(unsigned int type, rbool trace);
	void insertNewResource         (CREF(LPRDOResource) pResource);

	RDOValue      getFuncArgument (int numberOfParam);
	LPRDOResource getGroupFuncRes ();
	void          pushFuncArgument(RDOValue arg);
	void          pushGroupFunc   (CREF(LPRDOResource) pResource);
	void          popFuncArgument ();
	void          popGroupFunc    ();
	void          pushFuncTop     ();
	void          resetFuncTop    (int numArg);
	void          popFuncTop      ();

	virtual rbool endCondition();
	void setTerminateIf(CREF(LPRDOCalc) _pTerminateIfCalc);

	virtual rbool breakPoints();
	void insertBreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc);
	LPRDOCalc findBreakPoint(CREF(tstring) name);
	tstring getLastBreakPointName() const;

	LPRDOResource getResourceByID(const int num) const;

	void setPatternParameter(unsigned int parNumb, RDOValue val);
	RDOValue getPatternParameter(int parNumb);

	std::vector<tstring>        activeAreasMouseClicked;
	std::list<unsigned int>     keysDown;
	std::vector<LPRDOFRMFrame>  allFrames;

	virtual void onPutToTreeNode();

	rdoSimulator::RDOExitCode whyStop;

	virtual void onNothingMoreToDo();
	virtual void onEndCondition();
	virtual void onRuntimeError();
	virtual void onUserBreak();

	virtual void postProcess();

	/*!
	  \enum      FunBreakFlag
	  \brief     ���� ��������� �������
	*/
	enum FunBreakFlag
	{
		FBF_CONTINUE = 0,
		FBF_BREAK,
		FBF_RETURN
	};

	void               setFunBreakFlag(CREF(FunBreakFlag) flag);
	CREF(FunBreakFlag) getFunBreakFlag() const;

	LPRDOMemoryStack getMemoryStack();

	typedef std::list<LPRDOResource> ResList;
	typedef ResList::const_iterator  ResCIterator;

	ResCIterator res_begin() const;
	ResCIterator res_end() const;

	CREF(LPIThreadProxy) getThreadProxy() const;

	void setStudioThread(PTR(RDOThread) pStudioThread);

private:
	RDORuntime();
	virtual ~RDORuntime();

	typedef RDOSimulatorTrace     Parent;
	typedef std::list<LPRDOCalc>  CalcList;

	std::vector<LPRDOResource>  allResourcesByID;      // ��� ������� ����������, ���� NULL (NULL ����� �� ����� ��� ���������� ���������� �������)
	std::list  <LPRDOResource>  allResourcesByTime;    // ��� ��, ������ ����������� �� ������� �������� � ��� NULL-��
	CalcList                    initCalcs;
	LPRDOMemoryStack            m_pMemoryStack;
	FunBreakFlag                m_funBreakFlag;
	LPIThreadProxy              m_pThreadProxy;
	PTR(RDOThread)              m_pStudioThread;

	/*!
	  \class     BreakPoint
	  \brief     ����� ��������
	*/
	OBJECT(BreakPoint) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(BreakPoint)
	public:
		CREF(tstring) getName() const;
		CREF(LPRDOCalc) getCalc() const;

	private:
		BreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc);

		tstring   m_name;
		LPRDOCalc m_pCalc;
	};

	typedef std::list<LPBreakPoint> BreakPointList;

	BreakPointList  breakPointsCalcs;
	LPBreakPoint    lastActiveBreakPoint;

	std::vector<RDOValue>       funcStack;
	std::vector<LPRDOResource>  groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

	virtual void onInit();
	virtual void onDestroy();

	virtual ResList getResourcesBeforeSim() const;

	LPIPokazList            m_pokazAllList;
	LPIPokazTraceList       m_pokazTraceList;
	LPIPokazWatchValueList  m_pokazWatchValueList;

	LPIActivity             m_currActivity;

	std::vector<RDOValue> patternParameters;

	time_t physic_time;
	virtual void preProcess();

	PTR(RDOResults) results;
	PTR(RDOResults) results_info;

	LPRDOCalc pTerminateIfCalc;
	std::vector<RDOValue> allConstants;

	void writeExitCode();

	rbool key_found;
	virtual rbool isKeyDown();

	typedef std::multimap<ruint, PTR(INotify)> Connected;
	Connected m_connected;

	virtual void onResetPokaz();
	virtual void onCheckPokaz();
	virtual void onAfterCheckPokaz();

	unsigned int m_currentTerm;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_runtime.inl"

#endif // _LIB_RUNTIME_RUNTIME_H_
