/*!
  \copyright (c) RDO-Team, 2012
  \file      rdo_build_edit_line_info.cpp
  \authors   ������� ��������� (lord.tiran@gmail.com)
  \date      12.08.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/report/comment.h"
#include "simulator/report/rdo_build_edit_line_info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

RDOBuildEditLineInfo::RDOBuildEditLineInfo( CREF(FileMessage) message ) :
	RDOLogEditLineInfo( message )
{}

RDOBuildEditLineInfo::RDOBuildEditLineInfo( CREF(tstring) message ) :
	RDOLogEditLineInfo( message )
{}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{}

tstring RDOBuildEditLineInfo::getMessage() const
{
	if ( isSimpleTextMessage() )
	{
		return getText();
	}
	else
	{
		tstring file = rdoModelObjects::getFileTypeString(getFileType());
		tstring error = (getMessageType() == FileMessage::MT_WARNING) ? tstring( WARNING_STRING ) : tstring( ERROR_STRING );
		tstring text = rdo::format( "%s (%d): %s: %s", file.c_str(), getLineNumber() + 1, error.c_str(), getText().c_str() );
		return text;
	}
}

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE