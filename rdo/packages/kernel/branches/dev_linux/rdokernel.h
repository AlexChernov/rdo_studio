#ifndef RDO_KERNEL_H
#define RDO_KERNEL_H

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#if _MSC_VER > 1000
	#pragma once
	#endif
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
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

namespace rdoRuntime {
class RDOThreadRunTime;
}

namespace rdoRepository {
class RDOThreadRepository;
}

class RDOKernel: public RDOThreadMT
{
friend void RDOThread::broadcastMessage( RDOTreadMessage message, void* param, bool lock );
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
	rdoRuntime::RDOThreadRunTime*       thread_runtime;
	rdoSimulator::RDOThreadSimulator*   thread_simulator;
	rdoSimulator::RDOThreadCodeComp*    thread_codecomp;
	rdoRepository::RDOThreadRepository* thread_repository;
	
	
#ifdef CORBA_ENABLE
	rdoCorba::RDOThreadCorba*           thread_corba;
#endif
	

	void registration( RDOThread* thread );
	void unregistered( RDOThread* thread );

#ifdef RDO_ST
	virtual void idle();
#endif

public:
	static void init();
	static void close();

	RDOThread*                          studio() const     { return thread_studio;     }
	rdoRuntime::RDOThreadRunTime*       runtime() const    { return thread_runtime;    }
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
friend virtual bool RDOThreadGUI::processMessages();

protected:
	RDOKernelGUI( const std::string& _thread_name ); // �������� � �������� ����� ��������
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
