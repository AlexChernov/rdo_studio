/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     RDOResource implementation
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_array.h"
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
	, m_db               (pRuntime->getDB()                     )
#ifdef SERIALIZE_IN_DB_TRC
	, m_trcDB            (pRuntime->getTrcDB()                  )
#endif
{
	appendParams(paramList.begin(), paramList.end());
}

/// @todo копирующий конструктор не используется - нужен ли он?
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
/// @todo посмотреть history и принять решение о комментарии
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

tstring RDOResource::getTypeId() const
{
	rdo::ostringstream str;
	str << m_type;
	return str.str();
}

tstring RDOResource::traceParametersValue()
{
#ifdef SERIALIZE_IN_DB_TRC
	int r_id = m_trcDB->popContext<int>();
#endif

	rdo::ostringstream str;
	if(m_paramList.size() > 0)
	{
		ParamList::iterator end = m_paramList.end();
		int i = 1;
		for (ParamList::iterator it = m_paramList.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			rdo::ostringstream _str;
			_str << *it;
			tstring::size_type pos = _str.str().find("e");
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
#ifdef SERIALIZE_IN_DB_TRC
			it->serializeInDB(*m_trcDB);
			m_trcDB->queryListPushBack(QString("INSERT INTO trc_r_param_value VALUES(%1,%2,%3);")
				.arg(r_id)
				.arg(i)
				.arg(m_trcDB->popContext<int>()));
#endif
			str << *it;
#endif
			if (++it == end)
				break;
			str << " ";
			++i;
		}
	}
	return str.str();
}

tstring RDOResource::traceResourceState(char prefix, CREF(LPRDORuntime) pRuntime)
{
	double time = pRuntime->getCurrentTime();
	tstring rtpId = traceTypeId();
	tstring resId = traceId();

	rdo::ostringstream res;
	if (traceable() || (prefix != '\0'))
	{
		if (m_state == RDOResource::CS_NoChange || m_state == RDOResource::CS_NonExist)
			return "";

#ifdef SERIALIZE_IN_DB_TRC
		m_trcDB->queryListExec();
		m_trcDB->queryListPushBack(
			QString("INSERT INTO trc_r VALUES(DEFAULT,%1,'")
				.arg(time));
#endif

		if (prefix != '\0')
		{
			res << prefix;
#ifdef SERIALIZE_IN_DB_TRC
			m_trcDB->queryListPushBack(QString(prefix));
#endif
		}

		switch (m_state)
		{
		case RDOResource::CS_Create:
#ifdef SERIALIZE_IN_DB_TRC
			m_trcDB->queryListPushBack("RC',");
#endif
			res << "RC ";
			break;
		case RDOResource::CS_Erase:
#ifdef SERIALIZE_IN_DB_TRC
			m_trcDB->queryListPushBack("RE',");
			res << "RE "
#endif
#ifdef RDOSIM_COMPATIBLE
				<< pRuntime->getCurrentTime() << " "
				<< traceTypeId()              << " "
				<< traceId()                  << std::endl;
			return res.str();
#else
				;
#endif
			break;
		default:
#ifdef SERIALIZE_IN_DB_TRC
			m_trcDB->queryListPushBack("RK',");
#endif
			res << "RK ";
			break;
		}

#ifdef SERIALIZE_IN_DB_TRC
		m_trcDB->queryListPushBack(QString("%1,%2) RETURNING id;")
			.arg(QString::fromStdString(rtpId))
			.arg(QString::fromStdString(resId)));

		m_trcDB->pushContext<int>(m_trcDB->queryListExecInd());
#endif

		res << time                  << " "
			<< rtpId                 << " "
			<< resId                 << "  "
			<< traceParametersValue()<< std::endl;
	}
	return res.str();
}

void RDOResource::setParam(ruint index, CREF(RDOValue) value)
{
	ASSERT(index < m_paramList.size());

#ifdef SERIALIZE_IN_DB_MAINSTREAM
	#define DEFINE_RDO_VALUE(Table_name,Value) \
		m_db->queryExec(QString("update %1 set vv=%2 where id= (select value from rss_param where rss_id=%3 and id=%4)::integer;")\
			.arg(Table_name)                   \
			.arg(Value)                        \
			.arg(getTraceID())                 \
			.arg(index));

	LPRDOArrayType pThisArrayType;

	switch (value.typeID())
	{
	case RDOType::t_unknow        : break;
	case RDOType::t_int           : DEFINE_RDO_VALUE("int_rv"          ,                                           value.getInt     ()  ); break;
	case RDOType::t_real          : DEFINE_RDO_VALUE("real_rv"         ,                                           value.getDouble  ()  ); break;
	case RDOType::t_enum          : DEFINE_RDO_VALUE("enum_rv"         ,QString("'%1'").arg(QString::fromStdString(value.getAsString()))); break;
	case RDOType::t_bool          : DEFINE_RDO_VALUE("bool_rv"         ,QString("'%1'").arg(QString::fromStdString(value.getAsString()))); break;
	case RDOType::t_string        : DEFINE_RDO_VALUE("string_rv"       ,QString("'%1'").arg(QString::fromStdString(value.getString  ()))); break;
	case RDOType::t_identificator : DEFINE_RDO_VALUE("identificator_rv",QString("'%1'").arg(QString::fromStdString(value.getAsString()))); break;
	case RDOType::t_pointer       : pThisArrayType = value.type().object_dynamic_cast<RDOArrayType>(); if (pThisArrayType) m_paramList[index].updateArrayDB(index,getTraceID(),*m_db, value); break;
	default                       : throw RDOValueException("Данная величина не может быть записана в базу данных");
	}
#endif

	setState(CS_Keep);
	m_paramList[index] = value;
}

CREF(RDOValue) RDOResource::getParam(ruint index)
{
	ASSERT(index < m_paramList.size());

#ifndef DB_CACHE_ENABLE
	QSqlQuery* query = new QSqlQuery(*db);

#define GET_PARAM_DB(Table_name) \
	query->exec(QString("select vv from %1 where id= (select value from rss_param where rss_id=%2 and id=%3)::integer;") \
			.arg(Table_name)   \
			.arg(getTraceID()) \
			.arg(index));      \
	query->next();             \
	QVariant varValue = query->value(query->record().indexOf("vv"));

	if (value.typeID() == RDOType::t_int)
	{
		GET_PARAM_DB("int_rv");
		int varValueInt = varValue.toInt();
		if (varValueInt != m_paramList[index].getInt())
		{
			m_paramList[index] = RDOValue(varValueInt);
		}
	}
	else if (value.typeID() == RDOType::t_real)
	{
		GET_PARAM_DB("real_rv");
		double varValueDouble = varValue.toDouble();
		if (varValueDouble != m_paramList[index].getDouble())
		{
			m_paramList[index] = RDOValue(varValueDouble);
		}
	}
	else if (value.typeID() == RDOType::t_enum)
	{
		GET_PARAM_DB("enum_rv");
		tstring varValueEnum = varValue.toString().toStdString();
		if (varValueEnum != m_paramList[index].getAsString())
		{
			m_paramList[index] = RDOValue(m_paramList[index].type().object_static_cast<RDOEnumType>(),varValueEnum);
		}
	}
	else if (value.typeID() == RDOType::t_bool)
	{
		GET_PARAM_DB("bool_rv");
		bool varValueBool = varValue.toBool();
		if (varValueBool != m_paramList[index].getBool())
		{
			m_paramList[index] = RDOValue(varValueBool);
		}
	}
	else if (value.typeID() == RDOType::t_string)
	{
		GET_PARAM_DB("string_rv");
		tstring varValueString = varValue.toString().toStdString();
		if (varValueString != m_paramList[index].getString())
		{
			m_paramList[index] = RDOValue(varValueString);
		}
	}
	else if (value.typeID() == RDOType::t_identificator)
	{
		GET_PARAM_DB("identificator_rv");
		tstring varValueIdentificator = varValue.toString().toStdString();
		if (varValueIdentificator != m_paramList[index].getIdentificator())
		{
			m_paramList[index] = RDOValue(varValueIdentificator);
		}
	}
#endif

	return m_paramList[index];
}

#ifdef SERIALIZE_IN_DB_MAINSTREAM
void RDOResource::serializeInDB() const
{
	int rss_id = m_db->insertRowInd("rss",QString("%1,%2,%3")
		.arg(getTraceID())
		.arg(boost::lexical_cast<int>(getTypeId()))
		.arg(traceable() ? "true" : "false"));
	int param_id = -1;

	BOOST_FOREACH(const RDOValue& param, m_paramList)
	{
		param.serializeInDB(*m_db);
		m_db->insertRow("rss_param",QString("%1,%2,%3")
			.arg(rss_id)
			.arg(++param_id)
			.arg(m_db->popContext<int>()));
	}
}
#endif

CLOSE_RDO_RUNTIME_NAMESPACE
