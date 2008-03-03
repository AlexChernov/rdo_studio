#ifndef RDO_THREAD_H
#define RDO_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <string>
#include <list>
#include <vector>
#include <afxmt.h>

// --------------------------------------------------------------------
// #define RDOSIM_COMPATIBLE
//
// �������� ����� ��������� �������
// ������������ ��� ���������� ������, ����� �����������, �� ������ ���
// ������� (������ ��� ���� � ������ ���):
//   1. ���������� ������������� ������� �� LIFO
//   2. ����������� ��������� � ����������� ��������
//   3. ����������� ������� ������� ����� ����������� ���� ������� �������
//   4. ������� � ����������� ������������ �������� � ����� ������� ����� ����������, � �� �����

// --------------------------------------------------------------------
// ������������ ��� ���������� (����/�����)-�������� ������ ���
// ���� ���������� � �������� �������
#if defined(RDO_SP)
	#undef RDO_MT
// ���� ���������� � �������� �������
#elif defined(RDO_MT)
	#undef RDO_SP
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

// --------------------------------------------------------------------
// ---------- RDOThread
// --------------------------------------------------------------------
#ifdef RDO_MT
typedef unsigned int (*RDOThreadFun)( void* param );
#endif

class RDOThread
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
		RT_STUDIO_MODEL_NEW,                   // param = rdoRepository::RDOThreadRepository::NewModel* = { model_name:std::string&, model_path:std::string&      }
		RT_STUDIO_MODEL_OPEN,                  // param = rdoRepository::RDOThreadRepository::OpenFile* = { model_name:std::string&, result:bool&, readonly:bool& }
		RT_STUDIO_MODEL_SAVE,
		RT_STUDIO_MODEL_SAVE_AS,
		RT_STUDIO_MODEL_CLOSE,
		RT_STUDIO_MODEL_BUILD,
		RT_STUDIO_MODEL_RUN,
		RT_STUDIO_MODEL_STOP,
		RT_STUDIO_MODEL_GET_TEXT,              // param = rdoRepository::RDOThreadRepository::FileData* =  { file_type:rdoModelObjects::RDOFileType, result:rdo::binarystream& }
		RT_REPOSITORY_MODEL_NEW,               // param = rdoRepository::RDOThreadRepository::NewModel* =  { model_name:std::string&, model_path:std::string&   }
		RT_REPOSITORY_MODEL_OPEN,
		RT_REPOSITORY_MODEL_OPEN_ERROR,        // param = model_name:std::string*
		RT_REPOSITORY_MODEL_SAVE,
		RT_REPOSITORY_MODEL_OPEN_GET_NAME,     // param = rdoRepository::RDOThreadRepository::OpenFile* =  { model_name:std::string, readonly:bool, result:bool }
		RT_REPOSITORY_MODEL_SAVE_GET_NAME,     // param = rdoRepository::RDOThreadRepository::OpenFile* = { model_name:std::string, readonly:bool, result:bool }
		RT_REPOSITORY_MODEL_CLOSE,
		RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE,   // param = result:bool*, �������� ��� �
		RT_REPOSITORY_MODEL_CLOSE_ERROR,
		RT_REPOSITORY_MODEL_GET_FILEINFO,      // param = rdoRepository::RDOThreadRepository::FileInfo* = { type:rdoModelObjects::RDOFileType, name:std::string, full_name:std::string, extention:std::string, readonly:bool, described:bool }
		RT_REPOSITORY_LOAD,                    // param = rdoRepository::RDOThreadRepository::FileData* = { file_type:rdoModelObjects::RDOFileType, result:rdo::binarystream& }
		RT_REPOSITORY_SAVE,                    // param = rdoRepository::RDOThreadRepository::FileData* = { file_type:rdoModelObjects::RDOFileType, stream:rdo::binarystream& }
		RT_REPOSITORY_LOAD_BINARY,             // param = rdoRepository::RDOThreadRepository::BinaryFile* = { name:std::string, result:rdo::binarystream& }
		RT_SIMULATOR_PARSE_OK,
		RT_SIMULATOR_PARSE_ERROR,
		RT_SIMULATOR_PARSE_ERROR_SMR,
		RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY,
		RT_SIMULATOR_PARSE_STRING,             // param = std::string*
		RT_SIMULATOR_MODEL_STOP_OK,
		RT_SIMULATOR_MODEL_STOP_BY_USER,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR,
		RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY,
		RT_SIMULATOR_GET_MODEL_STRUCTURE,      // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_RESULTS,        // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_RESULTS_INFO,   // param = result:std::stringstream*
		RT_SIMULATOR_GET_MODEL_EXITCODE,       // param = result:rdoSimulator::RDOExitCode*
		RT_SIMULATOR_GET_LIST,                 // param = rdoSimulator::RDOThreadSimulator::GetList* = { type:GetList::Type, result:std::list< std::string >* }
		RT_SIMULATOR_GET_ERRORS,               // param = result:std::vector< RDOSyntaxError >*
		RT_RUNTIME_MODEL_START_BEFORE,
		RT_RUNTIME_MODEL_START_AFTER,
		RT_RUNTIME_MODEL_STOP_BEFORE,
		RT_RUNTIME_MODEL_STOP_AFTER,
		RT_RUNTIME_TRACE_STRING,               // param = std::string*
		RT_RUNTIME_GET_MODE,                   // param = result:rdoRuntime::RunTimeMode*
		RT_RUNTIME_SET_MODE,                   // param = mode:rdoRuntime::RunTimeMode*
		RT_RUNTIME_GET_SPEED,                  // param = result:double[0..1]%*
		RT_RUNTIME_SET_SPEED,                  // param = speed:double[0..1]%*
		RT_RUNTIME_GET_SHOWRATE,               // param = result:double[0..+max_double]*
		RT_RUNTIME_SET_SHOWRATE,               // param = show_rate:double[0..+max_double]*
		RT_RUNTIME_GET_TIMENOW,                // param = result:double*
		RT_RUNTIME_GET_FRAME,                  // param = { result:rdoSimulator::RDOFrame*, frame_number:int }*
		RT_RUNTIME_GET_LAST_BREAKPOINT,        // param = std::string*
		RT_RUNTIME_KEY_DOWN,                   // param = scan_code:uint*
		RT_RUNTIME_KEY_UP,                     // param = scan_code:uint*
		RT_RUNTIME_FRAME_AREA_DOWN,            // param = area_name:std::string*
		RT_CODECOMP_GET_DATA,                  // param = rdoSimulator::RDOThreadCodeComp::GetCodeComp* = { file_type:rdoModelObjects::RDOFileType, pos_x:int, pos_y:int, result:std::list< std::string >& }
		RT_DEBUG_STRING                        // param = std::string*
	};
	std::string messageToString( RDOTreadMessage message ) {
		switch ( message ) {
			case RT_THREAD_CLOSE                      : return "RT_THREAD_CLOSE";
			case RT_THREAD_CONNECTION                 : return "RT_THREAD_CONNECTION";
			case RT_THREAD_DISCONNECTION              : return "RT_THREAD_DISCONNECTION";
			case RT_THREAD_REGISTERED                 : return "RT_THREAD_REGISTERED";
			case RT_THREAD_UNREGISTERED               : return "RT_THREAD_UNREGISTERED";
			case RT_THREAD_STOP_AFTER                 : return "RT_THREAD_STOP_AFTER";
			case RT_STUDIO_MODEL_NEW                  : return "RT_STUDIO_MODEL_NEW";
			case RT_STUDIO_MODEL_OPEN                 : return "RT_STUDIO_MODEL_OPEN";
			case RT_STUDIO_MODEL_SAVE                 : return "RT_STUDIO_MODEL_SAVE";
			case RT_STUDIO_MODEL_SAVE_AS              : return "RT_STUDIO_MODEL_SAVE_AS";
			case RT_STUDIO_MODEL_CLOSE                : return "RT_STUDIO_MODEL_CLOSE";
			case RT_STUDIO_MODEL_BUILD                : return "RT_STUDIO_MODEL_BUILD";
			case RT_STUDIO_MODEL_RUN                  : return "RT_STUDIO_MODEL_RUN";
			case RT_STUDIO_MODEL_STOP                 : return "RT_STUDIO_MODEL_STOP";
			case RT_STUDIO_MODEL_GET_TEXT             : return "RT_STUDIO_MODEL_GET_TEXT";
			case RT_REPOSITORY_MODEL_NEW              : return "RT_REPOSITORY_MODEL_NEW";
			case RT_REPOSITORY_MODEL_OPEN             : return "RT_REPOSITORY_MODEL_OPEN";
			case RT_REPOSITORY_MODEL_OPEN_ERROR       : return "RT_REPOSITORY_MODEL_OPEN_ERROR";
			case RT_REPOSITORY_MODEL_SAVE             : return "RT_REPOSITORY_MODEL_SAVE";
			case RT_REPOSITORY_MODEL_OPEN_GET_NAME    : return "RT_REPOSITORY_MODEL_OPEN_GET_NAME";
			case RT_REPOSITORY_MODEL_SAVE_GET_NAME    : return "RT_REPOSITORY_MODEL_SAVE_GET_NAME";
			case RT_REPOSITORY_MODEL_CLOSE            : return "RT_REPOSITORY_MODEL_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  : return "RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE";
			case RT_REPOSITORY_MODEL_CLOSE_ERROR      : return "RT_REPOSITORY_MODEL_CLOSE_ERROR";
			case RT_REPOSITORY_MODEL_GET_FILEINFO     : return "RT_REPOSITORY_MODEL_GET_FILEINFO";
			case RT_REPOSITORY_LOAD                   : return "RT_REPOSITORY_LOAD";
			case RT_REPOSITORY_SAVE                   : return "RT_REPOSITORY_SAVE";
			case RT_REPOSITORY_LOAD_BINARY            : return "RT_REPOSITORY_LOAD_BINARY";
			case RT_SIMULATOR_PARSE_OK                : return "RT_SIMULATOR_PARSE_OK";
			case RT_SIMULATOR_PARSE_ERROR             : return "RT_SIMULATOR_PARSE_ERROR";
			case RT_SIMULATOR_PARSE_ERROR_SMR         : return "RT_SIMULATOR_PARSE_ERROR_SMR";
			case RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   : return "RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY";
			case RT_SIMULATOR_PARSE_STRING            : return "RT_SIMULATOR_PARSE_STRING";
			case RT_SIMULATOR_MODEL_STOP_OK           : return "RT_SIMULATOR_MODEL_STOP_OK";
			case RT_SIMULATOR_MODEL_STOP_BY_USER      : return "RT_SIMULATOR_MODEL_STOP_BY_USER";
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: return "RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR";
			case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY: return "RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY";
			case RT_SIMULATOR_GET_MODEL_STRUCTURE     : return "RT_SIMULATOR_GET_MODEL_STRUCTURE";
			case RT_SIMULATOR_GET_MODEL_RESULTS       : return "RT_SIMULATOR_GET_MODEL_RESULTS";
			case RT_SIMULATOR_GET_MODEL_RESULTS_INFO  : return "RT_SIMULATOR_GET_MODEL_RESULTS_INFO";
			case RT_SIMULATOR_GET_MODEL_EXITCODE      : return "RT_SIMULATOR_GET_MODEL_EXITCODE";
			case RT_SIMULATOR_GET_LIST                : return "RT_SIMULATOR_GET_LIST";
			case RT_SIMULATOR_GET_ERRORS              : return "RT_SIMULATOR_GET_ERRORS";
			case RT_RUNTIME_MODEL_START_BEFORE        : return "RT_RUNTIME_MODEL_START_BEFORE";
			case RT_RUNTIME_MODEL_START_AFTER         : return "RT_RUNTIME_MODEL_START_AFTER";
			case RT_RUNTIME_MODEL_STOP_BEFORE         : return "RT_RUNTIME_MODEL_STOP_BEFORE";
			case RT_RUNTIME_MODEL_STOP_AFTER          : return "RT_RUNTIME_MODEL_STOP_AFTER";
			case RT_RUNTIME_TRACE_STRING              : return "RT_RUNTIME_TRACE_STRING";
			case RT_RUNTIME_GET_MODE                  : return "RT_RUNTIME_GET_MODE";
			case RT_RUNTIME_SET_MODE                  : return "RT_RUNTIME_SET_MODE";
			case RT_RUNTIME_GET_SPEED                 : return "RT_RUNTIME_GET_SPEED";
			case RT_RUNTIME_SET_SPEED                 : return "RT_RUNTIME_SET_SPEED";
			case RT_RUNTIME_GET_SHOWRATE              : return "RT_RUNTIME_GET_SHOWRATE";
			case RT_RUNTIME_SET_SHOWRATE              : return "RT_RUNTIME_SET_SHOWRATE";
			case RT_RUNTIME_GET_TIMENOW               : return "RT_RUNTIME_GET_TIMENOW";
			case RT_RUNTIME_GET_FRAME                 : return "RT_RUNTIME_GET_FRAME";
			case RT_RUNTIME_GET_LAST_BREAKPOINT       : return "RT_RUNTIME_GET_LAST_BREAKPOINT";
			case RT_RUNTIME_KEY_DOWN                  : return "RT_RUNTIME_KEY_DOWN";
			case RT_RUNTIME_KEY_UP                    : return "RT_RUNTIME_KEY_UP";
			case RT_RUNTIME_FRAME_AREA_DOWN           : return "RT_RUNTIME_FRAME_AREA_DOWN";
			case RT_CODECOMP_GET_DATA                 : return "RT_CODECOMP_GET_DATA";
			case RT_DEBUG_STRING                      : return "RT_DEBUG_STRING";
			default                                   : return "RT_UNKNOWN";
		}
	}

	class RDOMessageInfo {
	friend class RDOThread;
	friend class RDOKernel;
#ifdef RDO_MT
	private:
		enum Type { post = 0, send, manual } type;
		CEvent* send_event;
		CMutex* param_lock;
#endif
	public:
		RDOThread*      from;
		RDOTreadMessage message;
		void*           param;
#ifdef RDO_MT
		RDOMessageInfo( RDOThread* _from, RDOTreadMessage _message, void* _param, Type _type ): from( _from ), message( _message ), param( _param ), type( _type ), send_event( NULL ), param_lock( NULL ) {}
		RDOMessageInfo( const RDOMessageInfo& copy ): type( copy.type ), send_event( copy.send_event ), from( copy.from ), message( copy.message ), param( copy.param ), param_lock( copy.param_lock ) {}
		void lock()   { if ( param_lock ) param_lock->Lock();   }
		void unlock() { if ( param_lock ) param_lock->Unlock(); }
#else
		RDOMessageInfo( RDOThread* _from, RDOTreadMessage _message, void* _param ): from( _from ), message( _message ), param( _param ) {}
		RDOMessageInfo( const RDOMessageInfo& copy ): from( copy.from ), message( copy.message ), param( copy.param ) {}
		void lock()   {}
		void unlock() {}
#endif
	};

	std::string getName() const         { return thread_name;               }
	int         getID() const           { return thread_id;                 }
#ifdef RDO_MT
	bool        isGUI() const           { return thread_fun ? false : true; }
	CEvent*     getDestroyEvent() const { return thread_destroy;            }
#endif

/*
	class RDOTreadMethod {
	public:
		RDOThread*   thread;
		std::string  name;
		unsigned int index;
	};

	// POST: �������� ��������� ��� �������� ����������
	void postMessage( RDOTreadMessage message, void* param = NULL ) {
		postMessageFrom( NULL, message, param );
	}
	void postMessageFrom( RDOThread* from, RDOTreadMessage message, void* param = NULL ) {
		messages_mutex.Lock();
		messages.push_back( RDOMessageInfo( from, message, param, RDOThread::RDOMessageInfo::post ) );
		messages_mutex.Unlock();
	}
*/
	// SEND: �������� ��������� � ��������� ����������
	void sendMessage( RDOThread* to, RDOTreadMessage message, void* param = NULL ) {
#ifdef RDO_MT
		RDOMessageInfo msg( this, message, param, RDOThread::RDOMessageInfo::send );
		CEvent event;
		msg.send_event = &event;

		to->messages_mutex.Lock();
		to->messages.push_back( msg );
		to->messages_mutex.Unlock();

		while ( ::WaitForSingleObject( event.m_hObject, 0 ) == WAIT_TIMEOUT ) {
			processMessages();
		}
#else
		to->processMessages( RDOMessageInfo( this, message, param ) );
#endif
	}

#ifdef RDO_MT
	// MANUAL: �������� ��������� � '������' ��������� ���������� ��� this
	CEvent* manualMessageFrom( RDOTreadMessage message, void* param = NULL );
#endif

	// �������� ����������� ���� ������ � ������ �� notifies
	// �����: ������ ���������� ������ ��� this (� ����������� �����)
	void broadcastMessage( RDOTreadMessage message, void* param = NULL, bool lock = false );

#ifdef TR_TRACE
	static void trace( const std::string& str );
#endif

protected:
#ifdef RDO_MT
	// ���� ��� ����������� �� ������������� ���� � RDO_MT (� thread-safety �� � �������, imho):
	// 1. ������ ����� ����� ������ � ����� ��������� (messages) ����� ������ �����, ����� �������� ���� ����� ���������.
	// 2. ������ ����� ����� ������ � ������ ����������� (notifies), ����� ������, � ���� �� �������� ��������� �����.
	// ������ ��� ����� ������� ����� ������������: map< key = thread*, value = notifies > � ������ �����,
	// � ��� ��� �������� ��������� - �� ������ ��������.
	static unsigned int threadFun( void* param );
	const RDOThreadFun thread_fun;
	CWinThread*        thread_win;
	CEvent             proc_create;    // ���������� �� ��������� �����, ����������� ������ ��� ���������
	CEvent             thread_create;  // ���������� �� ������������ �������, ��������� ������ ��� ���������
	CEvent*            thread_destroy; // ���������� �� ����������� �������
	bool               broadcast_waiting; // ��� �������, �.�. �������� ������ � ����� �����
	bool               was_start;         // ��� �������, �.�. �������� ������ � ����� �����
	bool               was_close;
#endif
	const std::string  thread_name;
	int                thread_id;
	int                idle_cnt;

	std::vector< RDOTreadMessage > notifies; // ������ ���������, �� ������� ��������� �����
	                                         // ���� �� ����, �� �������������� ��� ���������.
											 // RT_THREAD_CLOSE �������������� � RDOThread::processMessages()
	                                         // ���� ��������� ���������� �� �� kernel'�, � ��������, ��
											 // �� ��� �� ����� ��������� ����� ���� ������, � ����� ������� � �������.
#ifdef RDO_MT
	CMutex                         notifies_mutex;

	// ������� ���������
	std::list< RDOMessageInfo > messages;
	CMutex                      messages_mutex;

	class BroadcastData {
	public:
		int cnt;
		std::vector< CEvent* > events;
		HANDLE*                handles;
		BroadcastData(): cnt( 0 ), handles( NULL ) {
		}
		BroadcastData( int _cnt ): cnt( _cnt ) {
			for ( int i = 0; i < cnt; i++ ) {
				events.push_back( new CEvent() );
			}
			handles = new HANDLE[cnt];
		}
		BroadcastData( const BroadcastData& copy ): cnt( copy.cnt ), handles( copy.handles ) {
			events.assign( copy.events.begin(), copy.events.end() );
		}
		~BroadcastData() {
		}
		void clear() {
			for ( int i = 0; i < cnt; i++ ) {
				delete events[i];
			}
			delete[] handles;
		}
		void resize() {
			if ( cnt ) {
				events.resize( cnt * 2 );
				HANDLE* handles_backup = handles;
				handles = new HANDLE[ cnt * 2 ];
				for ( int i = 0; i < cnt; i++ ) {
					events.push_back( new CEvent() );
					handles[i] = handles_backup[i];
					handles[cnt+i] = new HANDLE;
				}
				delete[] handles_backup;
				cnt *= 2;
			} else {
				cnt = 10;
				for ( int i = 0; i < cnt; i++ ) {
					events.push_back( new CEvent() );
				}
				handles = new HANDLE[cnt];
			}
		}
	};
	std::vector< BroadcastData > broadcast_data;
	int                          broadcast_cnt;

#endif

#ifdef RDO_MT
	virtual RDOThread* getKernel();
#else
	RDOThread* getKernel();
#endif

	// ��������� ����� ������ ����� ��������
#ifdef RDO_MT
	RDOThread( const std::string& _thread_name, RDOThreadFun _thread_fun = NULL );
#else
	RDOThread( const std::string& _thread_name );
#endif
	// ������� ������ ������� �����. kernel ��������� ����� ����������� �����
	virtual ~RDOThread();                                                                          

	// ���� ����������� ������� �� ������������ �������, ����� ��������� ��������� this ��� ����������� �������,
	void after_constructor();

	virtual void proc( RDOMessageInfo& msg ) = 0;
	virtual void idle();
	virtual void start();
	virtual void stop();
#ifdef RDO_MT
	virtual bool processMessages();
#else
	void processMessages( RDOMessageInfo& msg ) {
#ifdef TR_TRACE
		RDOThread::trace( "---------------- " + messageToString(msg.message) + ": " + (msg.from ? msg.from->thread_name : "NULL") + " -> " + thread_name );
#endif
		proc( msg );
		if ( msg.message == RT_THREAD_CLOSE ) {
			if ( this != getKernel() ) sendMessage( getKernel(), RT_THREAD_DISCONNECTION );
			stop();
			delete this;
			return;
		}
	}
#endif
};

// --------------------------------------------------------------------
// ---------- RDOThreadMT
// --------------------------------------------------------------------
// �������� ������� ����� ��� �����, ������� �������� � ����� �����
//
class RDOThreadMT: public RDOThread
{
protected:
#ifdef RDO_MT
	RDOThreadMT( const std::string& _thread_name, RDOThreadFun _thread_fun = RDOThread::threadFun ): RDOThread( _thread_name, _thread_fun ) {}
#else
	RDOThreadMT( const std::string& _thread_name ): RDOThread( _thread_name ) {}
#endif
};

// --------------------------------------------------------------------
// ---------- RDOThreadGUI
// --------------------------------------------------------------------
// ������� ����� ��� �����, ������� �������� ��������� �� kernel_gui, � �� �� kernel.
// ������������ ��� ���������� ��������� ����� � ��������� ����� kernel_gui
//
#ifdef RDO_MT
class RDOThreadGUI: public RDOThread
{
private:
	RDOThread* kernel_gui;

protected:
	RDOThreadGUI( const std::string& _thread_name, RDOThread* _kernel_gui ): RDOThread( _thread_name ), kernel_gui( _kernel_gui ) {}
	virtual RDOThread* getKernel();
	virtual bool processMessages();
};
#else
class RDOThreadGUI: public RDOThread
{
protected:
	RDOThreadGUI( const std::string& _thread_name, RDOThread* ): RDOThread( _thread_name ) {}
};
#endif

#endif // RDO_THREAD_H
