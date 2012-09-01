/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_log_edit_line_info.h
  \authors   ������� ��������� (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_LOG_EDIT_LINE_INFO_H_
#define _RDO_LOG_EDIT_LINE_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "simulator/report/namespace.h"
#include "simulator/report/file_message.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

class RDOLogEditLineInfo
{
public:
	explicit RDOLogEditLineInfo( CREF(FileMessage) message );
	explicit RDOLogEditLineInfo( CREF(tstring) message );
	virtual ~RDOLogEditLineInfo();

	virtual tstring getMessage() const;
	rbool isSimpleTextMessage() const;

	FileMessage::Type getMessageType() const;
	rdoModelObjects::RDOFileType getFileType() const;
	ruint   getLineNumber() const;
	ruint   getPosInLine () const;
	ruint   getPosInLog  () const;
	tstring getText      () const;

	void setPosInLog(int posInLog);

private:
	FileMessage m_message;
	ruint m_posInLog;
	rbool m_simpleTextMessage;
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _RDO_LOG_EDIT_LINE_INFO_H_