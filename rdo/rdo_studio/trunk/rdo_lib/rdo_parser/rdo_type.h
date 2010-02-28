/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_H_
#define _RDOPARSER_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType;
typedef rdo::smart_ptr<RDOType> LPRDOType;

class RDOType: public IModelStructure
{
public:
	CREF(rdoRuntime::RDOType)        type() const { return *m_type; }
	CPTR(rdoRuntime::RDOType) operator-> () const { return  m_type; }

	LPRDOType type_cast_throw(
		CREF(LPRDOType)        from,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const;

	virtual tstring              name      ()                                const = 0;
	virtual LPRDOType            type_cast (CREF(LPRDOType)            from) const = 0;
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const = 0;

	static LPRDOType getTypeByID(rdoRuntime::RDOType::TypeID typeID);

protected:
	RDOType(CPTR(rdoRuntime::RDOType) type)
		: m_type(type)
	{
		ASSERT(m_type);
	}

	CPTR(rdoRuntime::RDOType) m_type;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type, TypeName)                                                         \
class RDOType__##Type: public RDOType                                                                   \
{                                                                                                       \
public:                                                                                                 \
	RDOType__##Type():                                                                                  \
		RDOType(&rdoRuntime::g_##Type)                                                                  \
	{}                                                                                                  \
	virtual tstring              name      ()                                const { return TypeName; } \
	virtual LPRDOType            type_cast (CREF(LPRDOType)            from) const;                     \
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const;                     \
	DECLARE_IModelStructure;                                                                            \
};                                                                                                      \
extern rdo::smart_ptr<RDOType> g_##Type;

DEFINE_ATOM_TYPE_PARSER(unknow,        _T("unknow")       );
DEFINE_ATOM_TYPE_PARSER(identificator, _T("identificator"));
DEFINE_ATOM_TYPE_PARSER(int,           _T("integer")      );
DEFINE_ATOM_TYPE_PARSER(real,          _T("real")         );
DEFINE_ATOM_TYPE_PARSER(bool,          _T("bool")         );
DEFINE_ATOM_TYPE_PARSER(string,        _T("string")       );

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_H_
