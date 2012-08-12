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

OPEN_RDO_SERVICE_SIMULATION_NAMESPACE

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
struct RDOSyntaxMessage
{
	enum Type
	{
		MESSAGE_ERROR = 0,
		MESSAGE_WARNING
	};

	tstring                       m_message; //!< ��������� �� ������
	rdoModelObjects::RDOFileType  m_file;    //!< ����, � ������� ������� ������
	ruint                         m_line;    //!< ����� ������ � �������
	ruint                         m_pos;     //!< ������� ������ � ������
	Type                          m_type;    //!< ��� ���������

	//! ����������� ������
	//! \param message - ��������� �� ������
	//! \param file    - ����, � ������� ������� ������
	//! \param line    - ����� ������ � �������
	//! \param pos     - ������� ������ � ������
	//! \param type    - ��� ���������. �������� �� ��������� \b MESSAGE_ERROR
	RDOSyntaxMessage(CREF(tstring) message, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MESSAGE_ERROR)
		: m_message(message)
		, m_file   (file   )
		, m_line   (line   )
		, m_pos    (pos    )
		, m_type   (type   )
	{}
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

#endif // _SIMULATOR_SERVICE_ERROR_CODE_H_
