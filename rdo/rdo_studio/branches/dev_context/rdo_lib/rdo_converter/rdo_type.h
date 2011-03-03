/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_TYPE_H_
#define _CONVERTOR_RDOCONVERTER_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IType
// ----------------------------------------------------------------------------
class RDOValue;
PREDECLARE_POINTER(RDOType);

class IType
{
public:
	virtual tstring   name() const = 0;
	virtual LPRDOType type_cast(
		CREF(LPRDOType)        from,
		CREF(RDOParserSrcInfo) from_src_info,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	virtual RDOValue  value_cast(
		CREF(RDOValue)         from,
		CREF(RDOParserSrcInfo) to_src_info,
		CREF(RDOParserSrcInfo) src_info
	) const = 0;
	//! calc_cast ���������� ������ ����� type_cast, ������� ������� RDOParserSrcInfo �� ����������
	virtual rdoRuntime::LPRDOCalc calc_cast(
		CREF(rdoRuntime::LPRDOCalc) pCalc,
		CREF(LPRDOType)             pType
	) const = 0;
	virtual RDOValue get_default() const = 0;
};
#define DECLARE_IType                                                                                                                                                                  \
	virtual tstring                name       () const;                                                                                                                                \
	virtual LPRDOType              type_cast  (CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const; \
	virtual RDOValue               value_cast (CREF(RDOValue)  from, CREF(RDOParserSrcInfo) to_src_info,   CREF(RDOParserSrcInfo) src_info)                                     const; \
	virtual rdoRuntime::LPRDOCalc  calc_cast  (CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const;                                                                        \
	virtual RDOValue               get_default() const;

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
OBJECT(RDOType)
	IS  IMPLEMENTATION_OF(IType          )
	AND IMPLEMENTATION_OF(IModelStructure)
{
DECLARE_FACTORY(RDOType)
public:
	CREF(rdoRuntime::LPRDOType)        type() const { return m_pType;           }
	CREF(rdoRuntime::LPRDOType) operator-> () const { return m_pType;           }

	rdoRuntime::RDOType::TypeID      typeID() const { return m_pType->typeID(); }

	virtual rdoRuntime::LPRDOCalc calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
	{
		return pCalc;
	}

protected:
	RDOType(CREF(rdoRuntime::LPRDOType) pType)
		: m_pType(pType)
	{
		ASSERT(m_pType);
	}
	virtual ~RDOType()
	{}

	rdoRuntime::LPRDOType m_pType;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type)    \
class RDOType__##Type: public RDOType    \
{                                        \
private:                                 \
typedef RDOType parent_type;             \
public:                                  \
	RDOType__##Type():                   \
		RDOType(rdoRuntime::g_##Type)    \
	{}                                   \
	~RDOType__##Type()                   \
	{}                                   \
	DECLARE_IType;                       \
	DECLARE_IModelStructure;             \
};                                       \
DECLARE_POINTER(RDOType__##Type);

DEFINE_ATOM_TYPE_PARSER(unknow       );
DEFINE_ATOM_TYPE_PARSER(identificator);
DEFINE_ATOM_TYPE_PARSER(int          );
DEFINE_ATOM_TYPE_PARSER(real         );
DEFINE_ATOM_TYPE_PARSER(bool         );
DEFINE_ATOM_TYPE_PARSER(string       );

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_TYPE_H_
