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

struct RDOSyntaxMessage
{
	enum ErrorCode
	{
		UNKNOWN = 1
	};

	enum Type
	{
		MESSAGE_ERROR = 0,
		MESSAGE_WARNING
	};

	tstring                      text;
	ErrorCode                    code;
	rdoModelObjects::RDOFileType file;
	ruint                        line;    //!< ����� ������ � �������
	ruint                        pos;     //!< ������� ������ � ������
	Type                         type; 

	RDOSyntaxMessage(CREF(tstring) text, ErrorCode code, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MESSAGE_ERROR) : 
		text(text),
		code(code),
		line(line),
		pos (pos ),
		file(file),
		type(type)
	{
	}
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

#endif // _SIMULATOR_SERVICE_ERROR_CODE_H_
