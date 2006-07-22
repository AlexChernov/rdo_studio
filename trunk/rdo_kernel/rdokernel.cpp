#include "rdokernel.h"
#include <rdorepository.h>
#include <rdosimwin.h>

#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RDOKernel* kernel = NULL;

// --------------------------------------------------------------------
// ---------- RDOKernel
// --------------------------------------------------------------------
RDOKernel::RDOKernel():
	RDOThreadMT( "RDOKernel" ),
	thread_simulator( NULL ),
	thread_repository( NULL )
{
	notifies.push_back( RT_THREAD_CONNECTION );
	notifies.push_back( RT_THREAD_DISCONNECTION );
	after_constructor();
	kernel = this;
}

RDOKernel::~RDOKernel()
{
	kernel = NULL;
}

void RDOKernel::init()
{
	new RDOKernel();
}

#ifdef RDO_MT
void RDOKernel::close()
{
#ifdef RDO_MT
	CEvent* thread_destroy = kernel->thread_destroy;

	// ����������� ��������� ��������� RT_THREAD_CLOSE
	CEvent* event = kernel->manualMessageFrom( RDOThread::RT_THREAD_CLOSE );
	while ( ::WaitForSingleObject( event->m_hObject, 0 ) == WAIT_TIMEOUT ) {
		kernel->processMessages();
	}
	delete event;

	// ���������� �����������
	thread_destroy->Lock();
	delete thread_destroy;
#endif
}
#else
void RDOKernel::close()
{
	kernel->sendMessage( kernel, RDOThread::RT_THREAD_CLOSE );
}
#endif

void RDOKernel::start()
{
	RDOThread::start();
#ifdef TR_TRACE
	trace( "kernel ready !!!" );
#endif
}

void RDOKernel::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		// ������� ��� �����
		case RT_THREAD_CLOSE: {
#ifdef TR_TRACE
			trace( thread_name + " stop begin" );
#endif
#ifdef RDO_MT
			threads_mutex.Lock();
#endif
			std::list< RDOThread* >::iterator it = threads.begin();
			while ( it != threads.end() ) {
				RDOThread* thread = *it;
#ifdef RDO_MT
				if ( !thread->isGUI() ) {
					CEvent* thread_destroy = thread->getDestroyEvent();
					threads_mutex.Unlock();
					sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
					thread_destroy->Lock();
					threads_mutex.Lock();
					delete thread_destroy;
					it = threads.begin();
				} else {
					it++;
				}
#else
				sendMessage( thread, RDOThread::RT_THREAD_CLOSE );
				it = threads.begin();
#endif
			}
#ifdef RDO_MT
			threads_mutex.Unlock();
#endif
#ifdef TR_TRACE
			trace ( thread_name + " stop end" );
#endif
			break;
		}
		case RT_THREAD_CONNECTION: {
			if ( msg.from != this ) {
				registration( msg.from );
			}
			break;
		}
		case RT_THREAD_DISCONNECTION: {
			if ( msg.from != this ) {
				unregistered( msg.from );
			}
			break;
		}
		default: break;
	}
}

#ifdef RDO_ST
void RDOKernel::idle()
{
	std::list< RDOThread* >::iterator it = threads.begin();
	while ( it != threads.end() ) {
		// it_next ������������ ��-�� ����, ��� � RDOThreadRunTime->idle() �.�. ������ RDOThreadRunTime � ����� �� threads
		std::list< RDOThread* >::iterator it_next = it;
		it_next++;
		(*it)->idle();
		it = it_next;
	}
}
#endif
// -autorun -autoexit "C:\rdo\rdo_cdrom_1\RAO-cd-rom-1\bin\RAO-explorer\Data\Russian\Models\Barber\Source\Barber.smr"
// -autorun -autoexit "C:\rdo\rdo_cdrom_1\RAO-cd-rom-1\bin\RAO-explorer\Data\Russian\Models\Heidel\Source\Heidel.smr

/*
RDOThread* RDOKernel::find( const std::string& thread_name ) const
{
	threads_mutex.Unlock();
	std::list< RDOThread* >::const_iterator it = threads.begin();
	while ( it != threads.end() ) {
		if ( (*it)->thread_name.compare( thread_name ) == 0 ) {
			RDOThread* thread = *it;
			threads_mutex.Unlock();
			return thread;
		}
		it++;
	}
	threads_mutex.Unlock();
	return NULL;
}

void RDOKernel::sendMessageToThreadByName( const std::string& thread_name, RDOTreadMessage message, void* param )
{
	threads_mutex.Lock();
	RDOThread* thread = find( thread_name );
	if ( thread ) {
		thread->sendMessageFrom( this, message, param );
	}
	threads_mutex.Unlock();
}

void RDOKernel::sendMessageToThreadByNameFrom( RDOThread* from, const std::string& thread_name, RDOTreadMessage message, void* param )
{
	threads_mutex.Lock();
	RDOThread* thread = find( thread_name );
	if ( thread ) {
		thread->sendMessageFrom( from, message, param );
	}
	threads_mutex.Unlock();
}
*/

class CheckThreadID
{
protected:
	int thread_id;
public:
	CheckThreadID( int _thread_id ): thread_id( _thread_id ) {}
	bool operator() ( RDOThread* thread ) { return thread->getID() == thread_id; }
};

void RDOKernel::registration( RDOThread* thread )
{
#ifdef RDO_MT
	threads_mutex.Lock();
#endif
	if ( thread && std::find( threads.begin(), threads.end(), thread ) == threads.end() ) {
#ifdef RDO_MT
		if ( thread->isGUI() ) {
			if ( std::find_if( threads.begin(), threads.end(), CheckThreadID(thread->getID()) ) == threads.end() ) {
				threads.push_back( thread );
			} else {
				threads_mutex.Unlock();
				// �� ����� �������������� �����, ���� ��� GUI, ��� tread_id ������� ��� ���� ������� �����.
				// ��� '������' ����� ������ ���� RDOKernelGUI, ����� ����� ������� ������������ �������
				// � ��������� �� ���������.
				return;
			}
		} else
#endif
		{
			threads.push_back( thread );
		}
	}
	if ( !thread_simulator  && thread->getName() == "RDOThreadSimulator"  ) thread_simulator  = static_cast<rdosim::RDOThreadSimulator*>(thread);
	if ( !thread_repository && thread->getName() == "RDOThreadRepository" ) thread_repository = static_cast<rdoRepository::RDOThreadRepository*>(thread);
#ifdef RDO_MT
	threads_mutex.Unlock();
#endif

#ifdef TR_TRACE
	trace( getName() + " INFO: " + thread->getName() + " REGISTERED" );
#endif
	broadcastMessage( RT_THREAD_REGISTERED, thread );
}

void RDOKernel::unregistered( RDOThread* thread )
{
#ifdef RDO_MT
	threads_mutex.Lock();
#endif
	if ( thread && std::find( threads.begin(), threads.end(), thread ) != threads.end() ) {
		threads.remove( thread );
	} else {
#ifdef RDO_MT
		threads_mutex.Unlock();
#endif
		return;
	}
	if ( thread_simulator  && thread->getName() == "RDOThreadSimulator"  ) thread_simulator  = NULL;
	if ( thread_repository && thread->getName() == "RDOThreadRepository" ) thread_repository = NULL;
#ifdef RDO_MT
	threads_mutex.Unlock();
#endif

#ifdef TR_TRACE
	trace( getName() + " INFO: " + thread->getName() + " UNREGISTERED" );
#endif
	broadcastMessage( RT_THREAD_UNREGISTERED, thread );
}

/*
void RDOKernel::method_registration( RDOTreadMethod& msg )
{
	methods_mutex.Lock();
	msg.index = 0;
	bool insert = true;
	std::list< RDOTreadMethod >::iterator it = methods.begin();
	while ( it != methods.end() ) {
		if ( it->thread == msg.thread && it->name == msg.name ) {
			msg.index = it->index;
			insert = false;
			break;
		} else if ( it->index >= msg.index ) {
			msg.index = it->index + 1;
		}
		it++;
	}
	if ( insert ) methods.push_back( msg );
	methods_mutex.Unlock();
#ifdef TR_TRACE
	trace( rdo::format("Kernel INFO: methods: %s = %d", msg.name.c_str(), msg.index) );
#endif
}
*/

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOKernelGUI
// --------------------------------------------------------------------
RDOKernelGUI::RDOKernelGUI( const std::string& _thread_name ):
	RDOThread( _thread_name )
{
	notifies.push_back( RT_THREAD_CONNECTION );
	notifies.push_back( RT_THREAD_DISCONNECTION );
}

RDOKernelGUI::~RDOKernelGUI()
{
}

void RDOKernelGUI::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		// ������� ������������� �����
		case RT_THREAD_CLOSE: {
#ifdef TR_TRACE
			trace( thread_name + " stop begin" );
#endif
			std::list< RDOThread* >::iterator it = threads.begin();
			while ( it != threads.end() ) {
				RDOThread* thread = *it;
				threads.erase( it );
				thread->stop();
				delete thread;
				it = threads.begin();
			}
#ifdef TR_TRACE
			trace ( thread_name + " stop end" );
#endif
			return;
		}
		case RT_THREAD_CONNECTION: {
			if ( msg.from != this ) {
				registration( msg.from );
				return;
			}
			break;
		}
		case RT_THREAD_DISCONNECTION: {
			if ( msg.from != this ) {
				unregistered( msg.from );
				return;
			}
			break;
		}
		default: break;
	}
	std::list< RDOThread* >::iterator it = threads.begin();
	while ( it != threads.end() ) {
		// it_next ������������ ��-�� ����, ��� � RDOThreadRunTime->idle() �.�. ������ RDOThreadRunTime � ����� �� threads
		std::list< RDOThread* >::iterator it_next = it;
		it_next++;
		if ( *it != msg.from ) {
			(*it)->proc( msg );
		}
		it = it_next;
	}
}

void RDOKernelGUI::idle()
{
	std::list< RDOThread* >::iterator it = threads.begin();
	while ( it != threads.end() ) {
		// it_next ������������ ��-�� ����, ��� � RDOThreadRunTime->idle() �.�. ������ RDOThreadRunTime � ����� �� threads
		std::list< RDOThread* >::iterator it_next = it;
		it_next++;
		(*it)->idle();
		it = it_next;
	}
}

void RDOKernelGUI::registration( RDOThread* thread )
{
	if ( thread && thread->thread_id == thread_id && std::find( threads.begin(), threads.end(), thread ) == threads.end() ) {
		threads.push_back( thread );
#ifdef TR_TRACE
		trace( getName() + " INFO: " + thread->getName() + " REGISTERED" );
#endif
		broadcastMessage( RT_THREAD_REGISTERED, thread );
	}
}

void RDOKernelGUI::unregistered( RDOThread* thread )
{
	if ( thread && std::find( threads.begin(), threads.end(), thread ) != threads.end() ) {
		threads.remove( thread );
#ifdef TR_TRACE
		trace( getName() + " INFO: " + thread->getName() + " UNREGISTERED" );
#endif
		broadcastMessage( RT_THREAD_UNREGISTERED, thread );
	}
}
#endif
