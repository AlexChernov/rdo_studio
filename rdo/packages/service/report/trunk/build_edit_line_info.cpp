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
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

BuildEditLineInfo::BuildEditLineInfo( CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type ) :
	LogEditLineInfo( text, file, line, pos, type )
{}

BuildEditLineInfo::BuildEditLineInfo( CREF(FileMessage) message )
	: LogEditLineInfo(message)
{}

BuildEditLineInfo::BuildEditLineInfo( CREF(tstring) message ) :
	LogEditLineInfo( message )
{}

BuildEditLineInfo::~BuildEditLineInfo()
{}

tstring BuildEditLineInfo::getMessage() const
{
	if ( isSimpleTextMessage() )
	{
		return getText();
	}
	else
	{
		tstring file = rdoModelObjects::getFileTypeString(getFileType());
		tstring error = ( getType() == FileMessage::MT_WARNING ) ? tstring( WARNING_STRING ) : tstring( ERROR_STRING );
		tstring text = rdo::format(_T("%s (%d): %s: %s"), file.c_str(), getLineNumber() + 1, error.c_str(), getText().c_str() );
		return text;
	}
}

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE