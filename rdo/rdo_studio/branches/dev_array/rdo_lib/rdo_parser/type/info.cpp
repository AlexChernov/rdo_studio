/*
 * copyright: (c) RDO-Team, 2011
 * filename : info.cpp
 * author   : ������ ������
 * date     : 09.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/info.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------

LPTypeInfo TypeInfo::type_cast(CREF(LPTypeInfo) pFrom, CREF(RDOParserSrcInfo) src_info) const
{
	//! TODO: TypeInfo ������ �������� �� src_info()
	LPRDOType pType = type()->type_cast(pFrom->type(), pFrom->src_info(src_info), this->src_info(src_info), src_info);
	ASSERT(pType);
	LPTypeInfo pTypeInfo = rdo::Factory<TypeInfo>::create(pType, this->src_info(src_info));
	ASSERT(pTypeInfo);
	return pTypeInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
