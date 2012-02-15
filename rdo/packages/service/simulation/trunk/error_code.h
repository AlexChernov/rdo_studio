/*!
  \copyright (c) RDO-Team, 2012
  \file      error_code.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     ���� ��������� �������������
  \indent    4T
*/

#ifndef _SIMULATOR_SERVICE_ERROR_CODE_H_
#define _SIMULATOR_SERVICE_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATOR_NAMESPACE

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

#endif // _SIMULATOR_SERVICE_ERROR_CODE_H_
