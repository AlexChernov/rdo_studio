/*
 * copyright: (c) RDO-Team, 2010
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
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
inline RDOArrayValue::RDOArrayValue()
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayType) type)
	: m_arrayType(&type)
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_arrayType(value.m_arrayType)
{
	m_arrayValue = value.m_arrayValue;
}

inline RDOArrayValue::~RDOArrayValue()
{}

inline CREF(RDOArrayType) RDOArrayValue::type() const
{
	return *m_arrayType;
}

inline void RDOArrayValue::insert_array(CREF(RDOValue) pArray)
{
	m_arrayValue.push_back(&pArray);
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent, ArrayType pArrayType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_pArrayType    (pArrayType      )
{}

inline RDOArrayType::ArrayType RDOArrayType::getArrayType() const
{
	return m_pArrayType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
