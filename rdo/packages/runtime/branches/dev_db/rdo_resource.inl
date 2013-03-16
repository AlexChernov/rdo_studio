/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      15.06.2011
  \brief     RDOResource inline implementation
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
inline void RDOResource::setRuntime(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	/// @todo походу надо удалить метод
	NEVER_REACH_HERE;
}

inline tstring RDOResource::whoAreYou()
{
	return "rdoRes";
}

inline void RDOResource::makeTemporary(rbool value)
{
	m_temporary = value;
}

inline RDOResource::ConvertStatus RDOResource::getState() const
{
	return m_state;
}

inline void RDOResource::setState(RDOResource::ConvertStatus value)
{
	m_state = value;
}

inline rbool RDOResource::checkType(ruint type) const
{
	return m_type == type;
}

inline CREF(LPIResourceType) RDOResource::getResType() const
{
	return m_resType;
}

inline ruint RDOResource::getType() const
{
	return m_type;
}

inline CREF(RDOResource::ParamList) RDOResource::getParamList() const
{
	return m_paramList;
}

inline REF(RDOValue) RDOResource::getParamRaw(ruint index)
{
	ASSERT(index < m_paramList.size());
	setState(CS_Keep);
	return m_paramList[index];
}

inline ruint RDOResource::paramsCount() const
{
	return m_paramList.size();
}

inline void RDOResource::appendParams(CREF(ParamCIt) from_begin, CREF(ParamCIt) from_end)
{
	m_paramList.insert(m_paramList.end(), from_begin, from_end);
}

inline rbool RDOResource::canFree() const
{
	return m_referenceCount == 0;
}

inline void RDOResource::incRef()
{
	++m_referenceCount;
}

inline void RDOResource::decRef()
{
	--m_referenceCount;
}

inline tstring RDOResource::traceTypeId()
{
	return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
}

CLOSE_RDO_RUNTIME_NAMESPACE
