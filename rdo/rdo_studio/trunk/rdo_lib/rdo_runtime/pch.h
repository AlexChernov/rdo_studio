/*!
  \copyright (c) RDO-Team, 2006
  \file      pch.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ��������� ����������� ������������ ������ C++ � ���
  \indent    4T
*/

#pragma warning(disable : 4786)

#ifndef WINVER               // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
#endif

// ----------------------------------------------------------------------- INCLUDES
#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
#include <algorithm>
#include <list>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdodebug.h"
// --------------------------------------------------------------------------------
