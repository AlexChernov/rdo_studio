/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      app/rdo_studio/src/thread.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      15.07.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_THREAD_H_
#define _RDO_STUDIO_THREAD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
// --------------------------------------------------------------------------------

#ifdef RDO_MT
// --------------------------------------------------------------------------------
// -------------------- ThreadStudio
// --------------------------------------------------------------------------------
class ThreadStudio: public RDOThread
{
friend class Application;
protected:
	ThreadStudio();
	virtual ~ThreadStudio() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- ThreadStudioGUI
// --------------------------------------------------------------------------------
class ThreadStudioGUI: public RDOKernelGUI
{
friend class Application;
protected:
	ThreadStudioGUI();
	virtual ~ThreadStudioGUI() {}; // ����� ������ ���� ������� ����� delete
};
#endif

/*
// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio1
// --------------------------------------------------------------------------------
class RDOThreadStudio1: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio1();
	virtual ~RDOThreadStudio1() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadStudio2
// --------------------------------------------------------------------------------
class RDOThreadStudio2: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio2();
	virtual ~RDOThreadStudio2() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};
*/

#endif // _RDO_STUDIO_THREAD_H_
