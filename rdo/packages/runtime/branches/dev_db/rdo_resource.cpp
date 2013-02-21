/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     RDOResource implementation
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/src/db/general_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(ParamList) paramList, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary)
	: RDORuntimeObject   (                                      )
	, RDOTraceableObject (trace, resID, rdo::toString(resID + 1))
	, m_temporary        (temporary                             )
	, m_state            (RDOResource::CS_None                  )
	, m_type             (typeID                                )
	, m_referenceCount   (0                                     )
	, m_resType          (pResType                              )
{
	UNUSED(pRuntime);
	appendParams(paramList.begin(), paramList.end());
}

/// @todo ���������� ����������� �� ������������ - ����� �� ��?
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy)
	: RDORuntimeObject   (                 )
	, RDOTraceableObject (copy.traceable(), copy.getTraceID(), copy.traceId())
	, m_paramList        (copy.m_paramList )
	, m_temporary        (copy.m_temporary )
	, m_state            (copy.m_state     )
	, m_type             (copy.m_type      )
	, m_referenceCount   (0                )
	, m_resType          (copy.m_resType   )
	, m_typeId           (copy.m_typeId    )
{
	UNUSED(pRuntime);
	appendParams(copy.m_paramList.begin(), copy.m_paramList.end());
/// @todo ���������� history � ������� ������� � �����������
//	getRuntime()->incrementResourceIdReference( getTraceID() );
}

RDOResource::~RDOResource()
{}

rbool RDOResource::operator!= (RDOResource &other)
{
	if (m_type != other.m_type) return true;
	if (m_paramList.size() != other.m_paramList.size()) return true;

	int size = m_paramList.size();
	for (int i = 0; i < size; ++i)
	{
		if (m_paramList.at(i) != other.m_paramList.at(i)) return true;
	}
	return false;
}

LPRDOResource RDOResource::clone(CREF(LPRDORuntime) pRuntime) const
{
	LPRDOResource pResource = rdo::Factory<RDOResource>::create(pRuntime, m_paramList, m_resType, getTraceID(), m_type, traceable(), m_temporary);
	ASSERT(pResource);
	pRuntime->insertNewResource(pResource);
	return pResource;
}

tstring RDOResource::getTypeId()
{
	rdo::ostringstream str;
	str << m_type;
	return str.str();
}

tstring RDOResource::traceParametersValue()
{
	rdo::ostringstream str;
	if(m_paramList.size() > 0)
	{
		ParamList::iterator end = m_paramList.end();
		for (ParamList::iterator it = m_paramList.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			rdo::ostringstream _str;
			_str << *it;
			tstring::size_type pos = _str.str().find(_T("e"));
			if (pos != tstring::npos)
			{
				tstring __str = _str.str();
				__str.erase(pos + 2, 1);
				str << __str.c_str();
			}
			else
			{
				str << _str.str().c_str();
			}
#else
			str << *it;
#endif
			if (++it == end)
				break;
			str << _T(" ");
		}
	}
	return str.str();
}

tstring RDOResource::traceResourceState(char prefix, CREF(LPRDORuntime) pRuntime)
{
	rdo::ostringstream res;
	if (traceable() || (prefix != '\0'))
	{
		if (m_state == RDOResource::CS_NoChange || m_state == RDOResource::CS_NonExist)
			return _T("");

		if (prefix != '\0')
			res << prefix;

		switch (m_state)
		{
		case RDOResource::CS_Create:
			res << "RC ";
			break;
		case RDOResource::CS_Erase:
			res << "RE "
#ifdef RDOSIM_COMPATIBLE
				<< pRuntime->getCurrentTime() << _T(" ")
				<< traceTypeId()              << _T(" ")
				<< traceId()                  << std::endl;
			return res.str();
#else
				;
#endif
			break;
		default:
			res << "RK ";
			break;
		}

		res << pRuntime->getCurrentTime() << " "
			<< traceTypeId()              << " "
			<< traceId()                  << " "
			<< traceParametersValue()     << std::endl;
	}
	return res.str();
}

void RDOResource::setParam(ruint index, CREF(RDOValue) value)
{
	//QSqlQuery query;

	//query.exec(QString("select value from rss_param where rss_id=%1 and id=%2;")
	//	.arg(getTraceID())
	//	.arg(index));
	//query.next();
	//int value_id = query.value(query.record().indexOf("value")).toInt();

	//query.exec(QString("select relname from pg_class, rdo_value where pg_class.relfilenode=rdo_value.table_id and rdo_value.value_id=%1;")
	//	.arg(value_id));
	//query.next();
	//QString table_name = query.value(query.record().indexOf("value")).toString();

	//#define DEFINE_RDO_VALUE(Value)                            \
	//	db.queryExec(QString("update %1 set vv=%2 where id=%3")\
	//	.arg(table_name)                                       \
	//	.arg(Value)                                            \
	//	.arg(value_id));

	//switch (value.typeID())
	//{
	//case RDOType::t_unknow        : break;
	//case RDOType::t_int           : DEFINE_RDO_VALUE(                                           value.getInt     ()          ); break;
	//case RDOType::t_real          : DEFINE_RDO_VALUE(                                           value.getDouble  ()          ); break;
	//case RDOType::t_enum          : DEFINE_RDO_VALUE(QString("'%1'").arg(QString::fromLocal8Bit(value.getAsString().c_str()))); break;
	//case RDOType::t_bool          : DEFINE_RDO_VALUE(QString("'%1'").arg(QString::fromLocal8Bit(value.getAsString().c_str()))); break;
	//case RDOType::t_string        : DEFINE_RDO_VALUE(QString("'%1'").arg(QString::fromLocal8Bit(value.getString  ().c_str()))); break;
	//case RDOType::t_identificator : DEFINE_RDO_VALUE(QString("'%1'").arg(QString::fromLocal8Bit(value.getAsString().c_str()))); break;
	//default                       : throw RDOValueException("������ �������� �� ����� ���� �������� � ���� ������");
	//}

	ASSERT(index < m_paramList.size());
	setState(CS_Keep);
	m_paramList[index] = value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
