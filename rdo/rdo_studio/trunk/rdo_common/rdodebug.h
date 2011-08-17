/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodebug.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

#ifndef _RDODEBUG_H_
#define _RDODEBUG_H_

// ====================================================================== INCLUDES
#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
// ===============================================================================

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

#define  TRACE(A)              ::OutputDebugString(A);
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

#else //! _DEBUG

#define  TRACE(A)
#define TRACE1(A, P1)
#define TRACE2(A, P1, P2)
#define TRACE3(A, P1, P2, P3)
#define ASSERT(A)
#define NEVER_REACH_HERE

#endif

#endif //! _RDODEBUG_H_
