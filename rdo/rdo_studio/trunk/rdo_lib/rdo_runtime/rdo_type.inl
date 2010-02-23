/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.inl
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
inline RDOType::RDOType(TypeID typeID)
	: m_typeID(typeID)
{}

inline RDOType::TypeID RDOType::typeID() const
{
	return m_typeID;
}

CLOSE_RDO_RUNTIME_NAMESPACE
