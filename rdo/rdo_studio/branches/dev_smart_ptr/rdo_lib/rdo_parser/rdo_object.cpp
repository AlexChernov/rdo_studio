/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_object.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
RDODeletable::RDODeletable(PTR(RDOParser) parser)
	: m_parser(parser)
{
	if (m_parser)
	{
		m_parser->insertDeletables(this);
		m_parser->runtime()->memory_insert(m_object_size);
	}
}

RDODeletable::~RDODeletable()
{
	noAutoDelete();
}

void RDODeletable::noAutoDelete()
{
	if (m_parser)
	{
		m_parser->removeDeletables(this);
	}
}

//#ifndef _DEBUG
//PTR(void) RDODeletable::operator new(size_t sz)
//{
//	PTR(RDODeletable) obj = static_cast<PTR(RDODeletable)>(::operator new(sz));
//	obj->m_object_size = sz;
//	return obj;
//}
//
//void RDODeletable::operator delete(PTR(void) v)
//{
//	if (static_cast<PTR(RDODeletable)>(v)->m_parser)
//	{
//		static_cast<PTR(RDODeletable)>(v)->m_parser->runtime()->memory_remove(static_cast<PTR(RDODeletable)>(v)->m_object_size);
//	}
//	::operator delete(v);
//}
//#endif

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
RDOParserSrcInfo::RDOParserSrcInfo()
	: RDOSrcInfo()
{
	init();
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(YYLTYPE) pos)
	: RDOSrcInfo()
{
	init();
	setSrcPos(pos);
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(rdoRuntime::RDOSrcInfo) info)
	: RDOSrcInfo()
{
	init();
	RDOSrcInfo::setSrcInfo(info);
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(rdoRuntime::RDOSrcInfo::Position) pos)
	: RDOSrcInfo()
{
	init();
	RDOSrcInfo::setSrcPos(pos);
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(tstring) text)
	: RDOSrcInfo()
{
	init();
	setSrcText(text);
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(YYLTYPE) pos, CREF(tstring) text)
	: RDOSrcInfo()
{
	init();
	setSrcPos (pos );
	setSrcText(text);
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, rbool first_align)
	: RDOSrcInfo()
{
	init();
	if (!first_align)
	{
		setSrcPos(pos_begin, pos_end);
	}
	else
	{
		if (pos_begin.first_line == pos_end.last_line)
		{
			setSrcPos(pos_begin, pos_end);
		}
		else
		{
			YYLTYPE pos(pos_begin);
			pos.first_line   = pos.last_line;
			pos.first_column = pos.last_column;
			setSrcPos(pos);
		}
	}
}

RDOParserSrcInfo::RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, CREF(tstring) text)
	: RDOSrcInfo()
{
	init();
	setSrcPos (pos_begin, pos_end);
	setSrcText(text);
}

void RDOParserSrcInfo::init()
{
	setSrcFileType(RDOParser::getFileToParse());
	rdoRuntime::RDOSrcInfo::Position pos;
	pos.m_first_pos  = RDOParser::lexer_loc_pos();
	pos.m_last_pos   = pos.m_first_pos;
	pos.m_first_line = RDOParser::lexer_loc_line();
	pos.m_last_line  = pos.m_first_line;
	RDOSrcInfo::setSrcPos(pos);
}

CLOSE_RDO_PARSER_NAMESPACE
