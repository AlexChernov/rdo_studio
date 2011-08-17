/*
  \copyright (c) RDO-Team, 2011
  \file      rdosimwin.h
  \authors   ���� ���������
  \authors   ������ ������
  \date      
  \brief     
  \indent    4T
 */

#ifndef _RDOSIMWIN_H_
#define _RDOSIMWIN_H_

// ====================================================================== INCLUDES
#pragma warning(disable : 4786)
#include <string>
#include <sstream>
#include <vector>
#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdostream.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_kernel/rdothread.h"
#include "rdo_lib/rdo_runtime/thread_proxy_i.h"
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_simulator/procgui.h"
// ===============================================================================

//#define DISABLE_CORBA
//#ifndef DISABLE_CORBA
OPEN_RDO_ANIMATION_NAMESPACE
struct RDOFrame;
CLOSE_RDO_ANIMATION_NAMESPACE

#define DISABLE_CORBA

//#define CORBA_ENABLE
#ifdef CORBA_ENABLE

#ifndef __WIN32__
#define __WIN32__
#endif

#ifndef __x86__
#define __x86__
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef __NT__
#define __NT__
#endif

#ifndef __OSVERSION__
#define __OSVERSION__ 4
#endif

#pragma warning(disable: 4996)
#include <omniORB4/CORBA.h>
#include "rdo_lib/rdo_corba/RDOCorba.hh"
#pragma warning(default: 4996)

namespace rdoCorba {

// --------------------------------------------------------------------
// ---------- RDOThreadCorba
// --------------------------------------------------------------------
class RDOThreadCorba: public RDOThreadMT
{
public:
	RDOThreadCorba();
	
private:
	virtual ~RDOThreadCorba() {}; //! ����� ������ ���� ������� ����� delete
	virtual void proc(REF(RDOMessageInfo) msg);
	virtual void idle();
	virtual void start();
	virtual void stop();

	PTR(CWinThread) thread_corbaRunThreadFun;
	static ruint corbaRunThreadFun(PTR(void) param);
};

} //! namespace rdoCorba

#endif //! CORBA_ENABLE

OPEN_RDO_SIMULATOR_NAMESPACE
class RDOThreadSimulator;
CLOSE_RDO_SIMULATOR_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE

class RDOResult;

// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
OBJECT(RDOThreadRunTime)
	IS  INSTANCE_OF      (RDOThreadMT )
	AND IMPLEMENTATION_OF(IThreadProxy)
{
DECLARE_FACTORY(RDOThreadRunTime);
public:
	rbool runtimeError() const;

	struct GetFrame
	{
		PTR(rdoAnimation::RDOFrame) m_pFrame;
		ruint                       m_number;

		GetFrame(PTR(rdoAnimation::RDOFrame) pFrame, ruint number)
			: m_pFrame(pFrame)
			, m_number(number)
		{}
	};

	struct FrameAreaDown
	{
		ruint   m_number;
		tstring m_name;

		FrameAreaDown(ruint number, CREF(tstring) name)
			: m_number(number)
			, m_name  (name  )
		{}
	};

private:
	PTR(rdoSimulator::RDOThreadSimulator) m_pSimulator;
	rbool                                 m_runtimeError;
	SYSTEMTIME                            m_timeStart;

	RDOThreadRunTime();
	virtual ~RDOThreadRunTime() //! ����� ������ ���� ������� ����� delete
	{};

	virtual void proc   (REF(RDOMessageInfo) msg);
	virtual void idle   ();
	virtual void start  ();
	virtual void stop   ();
	virtual void destroy();

	void writeResultsInfo();

	void sendMessage(ThreadID threadID, ruint messageID, PTR(void) pParam);
};

CLOSE_RDO_RUNTIME_NAMESPACE

class RDOTrace;

OPEN_RDO_PARSER_NAMESPACE
PREDECLARE_POINTER(RDOParser);
CLOSE_RDO_PARSER_NAMESPACE

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
class RDOThreadSimulator: public RDOThreadMT
{
friend class rdoRuntime::RDOThreadRunTime;
friend class RDORuntimeTracer;
friend class RDOSimResulter;

public:
	struct RTP
	{
		struct Param
		{
			tstring m_name;
		};
		typedef std::vector<Param> ParamList;

		tstring   m_name;
		ParamList m_params;
	};

	struct RSS
	{
		tstring m_name;
	};

	struct GetRTP: public std::vector<RTP>
	{};
	struct GetRSS: public std::vector<RSS>
	{};

private:
	rdoParse::LPRDOParser           m_pParser;
	rdoRuntime::LPRDORuntime        m_pRuntime;
	rbool                           m_canTrace;
	LPProcGUIProcess                m_pGUIProcess;
	LPProcGUIBlock                  m_pBlock;
	rdoRuntime::LPRDOThreadRunTime  m_pThreadRuntime;
	rdoSimulator::RDOExitCode       m_exitCode;

	void terminateModel();
	void closeModel    (); 

	ShowMode m_showMode; //! current show mode
	double   m_showRate; //! current show mode

	rdo::textstream m_resultString;
	rdo::textstream m_resultInfoString;

	
#ifdef CORBA_ENABLE

//	void corbaGetRTPcount(REF(::CORBA::Long) rtp_count);
//	void corbaGetRTPParamscount(REF(rdoParse::RDOCorba::PARAM_count) params_count);
	void corbaGetRTP(REF(rdoParse::RDOCorba::GetRTP_var) my_rtpList);
	void corbaGetRSS(REF(rdoParse::RDOCorba::GetRSS_var) my_rssList);

#endif //! CORBA_ENABLE

protected:
	virtual ~RDOThreadSimulator(); //! ����� ������ ���� ������� ����� delete �������� ��� � protected

	virtual void proc(REF(RDOMessageInfo) msg);

	rbool parseModel();
	void  runModel  ();
	void  stopModel ();

	typedef std::vector<RDOSyntaxError> SyntaxErrorList;
	SyntaxErrorList getErrors();

	void codeCompletion();

	void corbaGetRTP(PTR(GetRTP) RTPList);
	void corbaGetRSS(PTR(GetRSS) RSSList);

public:
	RDOThreadSimulator();

	void parseSMRFileInfo(REF(rdo::textstream) smr, REF(rdoModelObjectsConvertor::RDOSMRFileInfo) info);

	ShowMode getInitialShowMode   () const;
	int      getInitialFrameNumber() const;
	double   getInitialShowRate   () const;

	struct GetList
	{
		enum Type
		{
			frames,
			bitmaps
		};

		typedef std::list<tstring> StringList;

		Type            m_type;
		PTR(StringList) m_list;

		GetList(Type type, PTR(StringList) list)
			: m_type(type)
			, m_list(list)
		{}
	};
};

// --------------------------------------------------------------------
// ---------- RDOThreadCodeComp
// --------------------------------------------------------------------
class RDOThreadCodeComp: public RDOThreadMT
{
protected:
	rdoParse::LPRDOParser m_pParser;

	virtual ~RDOThreadCodeComp(); //! ����� ������ ���� ������� ����� delete �������� ��� � protected
	virtual void proc(REF(RDOMessageInfo) msg);

public:
	RDOThreadCodeComp();

	struct GetCodeComp
	{
		rdoModelObjects::RDOFileType m_file;
		int                          m_pos_x;
		int                          m_pos_y;
		REF(tstring)                 m_result;

		GetCodeComp(rdoModelObjects::RDOFileType file, int pos_x, int pos_y, REF(tstring) result)
			: m_file  (file  )
			, m_pos_x (pos_x )
			, m_pos_y (pos_y )
			, m_result(result)
		{}
	};
};

CLOSE_RDO_SIMULATOR_NAMESPACE

#endif //! _RDOSIMWIN_H_
