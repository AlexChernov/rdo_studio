#ifndef RDO_STUDIO_THREAD_H
#define RDO_STUDIO_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_kernel/rdothread.h"
#include "rdo_kernel/rdokernel.h"

#ifdef RDO_MT
// --------------------------------------------------------------------
// ---------- RDOThreadStudio
// --------------------------------------------------------------------
class RDOThreadStudio: public RDOThread
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio();
	virtual ~RDOThreadStudio() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------
// ---------- RDOThreadStudioGUI
// --------------------------------------------------------------------
class RDOThreadStudioGUI: public RDOKernelGUI
{
friend class RDOStudioApp;
protected:
	RDOThreadStudioGUI();
	virtual ~RDOThreadStudioGUI() {}; // ����� ������ ���� ������� ����� delete
};
#endif

/*
// --------------------------------------------------------------------
// ---------- RDOThreadStudio1
// --------------------------------------------------------------------
class RDOThreadStudio1: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio1();
	virtual ~RDOThreadStudio1() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};

// --------------------------------------------------------------------
// ---------- RDOThreadStudio2
// --------------------------------------------------------------------
class RDOThreadStudio2: public RDOThreadMT
{
friend class RDOStudioApp;
protected:
	RDOThreadStudio2();
	virtual ~RDOThreadStudio2() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};
*/

#endif // RDO_STUDIO_THREAD_H
