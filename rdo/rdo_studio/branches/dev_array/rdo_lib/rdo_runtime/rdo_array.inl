/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array.inl
 * author   : ������ ������
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
{}
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent,CREF(ArrayType) arraytype)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_arrayType     (arraytype       )
{}

inline RDOValue RDOArrayType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
		case RDOType::t_array: 
		{
			if (this == &from.type())
				return from;
			break;
	throw RDOTypeException();
}

inline tstring RDOArrayType::asString() const
{
	ASSERT(m_arrayType);
	return rdo::format(_T("array<%s>"), m_arrayType->asString().c_str());
}
CLOSE_RDO_RUNTIME_NAMESPACE
