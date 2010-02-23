/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
//#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
CPTR(RDOType) RDOType::type_cast_throw(CREF(RDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	CPTR(RDOType) toType = type_cast(from);
	if (!toType)
	{
		rdoParse::g_error().push_only(src_info, rdo::format(_T("������������� ���� ������: %s � %s"), type().name().c_str(), from.type().name().c_str()));
		rdoParse::g_error().push_only(to_src_info,   _T("��. ������ ���"));
		rdoParse::g_error().push_only(from_src_info, _T("��. ������ ���"));
		rdoParse::g_error().push_done();
	}
	return toType;
}

CREF(RDOType) RDOType::getTypeByID(rdoRuntime::RDOType::TypeID typeID)
{
	switch (typeID)
	{
		case rdoRuntime::RDOType::t_unknow       : return g_unknow;
		case rdoRuntime::RDOType::t_int          : return g_int;
		case rdoRuntime::RDOType::t_real         : return g_real;
		case rdoRuntime::RDOType::t_bool         : return g_bool;
		case rdoRuntime::RDOType::t_string       : return g_string;
		case rdoRuntime::RDOType::t_identificator: return g_identificator;
	}
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__unknow
CPTR(RDOType) RDOType__unknow::type_cast(CREF(RDOType) from) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__unknow::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__int
CPTR(RDOType) RDOType__int::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_int : return &g_int;
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__int::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getInt();
}

//! RDOType__real
CPTR(RDOType) RDOType__real::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real: return &g_real;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__real::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getDouble();
}

//! RDOType__string
CPTR(RDOType) RDOType__string::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_string: return &g_string;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__string::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getString();
}

//! RDOType__identificator
CPTR(RDOType) RDOType__identificator::type_cast(CREF(RDOType) from) const
{
	return NULL;
}

rdoRuntime::RDOValue RDOType__identificator::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	throw rdoRuntime::RDOTypeException();
}

//! RDOType__bool
CPTR(RDOType) RDOType__bool::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType::t_bool: return &g_bool;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOType__bool::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return from.getBool();
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Class) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE_PARSER(unknow       );
DECLARE_ATOM_TYPE_PARSER(identificator);
DECLARE_ATOM_TYPE_PARSER(int          );
DECLARE_ATOM_TYPE_PARSER(real         );
DECLARE_ATOM_TYPE_PARSER(bool         );
DECLARE_ATOM_TYPE_PARSER(string       );

CLOSE_RDO_PARSER_NAMESPACE
