/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.cpp
  \authors   ������� ��������� (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/report/rdo_log_edit_line_info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

RDOLogEditLineInfo::RDOLogEditLineInfo( CREF(FileMessage) message ):
	m_message( message ),
	m_posInLog(0),
	m_simpleTextMessage(false)
{}

RDOLogEditLineInfo::RDOLogEditLineInfo( CREF(tstring) message ) :
	m_message(FileMessage (
		message,
		rdoModelObjects::PAT,
		0, 
		0
		) ),
	m_posInLog(0),
	m_simpleTextMessage(true)
{}

RDOLogEditLineInfo::~RDOLogEditLineInfo()
{}

tstring RDOLogEditLineInfo::getMessage() const
{	
	if ( m_simpleTextMessage )
	{
		return m_message.text;
	}
	else
	{
		tstring file = rdoModelObjects::getFileTypeString(getFileType());
		tstring text = rdo::format( "%s (%d): %s", file.c_str(), m_message.line + 1, m_message.text.c_str() );
		return text;
	}
}

rbool RDOLogEditLineInfo::isSimpleTextMessage() const
{
	return m_simpleTextMessage;
}

rdoModelObjects::RDOFileType RDOLogEditLineInfo::getFileType() const
{
	return m_message.file;
}

ruint RDOLogEditLineInfo::getLineNumber() const
{
	return m_message.line;
}

ruint RDOLogEditLineInfo::getPosInLine() const
{
	return m_message.pos;
}

ruint RDOLogEditLineInfo::getPosInLog() const
{
	return m_posInLog;
}

tstring RDOLogEditLineInfo::getText() const
{
	return m_message.text;
}

FileMessage::Type RDOLogEditLineInfo::getMessageType() const
{
	return m_message.type;
}

void RDOLogEditLineInfo::setPosInLog(int posInLog)
{
	m_posInLog = posInLog;
}

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE