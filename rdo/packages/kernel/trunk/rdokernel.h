#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOKernel
// --------------------------------------------------------------------------------

#ifdef CORBA_ENABLE

namespace rdoCorba {
class RDOThreadCorba;
}

#endif

namespace rdoSimulator {
class RDOThreadSimulator;
class RDOThreadCodeComp;
}

OPEN_RDO_RUNTIME_NAMESPACE
class RDOThreadRunTime;
CLOSE_RDO_RUNTIME_NAMESPACE

namespace rdoRepository {
class RDOThreadRepository;
}

class RDOKernel: public RDOThreadMT
{
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, rbool lock );
friend class RDOStudioApp;

protected:
	RDOKernel();           // �������� � �������� ����� ����������� ������
	virtual ~RDOKernel();

	virtual void proc( RDOMessageInfo& msg );
	virtual void start();

	typedef std::list<PTR(RDOThread)> RDOThreadList;
	RDOThreadList               threads;
#ifdef RDO_MT
	mutable CMutex              threads_mutex;
#endif
//	std::list< RDOTreadMethod > methods;
//	CMutex                      methods_mutex;
//	void method_registration( RDOTreadMethod& msg ); // thread-safety

	RDOThread*                          thread_studio;
	rdo::runtime::RDOThreadRunTime*     thread_runtime;
	rdoSimulator::RDOThreadSimulator*   thread_simulator;
	rdoSimulator::RDOThreadCodeComp*    thread_codecomp;
	rdoRepository::RDOThreadRepository* thread_repository;
	
	
#ifdef CORBA_ENABLE
	rdoCorba::RDOThreadCorba*           thread_corba;
#endif
	

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );

public:
	static void init();
	static void close();

#ifdef RDO_ST
	virtual void idle();
#endif

	RDOThread*                          studio() const     { return thread_studio;     }
	rdo::runtime::RDOThreadRunTime*     runtime() const    { return thread_runtime;    }
	rdoSimulator::RDOThreadSimulator*   simulator() const  { return thread_simulator;  }
	rdoSimulator::RDOThreadCodeComp*    codecomp() const   { return thread_codecomp;   }
	rdoRepository::RDOThreadRepository* repository() const { return thread_repository; }

#ifdef CORBA_ENABLE
	rdoCorba::RDOThreadCorba*           corba() const      { return thread_corba;      }
#endif
};

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- RDOKernelGUI
// --------------------------------------------------------------------------------
// �������� ��������� ������ ��� ����� ���������.
// ������� ��������� ������������� � ��� ������ (RDOThreadGUI).
// ����� ����������� � � ���� ����� ������, � �������� ����������� RDOThreadGUI.
// ���������� ��������� RDOKernelGUI::processMessages() ���������� �� ����� ���������
// ��������� ������ win32-gui ����������, ������ �� OnIdle() ����������, �.�. ���������
// ����� ��������� ���������� �������� (���������). ��� ����, ������ �� ����� RDOKernelGUI,
// ���������� �������� ��������� ��������� ��������� ������.
//
class RDOKernelGUI: public RDOThread
{
friend virtual rbool RDOThreadGUI::processMessages();

protected:
	RDOKernelGUI( CREF(tstring) _thread_name ); // �������� � �������� ����� ��������
	virtual ~RDOKernelGUI();

	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();

	std::list< RDOThread* > threads;

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );
	void update_notifies();
};
#else
typedef RDOKernel RDOKernelGUI;
#endif

// --------------------------------------------------------------------------------
extern RDOKernel* kernel;

#endif // RDO_KERNEL_H
