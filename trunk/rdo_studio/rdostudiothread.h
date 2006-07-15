#ifndef RDO_STUDIO_THREAD_H
#define RDO_STUDIO_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdothread.h>

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
class RDOThreadStudioGUI: public RDOThread
{
friend class RDOStudioApp;
protected:
	RDOThreadStudioGUI();
	virtual ~RDOThreadStudioGUI() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
};

#endif // RDO_STUDIO_THREAD_H
