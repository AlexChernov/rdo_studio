#ifndef RDO_THREAD_H
#define RDO_THREAD_H

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning( disable : 4786 )
#endif // COMPILER_VISUAL_STUDIO
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// #define RDOSIM_COMPATIBLE
//
// �������� ����� ��������� �������
// ������������ ��� ���������� ������, ����� �����������, �� ������ ���
// ������� (������ ��� ���� � ������ ���):
//   1. ���������� ������������� ������� �� LIFO
//   2. ����������� ��������� � ����������� ��������
//   3. ����������� ������� ������� ����� ����������� ���� ������� �������
//   4. ������� � ����������� ������������ �������� � ����� ������� ����� ����������, � �� �����

// --------------------------------------------------------------------------------
// ������������ ��� ���������� (����/�����)-�������� ������ ���
// ���� ���������� � �������� �������
#if defined(RDO_ST)
	#undef RDO_MT
// ���� ���������� � �������� �������
#elif defined(RDO_MT)
	#undef RDO_ST
// ���� �� ���������� � �������� �������
#else
	#define RDO_MT
	#undef RDO_MT // ���������� ������������ ������ ���. ���� ����������������, �� ��������� �������������

	// RDO_ST ������������� ������������ ��� ������������ ������ ���
	#ifndef RDO_MT
		#define RDO_ST
	#endif
#endif

// ������������ ��� ����������� ��������� � ���� C:\rdo_kernel_mt.log
// ����� ���� ������������ � � ������������ ������
// ���� �� ���������� � �������� �������
#ifndef TR_TRACE
	#define TR_TRACE
	#undef TR_TRACE // ���������������� ��� ������ �����������
#endif

#ifdef RDO_MT
#include <afxmt.h>
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOThread
// --------------------------------------------------------------------------------
#ifdef RDO_MT
typedef ruint (*RDOThreadFun)(PTR(void) pParam);
#endif

class RDOThread: public boost::noncopyable
{
#ifdef RDO_MT
friend class RDOKernelGUI;
friend class RDOThreadGUI;
#else
friend class RDOKernel;
#endif
public:
	enum RDOTreadMessage {
		RT_THREAD_CLOSE = 1,
		RT_THREAD_CONNECTION,
		RT_THREAD_DISCONNECTION,
		RT_THREAD_REGISTERED,                  // param = thread:RDOThread*
		RT_THREAD_UNREGISTERED,                // param = thread:RDOThread*
		RT_THREAD_STOP_AFTER,
		RT_STUDIO_MODEL_NEW,                   // param = rdo::repository::RDOThreadRepository::NewModel* = { model_name:tstring&, model_path:tstring&      }
		RT_STUDIO_MODEL_OPEN,                  // param = rdo::repository::RDOThreadRepository::OpenFile* = { model_name:tstring&, result:rbool&, readonly:rbool& }
		RT_STUDIO_MODEL_SAVE,
		RT_STUDIO_MODEL_CLOSE,
		RT_STUDIO_MODEL_BUILD,
		RT_STUDIO_MODEL_RUN,
		RT_STUDIO_MODEL_STOP,
		RT_STUDIO_MODEL_GET_TEXT,              // param = rdo::repository::RDOThreadRepository::FileData* = { file_type:rdoModelObjects::RDOFileType, result:rdo::binarystream& }
		RT_REPOSITORY_MODEL_NEW,               // param = rdo::repository::RDOThreadRepository::NewModel* = { model_name:tstring&, model_path:tstring&   }
		RT_REPOSITORY_MODEL_OPEN,
		RT_REPOSITORY_MODEL_OPEN_ERROR,        // param = model_name:tstring*
		RT_REPOSITORY_MODEL_SAVE,
		RT_REPOSITORY_MODEL_OPEN_GET_NAME,     // param = rdo::repository::RDOThreadRepository::OpenFile* = { model_name:tstring, readonly:rbool, result:rbool }
		RT_REPOSITORY_MODEL_CLOSE,
		RT_REPOSITORY_MODEL_GET_FILEINFO,      // param = rdo::repository::RDOThreadRepository::FileInfo* = { type:rdoModelObjects::RDOFileType, name:tstring, full_name:tstring, extention:tstring, readonly:rbool, described:rbool }
		RT_REPOSITORY_LOAD,                    // param = rdo::repository::RDOThreadRepository::FileData* = { file_type:rdoModelObjects::RDOFileType, result:rdo::binarystream& }
		RT_REPOSITORY_SAVE,                    // param = rdo::repository::RDOThreadRepository::FileData* = { file_type:rdoModelObjects::RDOFileType, stream:rdo::binarystream& }
		RT_REPOSITORY_LOAD_BINARY,             // param = rdo::repository::RDOThreadRepository::BinaryFile* = { name:tstring, result:rdo::binarystream& }
		RT_REPOSITORY_CREATE_FILE,             // param = rdo::repository::RDOThreadRepository::CreateFile*
		RT_SIMULATOR_PARSE_OK,
		RT_SIMULATOR_PARSE_ERROR,
		RT_SIMULATOR_PARSE_ERROR_SMR,
		RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY,
		RT_SIMULATOR_PARSE_STRING,             // param = tstring*
		RT_SIMULATOR_MODEL_STOP_OK,
		RT_SIMULATOR_MODEL_STOP_BY_USER,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY,
		RT_SIMULATOR_GET_MODEL_STRUCTURE,      // param = result:rdo::textstream*
		RT_SIMULATOR_GET_MODEL_RESULTS,        // param = result:rdo::textstream*
		RT_SIMULATOR_GET_MODEL_RESULTS_INFO,   // param = result:rdo::textstream*
		RT_SIMULATOR_GET_MODEL_EXITCODE,       // param = result:rdo::simulation::report::RDOExitCode*
		RT_SIMULATOR_GET_LIST,                 // param = rdo::service::simulation::RDOThreadSimulator::GetList* = { type:GetList::Type, result:std::list< tstring >* }
		RT_SIMULATOR_GET_ERRORS,               // param = result:std::vector< RDOSyntaxError >*
		RT_RUNTIME_MODEL_START_BEFORE,
		RT_RUNTIME_MODEL_START_AFTER,
		RT_RUNTIME_MODEL_STOP_BEFORE,
		RT_RUNTIME_MODEL_STOP_AFTER,
		RT_RUNTIME_TRACE_STRING,               // param = tstring*
		RT_RUNTIME_GET_MODE,                   // param = result:rdo::runtime::RunTimeMode*
		RT_RUNTIME_SET_MODE,                   // param = mode:rdo::runtime::RunTimeMode*
		RT_RUNTIME_GET_SPEED,                  // param = result:double[0..1]%*
		RT_RUNTIME_SET_SPEED,                  // param = speed:double[0..1]%*
		RT_RUNTIME_GET_SHOWRATE,               // param = result:double[0..+max_double]*
		RT_RUNTIME_SET_SHOWRATE,               // param = show_rate:double[0..+max_double]*
		RT_RUNTIME_GET_TIMENOW,                // param = result:double*
		RT_RUNTIME_GET_FRAME,                  // param = { result:rdo::animation::Frame*, frame_number:int }*
		RT_RUNTIME_GET_LAST_BREAKPOINT,        // param = tstring*
		RT_RUNTIME_KEY_DOWN,                   // param = scanCode:uint*
		RT_RUNTIME_KEY_UP,                     // param = scanCode:uint*
		RT_RUNTIME_FRAME_AREA_DOWN,            // param = area_name:tstring*
		RT_CONVERTOR_NONE,
		RT_CONVERTOR_OK,
		RT_CONVERTOR_ERROR,
		RT_CODECOMP_GET_DATA,                  // param = rdo::service::simulation::RDOThreadCodeComp::GetCodeComp* = { file_type:rdoModelObjects::RDOFileType, pos_x:int, pos_y:int, result:std::list< tstring >& }
		RT_DEBUG_STRING,                       // param = tstring*
		RT_RESULT_STRING,                      // param = tstring*
		RT_CORBA_PARSER_GET_RTP,
		RT_CORBA_PARSER_GET_RSS,
		RT_CORBA_PARSER_GET_RTP_COUNT,
		RT_CORBA_PARSER_GET_RTP_PAR_COUNT,
		RT_PROCGUI_BLOCK_CREATE,
		RT_PROCGUI_BLOCK_PROCESS,
		RT_PROCGUI_BLOCK_TERMINATE
	};
	tstring messageToString(RDOTreadMessage message) {
		switch ( message ) {
			case RT_THREAD_CLOSE                      : return _T("RT_THREAD_CLOSE");
			case RT_THREAD_CONNECTION                 : return _T("RT_THREAD_CONNECTION");
			case RT_THREAD_DISCONNECTION              : return _T("RT_THREAD_DISCONNECTION");
			case RT_THREAD_REGISTERED                 : return _T("RT_THREAD_REGISTERED");
			case RT_THREAD_UNREGISTERED               : return _T("RT_THREAD_UNREGISTERED");
			case RT_THREAD_STOP_AFTER                 : return _T("RT_THREAD_STOP_AFTER");
			case RT_STUDIO_MODEL_NEW                  : return _T("RT_STUDIO_MODEL_NEW");
			case RT_STUDIO_MODEL_OPEN                 : return _T("RT_STUDIO_MODEL_OPEN");
			case RT_STUDIO_MODEL_SAVE                 : return _T("RT_STUDIO_MODEL_SAVE");
			case RT_STUDIO_MODEL_CLOSE                : return _T("RT_STUDIO_MODEL_CLOSE");
			case RT_STUDIO_MODEL_BUILD                : return _T("RT_STUDIO_MODEL_BUILD");
			case RT_STUDIO_MODEL_RUN                  : return _T("RT_STUDIO_MODEL_RUN");
			case RT_STUDIO_MODEL_STOP                 : return _T("RT_STUDIO_MODEL_STOP");
			case RT_STUDIO_MODEL_GET_TEXT             : return _T("RT_STUDIO_MODEL_GET_TEXT");
			case RT_REPOSITORY_MODEL_NEW              : return _T("RT_REPOSITORY_MODEL_NEW");
			case RT_REPOSITORY_MODEL_OPEN             : return _T("RT_REPOSITORY_MODEL_OPEN");
			case RT_REPOSITORY_MODEL_OPEN_ERROR       : return _T("RT_REPOSITORY_MODEL_OPEN_ERROR");
			case RT_REPOSITORY_MODEL_SAVE             : return _T("RT_REPOSITORY_MODEL_SAVE");
			case RT_REPOSITORY_MODEL_OPEN_GET_NAME    : return _T("RT_REPOSITORY_MODEL_OPEN_GET_NAME");
			case RT_REPOSITORY_MODEL_CLOSE            : return _T("RT_REPOSITORY_MODEL_CLOSE");
			case RT_REPOSITORY_MODEL_GET_FILEINFO     : return _T("RT_REPOSITORY_MODEL_GET_FILEINFO");
			case RT_REPOSITORY_LOAD                   : return _T("RT_REPOSITORY_LOAD");
			case RT_REPOSITORY_SAVE                   : return _T("RT_REPOSITORY_SAVE");
			case RT_REPOSITORY_LOAD_BINARY            : return _T("RT_REPOSITORY_LOAD_BINARY");
			case RT_REPOSITORY_CREATE_FILE            : return _T("RT_REPOSITORY_CREATE_FILE");
			case RT_SIMULATOR_PARSE_OK                : return _T("RT_SIMULATOR_PARSE_OK");
			case RT_SIMULATOR_PARSE_ERROR             : return _T("RT_SIMULATOR_PARSE_ERROR");
			case RT_SIMULATOR_PARSE_ERROR_SMR         : return _T("RT_SIMULATOR_PARSE_ERROR_SMR");
			case RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   : return _T("RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY");
			case RT_SIMULATOR_PARSE_STRING            : return _T("RT_SIMULATOR_PARSE_STRING");
			case RT_SIMULATOR_MODEL_STOP_OK           : return _T("RT_SIMULATOR_MODEL_STOP_OK");
			case RT_SIMULATOR_MODEL_STOP_BY_USER      : return _T("RT_SIMULATOR_MODEL_STOP_BY_USER");
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: return _T("RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR");
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY: return _T("RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY");
			case RT_SIMULATOR_GET_MODEL_STRUCTURE     : return _T("RT_SIMULATOR_GET_MODEL_STRUCTURE");
			case RT_SIMULATOR_GET_MODEL_RESULTS       : return _T("RT_SIMULATOR_GET_MODEL_RESULTS");
			case RT_SIMULATOR_GET_MODEL_RESULTS_INFO  : return _T("RT_SIMULATOR_GET_MODEL_RESULTS_INFO");
			case RT_SIMULATOR_GET_MODEL_EXITCODE      : return _T("RT_SIMULATOR_GET_MODEL_EXITCODE");
			case RT_SIMULATOR_GET_LIST                : return _T("RT_SIMULATOR_GET_LIST");
			case RT_SIMULATOR_GET_ERRORS              : return _T("RT_SIMULATOR_GET_ERRORS");
			case RT_RUNTIME_MODEL_START_BEFORE        : return _T("RT_RUNTIME_MODEL_START_BEFORE");
			case RT_RUNTIME_MODEL_START_AFTER         : return _T("RT_RUNTIME_MODEL_START_AFTER");
			case RT_RUNTIME_MODEL_STOP_BEFORE         : return _T("RT_RUNTIME_MODEL_STOP_BEFORE");
			case RT_RUNTIME_MODEL_STOP_AFTER          : return _T("RT_RUNTIME_MODEL_STOP_AFTER");
			case RT_RUNTIME_TRACE_STRING              : return _T("RT_RUNTIME_TRACE_STRING");
			case RT_RUNTIME_GET_MODE                  : return _T("RT_RUNTIME_GET_MODE");
			case RT_RUNTIME_SET_MODE                  : return _T("RT_RUNTIME_SET_MODE");
			case RT_RUNTIME_GET_SPEED                 : return _T("RT_RUNTIME_GET_SPEED");
			case RT_RUNTIME_SET_SPEED                 : return _T("RT_RUNTIME_SET_SPEED");
			case RT_RUNTIME_GET_SHOWRATE              : return _T("RT_RUNTIME_GET_SHOWRATE");
			case RT_RUNTIME_SET_SHOWRATE              : return _T("RT_RUNTIME_SET_SHOWRATE");
			case RT_RUNTIME_GET_TIMENOW               : return _T("RT_RUNTIME_GET_TIMENOW");
			case RT_RUNTIME_GET_FRAME                 : return _T("RT_RUNTIME_GET_FRAME");
			case RT_RUNTIME_GET_LAST_BREAKPOINT       : return _T("RT_RUNTIME_GET_LAST_BREAKPOINT");
			case RT_RUNTIME_KEY_DOWN                  : return _T("RT_RUNTIME_KEY_DOWN");
			case RT_RUNTIME_KEY_UP                    : return _T("RT_RUNTIME_KEY_UP");
			case RT_RUNTIME_FRAME_AREA_DOWN           : return _T("RT_RUNTIME_FRAME_AREA_DOWN");
			case RT_CONVERTOR_NONE                    : return _T("RT_CONVERTOR_NONE");
			case RT_CONVERTOR_OK                      : return _T("RT_CONVERTOR_OK");
			case RT_CONVERTOR_ERROR                   : return _T("RT_CONVERTOR_ERROR");
			case RT_CODECOMP_GET_DATA                 : return _T("RT_CODECOMP_GET_DATA");
			case RT_DEBUG_STRING                      : return _T("RT_DEBUG_STRING");
			case RT_RESULT_STRING                     : return _T("RT_RESULT_STRING");
			case RT_CORBA_PARSER_GET_RTP              : return _T("RT_CORBA_PARSER_GET_RTP");
			case RT_CORBA_PARSER_GET_RSS              : return _T("RT_CORBA_PARSER_GET_RSS");
			case RT_CORBA_PARSER_GET_RTP_COUNT        : return _T("RT_CORBA_PARSER_GET_RTP_COUNT");
			case RT_CORBA_PARSER_GET_RTP_PAR_COUNT    : return _T("RT_CORBA_PARSER_GET_RTP_PAR_COUNT");
			case RT_PROCGUI_BLOCK_CREATE              : return _T("RT_PROCGUI_BLOCK_CREATE");
			case RT_PROCGUI_BLOCK_PROCESS             : return _T("RT_PROCGUI_BLOCK_PROCESS");
			case RT_PROCGUI_BLOCK_TERMINATE           : return _T("RT_PROCGUI_BLOCK_TERMINATE");
			default                                   : return _T("RT_UNKNOWN");
		}
	}

	class RDOMessageInfo {
	friend class RDOThread;
	friend class RDOKernel;
#ifdef RDO_MT
	private:
		enum Type { post = 0, send, manual } type;
		PTR(CEvent) send_event;
		PTR(CMutex) param_lock;
#endif
	public:
		PTR(RDOThread)   from;
		RDOTreadMessage  message;
		PTR(void)        param;
#ifdef RDO_MT
		RDOMessageInfo(PTR(RDOThread) _from, RDOTreadMessage _message, PTR(void) _param, Type _type )
			: from      (_from   )
			, message   (_message)
			, param     (_param  )
			, type      (_type   )
			, send_event(NULL    )
			, param_lock(NULL    )
		{}
		RDOMessageInfo(CREF(RDOMessageInfo) copy)
			: type      (copy.type      )
			, send_event(copy.send_event)
			, from      (copy.from      )
			, message   (copy.message   )
			, param     (copy.param     )
			, param_lock(copy.param_lock)
		{}
		void lock()   { if ( param_lock ) param_lock->Lock();   }
		void unlock() { if ( param_lock ) param_lock->Unlock(); }
#else
		RDOMessageInfo(PTR(RDOThread) _from, RDOTreadMessage _message, PTR(void) _param)
			: from   (_from   )
			, message(_message)
			, param  (_param  )
		{}
		RDOMessageInfo(CREF(RDOMessageInfo) copy)
			: from   (copy.from   )
			, message(copy.message)
			, param  (copy.param  )
		{}
		void lock()   {}
		void unlock() {}
#endif
	};

	tstring      getName        () const         { return thread_name;               }
	ruint        getID          () const         { return thread_id;                 }
#ifdef RDO_MT
	rbool        isGUI          () const         { return thread_fun ? false : true; }
	PTR(CEvent)  getDestroyEvent() const         { return thread_destroy;            }
#endif

/*
	class RDOTreadMethod {
	public:
		RDOThread*   thread;
		tstring  name;
		ruint        index;
	};

	// POST: �������� ��������� ��� �������� ����������
	void postMessage( RDOTreadMessage message, void* pParam = NULL ) {
		postMessageFrom( NULL, message, pParam );
	}
	void postMessageFrom( RDOThread* from, RDOTreadMessage message, void* pParam = NULL ) {
		messages_mutex.Lock();
		messages.push_back( RDOMessageInfo( from, message, pParam, RDOThread::RDOMessageInfo::post ) );
		messages_mutex.Unlock();
	}
*/
	// SEND: �������� ��������� � ��������� ����������
	void sendMessage(PTR(RDOThread) to, RDOTreadMessage message, PTR(void) pParam = NULL)
	{
#ifdef RDO_MT
		RDOMessageInfo msg(this, message, pParam, RDOThread::RDOMessageInfo::send);
		CEvent event;
		msg.send_event = &event;

		to->messages_mutex.Lock();
		if (!to->was_close)
			to->messages.push_back(msg);

		to->messages_mutex.Unlock();

		while (::WaitForSingleObject(event.m_hObject, 0) == WAIT_TIMEOUT)
			processMessages();
#else
		RDOMessageInfo messageInfo(this, message, pParam);
		to->processMessages(messageInfo);
#endif
	}

#ifdef RDO_MT
	// MANUAL: �������� ��������� � '������' ��������� ���������� ��� this
	PTR(CEvent) manualMessageFrom(RDOTreadMessage message, PTR(void) pParam = NULL);
#endif

	// �������� ����������� ���� ������ � ������ �� notifies
	// �����: ������ ���������� ������ ��� this (� ����������� �����)
	void broadcastMessage(RDOTreadMessage message, PTR(void) pParam = NULL, rbool lock = false);

#ifdef TR_TRACE
	static void trace(CREF(tstring) str);
#endif

protected:
#ifdef RDO_MT
	// ���� ��� ����������� �� ������������� ���� � RDO_MT (� thread-safety �� � �������, imho):
	// 1. ������ ����� ����� ������ � ����� ��������� (messages) ����� ������ �����, ����� �������� ���� ����� ���������.
	// 2. ������ ����� ����� ������ � ������ ����������� (notifies), ����� ������, � ���� �� �������� ��������� �����.
	// ������ ��� ����� ������� ����� ������������: map< key = thread*, value = notifies > � ������ �����,
	// � ��� ��� �������� ��������� - �� ������ ��������.
	static ruint threadFun(PTR(void) pParam);
	const RDOThreadFun thread_fun;
	PTR(CWinThread)    thread_win;
	CEvent             proc_create;    // ���������� �� ��������� �����, ����������� ������ ��� ���������
	CEvent             thread_create;  // ���������� �� ������������ �������, ��������� ������ ��� ���������
	PTR(CEvent)        thread_destroy; // ���������� �� ����������� �������
	rbool              broadcast_waiting; // ��� �������, �.�. �������� ������ � ����� �����
	rbool              was_start;         // ��� �������, �.�. �������� ������ � ����� �����
	rbool              was_close;
#endif
	const tstring      thread_name;
	rsint              thread_id;
	rsint              idle_cnt;

	typedef std::vector<RDOTreadMessage> NotifieList;

	NotifieList        notifies; // ������ ���������, �� ������� ��������� �����
	                             // ���� �� ����, �� �������������� ��� ���������.
	                             // RT_THREAD_CLOSE �������������� � RDOThread::processMessages()
	                             // ���� ��������� ���������� �� �� kernel'�, � ��������, ��
	                             // �� ��� �� ����� ��������� ����� ���� ������, � ����� ������� � �������.

#ifdef RDO_MT
	CMutex                         notifies_mutex;

	// ������� ���������
	typedef std::list<RDOMessageInfo> MessageList;
	MessageList      messages;
	CMutex           messages_mutex;

	class BroadcastData {
	public:
		typedef std::vector<PTR(CEvent)> EventList;
		EventList    events;
		ruint        cnt;
		PTR(HANDLE)  handles;

		BroadcastData()
			: cnt    (0   )
			, handles(NULL)
		{}
		BroadcastData(ruint _cnt)
			: cnt(_cnt)
		{
			for (ruint i = 0; i < cnt; i++ )
				events.push_back(new CEvent());
			handles = new HANDLE[cnt];
		}
		BroadcastData(CREF(BroadcastData) copy)
			: cnt    (copy.cnt    )
			, handles(copy.handles)
		{
			events.assign(copy.events.begin(), copy.events.end());
		}
		~BroadcastData()
		{}

		void clear()
		{
			for (ruint i = 0; i < cnt; i++)
				delete events[i];
			delete[] handles;
		}
		void resize()
		{
			if (cnt)
			{
				events.resize(cnt * 2);
				PTR(HANDLE) handles_backup = handles;
				handles = new HANDLE[cnt * 2];
				for (ruint i = 0; i < cnt; i++)
				{
					events.push_back(new CEvent());
					handles[i] = handles_backup[i];
					handles[cnt+i] = new HANDLE;
				}
				delete[] handles_backup;
				cnt *= 2;
			}
			else
			{
				cnt = 10;
				for (ruint i = 0; i < cnt; i++)
					events.push_back(new CEvent());
				handles = new HANDLE[cnt];
			}
		}
	};
	typedef std::vector<BroadcastData> BroadcastDataList;
	BroadcastDataList  broadcast_data;
	ruint              broadcast_cnt;

#endif

#ifdef RDO_MT
	virtual PTR(RDOThread) getKernel();
#else
	PTR(RDOThread) getKernel();
#endif

	// ��������� ����� ������ ����� ��������
#ifdef RDO_MT
	RDOThread(CREF(tstring) _thread_name, RDOThreadFun _thread_fun = NULL);
#else
	RDOThread(CREF(tstring) _thread_name);
#endif
	// ������� ������ ������� �����. kernel ��������� ����� ����������� �����
	virtual ~RDOThread();                                                                          

	// ���� ����������� ������� �� ������������ �������, ����� ��������� ��������� this ��� ����������� �������,
	void after_constructor();

	virtual void  proc (REF(RDOMessageInfo) msg) = 0;
	virtual void  idle ();
	virtual void  start();
	virtual void  stop ();
#ifdef RDO_MT
	virtual rbool processMessages();
#else
	void          processMessages(REF(RDOMessageInfo) msg)
	{
#ifdef TR_TRACE
		RDOThread::trace(_T("---------------- ") + messageToString(msg.message) + _T(": ") + (msg.from ? msg.from->thread_name : _T("NULL")) + _T(" -> ") + thread_name);
#endif
		proc(msg);
		if (msg.message == RT_THREAD_CLOSE)
		{
			if (this != getKernel())
				sendMessage(getKernel(), RT_THREAD_DISCONNECTION);
			stop();
			destroy();
			return;
		}
	}
#endif

	virtual void destroy()
	{
		delete this;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadMT
// --------------------------------------------------------------------------------
// �������� ������� ����� ��� �����, ������� �������� � ����� �����
//
class RDOThreadMT: public RDOThread
{
protected:
#ifdef RDO_MT
	RDOThreadMT(CREF(tstring) _thread_name, RDOThreadFun _thread_fun = RDOThread::threadFun)
		: RDOThread(_thread_name, _thread_fun)
	{}
#else
	RDOThreadMT(CREF(tstring) _thread_name)
		: RDOThread(_thread_name)
	{}
#endif
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadGUI
// --------------------------------------------------------------------------------
// ������� ����� ��� �����, ������� �������� ��������� �� kernel_gui, � �� �� kernel.
// ������������ ��� ���������� ��������� ����� � ��������� ����� kernel_gui
//
#ifdef RDO_MT
class RDOThreadGUI: public RDOThread
{
friend class RDOKernelGUI;
private:
	PTR(RDOThread) kernel_gui;

protected:
	RDOThreadGUI(CREF(tstring) _thread_name, PTR(RDOThread) _kernel_gui)
		: RDOThread (_thread_name)
		, kernel_gui(_kernel_gui )
	{}
	virtual PTR(RDOThread) getKernel      ();
	virtual rbool          processMessages();
};
#else
class RDOThreadGUI: public RDOThread
{
protected:
	RDOThreadGUI(CREF(tstring) _thread_name, PTR(RDOThread))
		: RDOThread(_thread_name)
	{}
};
#endif

#endif // RDO_THREAD_H
