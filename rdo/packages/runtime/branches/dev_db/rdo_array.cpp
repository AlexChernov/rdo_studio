/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_array.cpp
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2009
  \brief     Массив
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOArrayValue
// --------------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pType)
	: m_pArrayType(pType)
{}

RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayValue) pValue)
	: m_container (pValue->m_container )
	, m_pArrayType(pValue->m_pArrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

CREF(LPRDOArrayType) RDOArrayValue::type() const
{
	ASSERT(m_pArrayType);
	return m_pArrayType;
}

void RDOArrayValue::push_back(CREF(RDOValue) item)
{
	m_container.push_back(item);
}

LPRDOArrayIterator RDOArrayValue::begin()
{
	return rdo::Factory<RDOArrayIterator>::create(m_container.begin());
}

LPRDOArrayIterator RDOArrayValue::end()
{
	return rdo::Factory<RDOArrayIterator>::create(m_container.end());
}

void RDOArrayValue::insert(CREF(LPRDOArrayIterator) pWhere, CREF(LPRDOArrayIterator) pFromFirst, CREF(LPRDOArrayIterator) pFromLast)
{
	ASSERT(pWhere    );
	ASSERT(pFromFirst);
	ASSERT(pFromLast );

	m_container.insert(pWhere->getIterator(), pFromFirst->getIterator(), pFromLast->getIterator());
}

void RDOArrayValue::erase(CREF(LPRDOArrayIterator) pFirst, CREF(LPRDOArrayIterator) pLast)
{
	ASSERT(pFirst)
	ASSERT(pLast );

	m_container.erase(pFirst->getIterator(), pLast->getIterator());
}

tstring RDOArrayValue::getAsString() const
{
	tstring result("[");
	STL_FOR_ALL_CONST(m_container, it)
	{
		if (it == m_container.begin())
		{
			result = rdo::format("%s%s", result.c_str(), it->getAsString().c_str());
		}
		else
		{
			result = rdo::format("%s, %s", result.c_str(), it->getAsString().c_str());
		}
	}
	return rdo::format("%s]", result.c_str());
}

ruint RDOArrayValue::size() const
{
	return m_container.size();
}

CREF(RDOValue) RDOArrayValue::getItem(CREF(RDOValue) index) const
{
	ruint ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException("Выход за пределы массива");
	}
	return m_container[ind];
}

void RDOArrayValue::setItem(CREF(RDOValue) index, CREF(RDOValue) item)
{
	ruint ind = index.getUInt();
	if (ind >= m_container.size())
	{
		throw RDORuntimeException("Выход за пределы массива");
	}
	m_container[ind] = item;
}

LPRDOArrayValue RDOArrayValue::clone() const
{
	LPRDOArrayValue pClone = rdo::Factory<RDOArrayValue>::create(type());
	ASSERT(pClone);

	BOOST_FOREACH(const RDOValue& value, m_container)
	{
		pClone->push_back(value.clone());
	}

	return pClone;
}

#ifdef INTERSECTION_DB
void RDOArrayValue::serializeInDB(REF(IDB) db) const
{
	QString tableName,tableNameVv;
	QString connectionName = db.getQtDB().connectionName();
	if (connectionName == "trc")
	{
		tableName = "trc_value_array";
		tableNameVv = "trc_value_array_vv";
	}
	if (connectionName == "rdo")
	{
		tableName = "array_rv";
		tableNameVv = "array_value";
	}

	int array_id = db.insertRowInd(tableName,"DEFAULT");

	BOOST_FOREACH(CREF(RDOValue) arrayItem, m_container)
	{
		arrayItem.serializeInDB(db);
		db.insertRow(tableNameVv,QString("%1,DEFAULT,%2")
			.arg(array_id)
			.arg(db.popContext<int>()));
	}

	db.pushContext<int>(array_id);
}
#endif

#ifdef SERIALIZE_IN_DB_MAINSTREAM
void RDOArrayValue::updateInDB(ruint index, ruint traceID, REF(IDB) db)
{
	QSqlQuery* query = new QSqlQuery(db.getQtDB());

	query->exec(QString("select value from rss_param where rss_id=%1 and id=%2;")
		.arg(traceID)
		.arg(index));
	query->next();
	int array_id = query->value(query->record().indexOf("value")).toInt();
	query->clear();

	eraseArrayValueDB(array_id,db);

	BOOST_FOREACH(CREF(RDOValue) arrayItem, m_container)
	{
		arrayItem.serializeInDB(db);
		db.insertRow("array_value",QString("%1,DEFAULT,%2")
			.arg(array_id)
			.arg(db.popContext<int>()));
	}
}

void RDOArrayValue::eraseArrayValueDB(int array_id, REF(IDB) db)
{
	QSqlQuery* queryOne = new QSqlQuery(db.getQtDB());
	QSqlQuery* queryTwo = new QSqlQuery(db.getQtDB());

	QString tableName;
	RDOType::TypeID type_id = m_pArrayType->typeID();
	switch (type_id)
	{
		case RDOType::t_unknow        : break;
		case RDOType::t_int           : tableName = "int_rv";           break;
		case RDOType::t_real          : tableName = "real_rv";          break;
		case RDOType::t_enum          : tableName = "enum_rv";          break;
		case RDOType::t_bool          : tableName = "bool_rv";          break;
		case RDOType::t_string        : tableName = "string_rv";        break;
		case RDOType::t_identificator : tableName = "identificator_rv"; break;
		case RDOType::t_pointer       : tableName = "array_rv";         break;
		default                       : throw RDOValueException("Тип массива, который РДО пытается обновить в БД, не определен");
	}

	queryOne->exec(QString("select vv_id from array_value where array_id=%1;")
		.arg(array_id));
	for (int i = 0; i < queryOne->size(); ++i)
	{
		queryOne->next();
		int array_value = queryOne->value(queryOne->record().indexOf("vv_id")).toInt();
		if (type_id == RDOType::t_pointer)
			eraseArrayValueDB(array_value,db);
		queryTwo->exec(QString("delete from %1 where id=%2;")
			.arg(tableName)
			.arg(array_value));
	}
	queryOne->clear();

	queryOne->exec(QString("delete from array_value where array_id=%1;")
		.arg(array_id));
}
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOArrayIterator
// --------------------------------------------------------------------------------
RDOArrayIterator::RDOArrayIterator(CREF(LPRDOArrayIterator) pIterator)
	: RDOType   (RDOType::t_pointer   )
	, m_iterator(pIterator->m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(RDOArrayIterator) iterator)
	: RDOType   (RDOType::t_pointer )
	, m_iterator(iterator.m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(Iterator) iterator)
	: RDOType   (RDOType::t_pointer)
	, m_iterator(iterator          )
{}

RDOArrayIterator::~RDOArrayIterator()
{}

RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

CREF(RDOValue) RDOArrayIterator::getValue() const
{
	return *m_iterator;
}

LPRDOArrayIterator RDOArrayIterator::preInc(rsint delta)
{
	m_iterator += delta;
	return LPRDOArrayIterator(this);
}

LPRDOArrayIterator RDOArrayIterator::postInc(rsint delta)
{
	LPRDOArrayIterator pPrev = rdo::Factory<RDOArrayIterator>::create(m_iterator);
	ASSERT(pPrev);
	m_iterator += delta;
	return pPrev;
}

LPRDOArrayIterator RDOArrayIterator::next()
{
	return preInc(1);
}

rbool RDOArrayIterator::equal(CREF(LPRDOArrayIterator) pIterator) const
{
	ASSERT(pIterator);
	return m_iterator == pIterator->m_iterator;
}

LPRDOArrayIterator RDOArrayIterator::clone() const
{
	return rdo::Factory<RDOArrayIterator>::create(*this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOArrayType
// --------------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPItemType) pItemType)
	: RDOType    (RDOType::t_pointer)
	, m_pItemType(pItemType         )
{}

RDOArrayType::~RDOArrayType()
{}

CREF(RDOArrayType::LPItemType) RDOArrayType::getItemType() const
{
	return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
