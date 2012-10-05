/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocommon.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      13.06.2009
  \brief     ������������� ������� � ����
  \indent    4T
*/

#ifndef _UTILS_RDOCOMMON_H_
#define _UTILS_RDOCOMMON_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdarg.h>
#include <vector>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

OPEN_RDO_NAMESPACE

	//! ���������� ����������� ��������
	//! \tparam T      - ��� ������������ ����������
	//! \param  v1, v2 - ��������, �� ������� ���������� �����������
	//! \result ����������� ��������
	template< class T >
	T rmin(T v1, T v2)
	{
		return v1 <= v2 ? v1 : v2;
	}

	//! ���������� ������������ ��������
	//! \tparam T      - ��� ������������ ����������
	//! \param  v1, v2 - ��������, �� ������� ���������� ������������
	//! \result ������������ ��������
	template< class T >
	T rmax(T v1, T v2)
	{
		return v1 >= v2 ? v1 : v2;
	}

	//! ������������ ������ �� �������, ���������� <a href="http://ru.wikipedia.org/wiki/Printf">printf</a>
	//! \param  str - �������� ������, ������� ����� ��������� ������� ��������������
	//! \param  ... - ������������ ����� ����������
	//! \result ����������������� �������
	tstring format(CPTR(tchar) str, ...);

	//! ������������ ������ �� �������
	//! \param  str    - �������� ������, ������� ����� ��������� ������� ��������������
	//! \param  params - ������������ �� ������� ����� ���������� (���������� �� ������� �� �����)
	//! \result ����������������� �������
	tstring format(CPTR(tchar) str, REF(va_list) params);

#ifdef COMPILER_VISUAL_STUDIO
	//! ������������ ������ �� �������, ���������� <a href="http://ru.wikipedia.org/wiki/Printf">printf</a>
	//! \param  resource - ����� ���������� �������, ������� ����� ��������� ������� ��������������
	//! \param  ...      - ������������ ����� ����������
	//! \result ����������������� �������
	tstring format(ruint resource, ...);

	//! ������������ ������ �� �������
	//! \param  resource - ����� ���������� �������, ������� ����� ��������� ������� ��������������
	//! \param  params   - ������������ �� ������� ����� ���������� (���������� �� ������� �� �����)
	//! \result ����������������� �������
	tstring format(ruint resource, REF(va_list) params);
#endif // COMPILER_VISUAL_STUDIO

	//! ������� ������ � ������ �������
	//! \param[in, out] str - ���������� ������
	//! \result ������������ � ������ ������� ������
	void toLower  (REF(tstring) str);

	//! ����������� ������� (������, ���������, ������� ������) � ������ � ����� ������
	//! \param[in, out] str - ���������� ������
	//! \result ������ ��� ������ � ������ � �����
	void trim     (REF(tstring) str);

	//! ����������� ������� (������, ���������, ������� ������) � ������ ������
	//! \param[in, out] str - ���������� ������
	//! \result ������ ��� ������ � ������
	void trimLeft (REF(tstring) str);

	//! ����������� ������� (������, ���������, ������� ������) � ����� ������
	//! \param[in, out] str - ���������� ������
	//! \result ������ ��� ������ � �����
	void trimRight(REF(tstring) str);

	//! ��������� ������ � unicode
	//! \param str - ������ � ansi
	//! \result ������ � unicode
	wstring toUnicode(CREF(astring) str);

	//! ��������� ���� ����� �� ��� �����
	//! \param fileName - ������ ��� �����
	//! \result ���� � �����, ��������������� '/'
	tstring extractFilePath(CREF(tstring) fileName);

	//! ��������� �������� � ������
	//! \tparam T     - ��� ��������
	//! \param  value - ��������
	//! \result ��������� ������������� ��������
	template< class T >
	inline tstring toString(T value)
	{
		std::ostringstream str;
		str << value;
		return str.str();
	}

	//! ������� ��� �������� ����������, ������� � ����������
	//! \tparam         T         - ��� ����������
	//! \param[in, out] container - ���������
	template<class T>
	void deleteAllObjects(REF(T) container)
	{
		typename T::reverse_iterator it = container.rbegin();
		while (it != container.rend())
		{
			delete *it;
			++it;
		}
		container.clear();
	}

	//! \class  vector
	//! \tparam T - ��� ��������� ��������
	//! \brief    ������
	//! \details  ������� �� std::vector, � ����� ������� �������. ������:
	//! \code
	//! rdo::vector<ruint> container;
	//! container(1)(3)(5)(2);        // ��������� � ���������� 4-�� ��������
	//! \endcode
	template <class T>
	class vector: public std::vector<T>
	{
	public:
		//! \details ������ ������ ���������
		vector()
		{}

		//! ������ ������ ��������� � �������� � ���� \ref item
		//! \param item - ������� ����������
		vector(CREF(T) item)
		{
			std::vector<T>::push_back(item);
		}

		//! ��������� � ��������� ����� ��������, ����� ������� ������ std::vector::push_back()
		//! \param item - ������� ����������
		//! \result ���������� ��� ���������, ����� ��� �� �������� ����� ���� ������� �����
		REF(vector) operator() (CREF(T) item)
		{
			std::vector<T>::push_back(item);
			return *this;
		}
	};
CLOSE_RDO_NAMESPACE

#include "utils/rdocommon.inl"

#endif // _UTILS_RDOCOMMON_H_
