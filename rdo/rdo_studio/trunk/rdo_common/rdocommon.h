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

#ifndef _LIB_COMMON_RDOCOMMON_H_
#define _LIB_COMMON_RDOCOMMON_H_

#ifdef WIN32
#pragma warning(disable : 4786)
#endif // WIN32

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/model_objects.h"
// --------------------------------------------------------------------------------

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

#ifdef WIN32
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
#endif // WIN32

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
			push_back(item);
		}

		//! ��������� � ��������� ����� ��������, ����� ������� ������ std::vector::push_back()
		//! \param item - ������� ����������
		//! \result ���������� ��� ���������, ����� ��� �� �������� ����� ���� ������� �����
		REF(vector) operator() (CREF(T) item)
		{
			push_back(item);
			return *this;
		}
	};
CLOSE_RDO_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE
	//! ������ �������������
	enum RunTimeMode
	{
		RTM_MaxSpeed,   //!< ������������ ������������������
		RTM_Jump,       //!< ���������� ��������
		RTM_Sync,       //!< ���������� ��������
		RTM_Pause,      //!< ����� �����
		RTM_BreakPoint  //!< ����� ����� ���������
	};
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_SIMULATOR_NAMESPACE
	//! ������ ��������
	enum ShowMode
	{
		SM_NoShow,    //!< ��� ��������
		SM_Animation, //!< � ���������
		SM_Monitor    //!< �������� � ������ �� �����
	};

	//! ��� ��������� ���������� ��� ������� ������
	enum RDOExitCode
	{
		EC_OK = 0,        //!< ������ ���������������� ��� ����������� ��� ������
		EC_ParserError,   //!< ���������� ����������� � �������
		EC_RunTimeError,  //!< ������ ������ ���������� � �������
		EC_UserBreak,     //!< ������ ����������� �������������
		EC_ModelNotFound, //!< ���� ������� ������ �� ������
		EC_NoMoreEvents   //!< ������ ����������� ������, ��� ������ ������ ������������
	};

	//! �������������� ������
	struct RDOSyntaxError
	{
		//! \brief   ��� ������
		//! \details ������ ������ ���� ������ ������������ ��������� ��������� �� ������
		enum ErrorCode
		{
			UNKNOWN = 1, //!< ����������� ������
		};

		ErrorCode                     m_code;    //!< ��� ������
		tstring                       m_message; //!< ��������� �� ������
		rdoModelObjects::RDOFileType  m_file;    //!< ����, � ������� ������� ������
		ruint                         m_line;    //!< ����� ������ � �������
		ruint                         m_pos;     //!< ������� ������ � ������
		rbool                         m_warning; //!< ������� �������������� (\b true - ��������������, \b false - ������)

		//! ����������� ������
		//! \param code    - ��� ������
		//! \param message - ��������� �� ������
		//! \param file    - ����, � ������� ������� ������
		//! \param line    - ����� ������ � �������
		//! \param pos     - ������� ������ � ������
		//! \param warning - ������� ��������������, ����� �������������. �������� �� ��������� \b false
		RDOSyntaxError(ErrorCode code, CREF(tstring) message, ruint line, ruint pos, rdoModelObjects::RDOFileType file, rbool warning = false)
			: m_code   (code   )
			, m_message(message)
			, m_line   (line   )
			, m_pos    (pos    )
			, m_file   (file   )
			, m_warning(warning)
		{}
	};
CLOSE_RDO_SIMULATOR_NAMESPACE

#include "rdo_common/rdocommon.inl"

#endif //! _LIB_COMMON_RDOCOMMON_H_
