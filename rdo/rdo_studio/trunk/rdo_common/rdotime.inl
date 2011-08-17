/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotime.inl
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

// ====================================================================== INCLUDES
#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
#include <time.h>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdodebug.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

inline Time::Time()
	: m_value(Time::invalid().value())
{}

inline Time::Time(CREF(Value) value)
	: m_value(value)
{}

inline rbool Time::operator== (CREF(Time) time)
{
	return m_value == time.m_value;
}

inline Time::operator Value() const
{
	return m_value;
}

inline tstring Time::asString() const
{
	if (m_value == Time::invalid())
	{
		return _T("[INVALID]");
	}
	else
	{
	    SYSTEMTIME st;
		ASSERT(::FileTimeToSystemTime(reinterpret_cast<CPTR(FILETIME)>(&m_value), &st));
		ruint size;
		//! ������� �����
		const tstring timeFormat(_T("HH:mm:ss"));
		size = ::GetTimeFormat(LOCALE_SYSTEM_DEFAULT, LOCALE_USE_CP_ACP, &st, timeFormat.c_str(), NULL, 0);
		ASSERT(size != 0);
		tstring time;
		time.resize(size);
		::GetTimeFormat(LOCALE_SYSTEM_DEFAULT, LOCALE_USE_CP_ACP, &st, timeFormat.c_str(), &time[0], size);
		//! ������� ����
		const tstring dateTimeFormat = rdo::format(_T("ddd MMM dd %s yyyy"), time.c_str());
		size = ::GetDateFormat(LOCALE_SYSTEM_DEFAULT, LOCALE_USE_CP_ACP, &st, dateTimeFormat.c_str(), NULL, 0);
		ASSERT(size != 0);
		tstring dateTime;
		dateTime.resize(size);
		::GetDateFormat(LOCALE_SYSTEM_DEFAULT, LOCALE_USE_CP_ACP, &st, dateTimeFormat.c_str(), &dateTime[0], size);
		return dateTime;
	}
}

inline CREF(Time::Value) Time::value() const
{
	return m_value;
}

inline Time Time::utc()
{
    SYSTEMTIME st;
	::GetSystemTime(&st);
	Time::Value value; 
	::SystemTimeToFileTime(&st, reinterpret_cast<PTR(FILETIME)>(&value));
	return Time(value);
}

inline Time Time::local()
{
    SYSTEMTIME st;
	::GetLocalTime(&st);
	Time::Value value; 
	::SystemTimeToFileTime(&st, reinterpret_cast<PTR(FILETIME)>(&value));
	return Time(value);
}

inline Time Time::invalid()
{
	return ~0;
}

CLOSE_RDO_NAMESPACE
