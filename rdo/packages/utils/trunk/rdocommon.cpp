/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocommon.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      13.06.2009
  \brief     ������������� ������� � ����
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <locale>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/copy.hpp>

#ifdef COMPILER_VISUAL_STUDIO
#	include <windows.h>
#	include <io.h>
#else
#	include <stdarg.h>
#	include <wchar.h>
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning(disable : 4786)
#endif

OPEN_RDO_NAMESPACE

tstring format(CPTR(tchar) str, ...)
{
	va_list params;
	va_start( params, str );
	tstring res = format( str, params );
	va_end( params );
	return res;
}

tstring format( CPTR(tchar) str, REF(va_list) params )
{
	std::vector< tchar > s;
	s.resize( 256 );
	int size = -1;
	while ( size == -1 ) {
#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning(disable: 4996)
#ifdef UNICODE
		size = _vsnwprintf( &s[0], s.size(), str, params );
#else
		size = _vsnprintf( &s[0], s.size(), str, params );
#endif
#	pragma warning(default: 4996)
#endif  // COMPILER_VISUAL_STUDIO

#ifdef COMPILER_GCC
#ifdef UNICODE
		size = vswprintf( &s[0], s.size(), str, params );
#else
		size = vsnprintf( &s[0], s.size(), str, params );
#endif
#endif // COMPILER_GCC
		if ( size == -1 )
		{
			s.resize( s.size() + 256 );
		}
	}
	s.resize( size );
	return tstring( s.begin(), s.end() );
}

#ifdef COMPILER_VISUAL_STUDIO
tstring format(ruint resource, ...)
{
	va_list params;
	va_start( params, resource );
	tstring res = format( resource, params );
	va_end( params );
	return res;
}

tstring format(ruint resource, REF(va_list) params)
{
	tchar buffer[1024];
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule)
	{
		if (LoadString(hModule, resource, buffer, sizeof(buffer)/sizeof(tchar)))
		return format(buffer, params);
	}
	return _T("");
}
#endif // COMPILER_VISUAL_STUDIO

struct _toLower {
	_toLower( std::locale loc ): m_loc(loc) {};
	tchar operator()(tchar c) {
		return std::tolower( c, m_loc );
	}
	std::locale m_loc;
};

void toLower( REF(tstring) str )
{
#ifdef COMPILER_VISUAL_STUDIO
	_toLower tr1( std::locale("rus") );
	std::transform( str.begin(), str.end(), str.begin(), tr1 );
	_toLower tr2( std::locale("C") );
#elif defined COMPILER_GCC
	boost::algorithm::to_lower(str);
#endif // COMPILER_VISUAL_STUDIO
}

void trim( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( 0, str.find_first_not_of( szDelims ) );
	str.erase( str.find_last_not_of( szDelims ) + 1, tstring::npos );
}

void trimLeft( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( 0, str.find_first_not_of( szDelims ) );
}

void trimRight( REF(tstring) str )
{
	static tchar szDelims[] = _T(" \t\n\r");
	str.erase( str.find_last_not_of( szDelims ) + 1, tstring::npos );
}

wstring toUnicode(CREF(astring) str)
{
	try
	{
		std::wostringstream wstm;
		//! @todo � ��� ������ � �� �������� ������� ? �.�. ��������� ��������� ?
		wstm.imbue(std::locale("rus"));
		CREF(std::ctype<wchar_t>) ctfacet =	std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
		for (std::size_t i = 0; i < str.size(); ++i)
		{
			wstm << ctfacet.widen(str[i]);
		}
		return wstm.str();
	}
	catch (CREF(std::runtime_error))
	{
	return wstring();
	}
}

tstring extractFilePath( CREF(tstring) fileName )
{
	tstring s;
	tstring::size_type pos = fileName.find_last_of( _T('\\') );
	if ( pos == tstring::npos )
	{
		pos = fileName.find_last_of( _T('/') );
	}
	if ( pos == tstring::npos )
	{
		return _T("");
	}
	if ( pos != tstring::npos && pos < fileName.length() - 1 ) {
		s.assign( &fileName[0], pos + 1 );
		static tchar szDelims[] = _T(" \t\n\r");
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, tstring::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( _T('\\') );
	if ( pos == tstring::npos ) {
		pos = s.find_last_of( _T('/') );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += _T("/");
	}
	return s;
}

CLOSE_RDO_NAMESPACE
