/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodebug.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     ���������� �������� ��� �������
  \indent    4T
*/

#ifndef _LIB_COMMON_RDODEBUG_H_
#define _LIB_COMMON_RDODEBUG_H_

// ----------------------------------------------------------------------- PLATFORM
#include "rdo_common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef OST_WINDOWS
	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
#else
	#include <iostream>
	#include <assert.h>
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef TRACE
#undef TRACE
#endif
#ifdef TRACE1
#undef TRACE1
#endif
#ifdef TRACE2
#undef TRACE2
#endif
#ifdef TRACE3
#undef TRACE3
#endif
#ifdef ASSERT
#undef ASSERT
#endif

#ifdef _DEBUG

#ifdef COMPILER_VISUAL_STUDIO

#define TRACE(A)               ::OutputDebugString(A);
#define TRACE1(A, P1)          ::OutputDebugString(rdo::format(A, P1).c_str());
#define TRACE2(A, P1, P2)      ::OutputDebugString(rdo::format(A, P1, P2).c_str());
#define TRACE3(A, P1, P2, P3)  ::OutputDebugString(rdo::format(A, P1, P2, P3).c_str());

#define ASSERT(A) \
	if (!(A)) \
	{ \
		::OutputDebugString(rdo::format(_T("ASSERT: line %d, source '%s', condition '%s'\n"), __LINE__, __FILE__, #A).c_str()); \
		::DebugBreak(); \
	}

#define NEVER_REACH_HERE ASSERT(false)

#else // not COMPILER_VISUAL_STUDIO

void NewOutputDebugString(CREF(tstring) A)
{
	std::cout << A;
}

#define TRACE(A)                NewOutputDebugString(A);
#define TRACE1(A, P1)           NewOutputDebugString(rdo::format(A, P1).c_str());
#define TRACE2(A, P1, P2)       NewOutputDebugString(rdo::format(A, P1, P2).c_str());
#define TRACE3(A, P1, P2, P3)   NewOutputDebugString(rdo::format(A, P1, P2, P3).c_str());
#define ASSERT(A)               assert(A);

#define NEVER_REACH_HERE ASSERT(false)

#endif // COMPILER_VISUAL_STUDIO

#else // not _DEBUG

#define  TRACE(A)
#define TRACE1(A, P1)
#define TRACE2(A, P1, P2)
#define TRACE3(A, P1, P2, P3)
#define ASSERT(A)
#define NEVER_REACH_HERE

#endif // _DEBUG

//! \def TRACE(A)
//! ������� ��������� � ���� Output Visial Studio. � ������ ���������� \b release ��������� �� ����.
//! \param A - ������ ������

//! \def TRACE1(A, P1)
//! ������� ��������� � ���� Output Visial Studio, ��������� ���� �������� ��� ������������ �������� ������. � ������ ���������� \b release ��������� �� ����.
//! \param A  - ������ ������, ������ ��������� �������� � ����� <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> ��� ������ ���������
//! \param P1 - �������� ��� ������� � �������������� ������

//! \def TRACE2(A, P1, P2)
//! ������� ��������� � ���� Output Visial Studio, ��������� ��� ��������� ��� ������������ �������� ������. � ������ ���������� \b release ��������� �� ����.
//! \param A      - ������ ������, ������ ��������� �������� � ����� <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> ��� ���� ����������
//! \param P1, P2 - ��������� ��� ������� � �������������� ������

//! \def TRACE3(A, P1, P2, P3)
//! ������� ��������� � ���� Output Visial Studio, ��������� ��� ��������� ��� ������������ �������� ������. � ������ ���������� \b release ��������� �� ����.
//! \param A          - ������ ������, ������ ��������� �������� � ����� <a href="http://ru.wikipedia.org/wiki/Printf">printf</a> ��� ��� ����������
//! \param P1, P2, P3 - ��������� ��� ������� � �������������� ������

//! \def ASSERT(A)
//! ���� �������� A ����� \b false, �� ������� ��������� �� ������ � ���� Output Visial Studio � ������ ����������. � ������ ���������� \b release ��������� �� ����, �.�. <b>������ ������ ������ ����������� ����� ������� ��� ������ ������ ���</b>. ������:
//! \code
//! LPIThreadProxy pThreadProxy = pRuntime->getThreadProxy();
//! ASSERT(pThreadProxy);
//! // �������� ��������� pThreadProxy �� ����� ����, ����� ��� ������������
//! pThreadProxy->sendMessage(IThreadProxy::TID_REPOSITORY, RDOThread::RT_REPOSITORY_CREATE_FILE, &file);
//! \endcode
//! \param A - ����������� ��������

//! \def NEVER_REACH_HERE
//! ����������� � ��� ������ ����, �� ������� �� ������ ����� ���� ��� ������ ���������. ������ ASSERT(false). � ������ ���������� \b release ��������� �� ����.

#endif // _LIB_COMMON_RDODEBUG_H_
