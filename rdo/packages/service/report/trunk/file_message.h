/*!
  \copyright (c) RDO-Team, 2012
  \file      file_message.h
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      1.10.2012
  \brief     �������� ���������
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_FILE_MESSAGE_H_
#define _SIMULATOR_REPORT_FILE_MESSAGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/model_objects.h"
#include "simulator/report/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

//! �������� ���������
struct FileMessage
{
	enum Type
	{
		MT_ERROR = 0,
		MT_WARNING
	};

	//! ����������� ���������
	//! \param message - ����� ���������
	//! \param file    - ����, � ������� ������� ������
	//! \param line    - ����� ������ � �������
	//! \param pos     - ������� ������ � ������
	//! \param type    - ��� ���������. �������� �� ��������� \b MT_ERROR
	FileMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MT_ERROR);

	tstring                      text; //!< ����� ���������
	rdoModelObjects::RDOFileType file; //!< ���� � �������
	ruint                        line; //!< ����� ������ � �������
	ruint                        pos;  //!< ������� ������ � ������
	Type                         type; //!<  ��� ���������
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_FILE_MESSAGE_H_
