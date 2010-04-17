#include "rdo_lib/rdo_mbuilder/pch.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ---------- ������������ ����� � ��������� ������������
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(RDOResType) obj)
{
	operator=( obj );
}

RDOResType::Param::Param(CREF(RDOResType::Param) obj)
{
	operator=( obj );
}

RDOResource::RDOResource(CREF(RDOResource) obj)
{
	operator=( obj );
}

void RDOResType::operator= (CREF(RDOResType) obj)
{
	m_name   = obj.m_name;
	m_exist  = obj.m_exist;
	m_type   = obj.m_type;
	m_id     = obj.m_id;
	m_params = obj.m_params;
}

void RDOResType::Param::operator= (CREF(RDOResType::Param) obj)
{
	m_name    = obj.m_name;
	m_exist   = obj.m_exist;
	m_type    = obj.m_type;
	m_id      = obj.m_id;
	m_min     = obj.m_min;
	m_max     = obj.m_max;
	m_default = obj.m_default;
}

void RDOResource::operator= (CREF(RDOResource) obj)
{
	m_name  = obj.m_name;
	m_exist = obj.m_exist;
	m_rtp   = obj.m_rtp;
	m_id    = obj.m_id;
	m_params.clear();
	for (Params::const_iterator it = obj.m_params.begin(); it != obj.m_params.end(); ++it)
		m_params[it->first] = it->second;
}

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
// ---- ������������� ���� ������� �� ������������� � ������
// ---- �������� ��� ��������� ������������� � ������ ���� �������
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(rdoParse::RDORTPResType) rtp)
	: rdoParse::RDOParserSrcInfo(rtp.src_info()              )
	, m_name (rtp.name()                                     )
	, m_type (rtp.isPermanent() ? rt_permanent : rt_temporary)
	, m_exist(true                                           )
	, m_id   (rtp.getNumber()                                )
{
	STL_FOR_ALL_CONST(rdoParse::RDORTPResType::ParamList, rtp.getParams(), param_it)
	{
		Param param(*param_it);
		param.m_id = m_params.size();
		m_params.append(param);
	}
}

RDOResType::Param::Param(CREF(rdoParse::LPRDORTPParam) param)
	: rdoParse::RDOParserSrcInfo(param->src_info()  )
	, m_name (param->name()                         )
	, m_type (&param->getParamType()->type()->type())
	, m_exist(true                                  )
	, m_id   (-1                                    )
{
	switch ( typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if (dynamic_cast<PTR(rdoParse::RDOTypeIntRange)>(param->getParamType()->type().get()))
			{
				rdoParse::LPRDOTypeIntRange pRange = param->getParamType()->type().lp_cast<rdoParse::LPRDOTypeIntRange>();
				m_min = pRange->range()->getMin().value().getInt();
				m_max = pRange->range()->getMax().value().getInt();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			if (dynamic_cast<PTR(rdoParse::RDOTypeRealRange)>(param->getParamType()->type().get()))
			{
				rdoParse::LPRDOTypeRealRange pRange = param->getParamType()->type().lp_cast<rdoParse::LPRDOTypeRealRange>();
				m_min = pRange->range()->getMin().value().getDouble();
				m_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
	}
	if (param->getParamType()->default().defined())
	{
		m_default = param->getParamType()->default().value();
	}
}

// --------------------------------------------------------------------
// ---- ������������� *������* ���� �������
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(tstring) name, Type type)
	: m_name (name )
	, m_type (type )
	, m_exist(false)
	, m_id   (-1   )
{}

rbool RDOResType::ParamList::append(REF(Param) param)
{
	if (std::find_if(begin(), end(), rdoParse::compareNameRef<Param>(param.name())) != end())
		return false;

	m_list.push_back( param );
	return true;
}

RDOResType::Param::Param(CREF(tstring) name, CPTR(rdoRuntime::RDOType) type, CREF(rdoRuntime::RDOValue) def)
	: m_name   (name)
	, m_type   (type)
	, m_default(def )
	, m_exist  (true)
	, m_id     (-1  )
{
	if (type->typeID() == rdoRuntime::RDOType::t_enum && def.typeID() == rdoRuntime::RDOType::t_string)
	{
		m_default = rdoRuntime::RDOValue(getEnum(), def.getAsString());
	}
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdoRuntime::RDOValue) def)
	: m_name   (name       )
	, m_type   (&def.type())
	, m_default(def        )
	, m_exist  (true       )
	, m_id     (-1         )
{}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdoRuntime::RDOValue) min, CREF(rdoRuntime::RDOValue) max, CREF(rdoRuntime::RDOValue) def)
	: m_name   (name                                                 )
	, m_type   (&rdoParse::RDOType::getTypeByID(min.typeID())->type())
	, m_min    (min                                                  )
	, m_max    (max                                                  )
	, m_default(def                                                  )
	, m_exist  (true                                                 )
	, m_id     (-1                                                   )
{}

void RDOResType::Param::setDiap(CREF(rdoRuntime::RDOValue) min, CREF(rdoRuntime::RDOValue) max)
{
	m_min = min;
	m_max = max;
}

void RDOResType::Param::setDefault(CREF(rdoRuntime::RDOValue) def)
{
	m_default = def;
}

rbool RDOResType::Param::operator== (CREF(Param) param) const
{
	return true;
}

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- �������� ��� ���� ��������, ������� ���� � �������
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList(PTR(rdoParse::RDOParser) parser)
	: RDOList<RDOResType>(parser)
{
	STL_FOR_ALL_CONST(rdoParse::RDOParser::RTPResTypeList, m_parser->getRTPResTypes(), rtp_it)
	{
		RDOResType rtp(**rtp_it);
		m_list.push_back(rtp);
	}
}

// --------------------------------------------------------------------
// ---- ���������� *������* ���� �������
// --------------------------------------------------------------------
rbool RDOResTypeList::append(REF(RDOResType) rtp)
{
	if (std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResType>(rtp.name())) != end())
		return false;

	PTR(rdoParse::RDORTPResType) pRTP = new rdoParse::RDORTPResType(m_parser, rtp.name(), rtp.isPermanent());
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, rtp.m_params, param)
	{
		rdoParse::LPRDOTypeParam pParamType;
		switch (param->typeID())
		{
			case rdoRuntime::RDOType::t_int:
			{
				rdoRuntime::RDOValue default = param->hasDefault() ? param->getDefault() : rdoRuntime::RDOValue();
				if (param->hasDiap())
				{
					rdoParse::LPRDOTypeRangeRange pRange    = rdo::Factory<rdoParse::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParse::RDOParserSrcInfo());
					rdoParse::LPRDOTypeIntRange   pIntRange = rdo::Factory<rdoParse::RDOTypeIntRange>::create(pRange);
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(pIntRange, default, rdoParse::RDOParserSrcInfo());
				}
				else
				{
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(rdoParse::g_int, default, rdoParse::RDOParserSrcInfo());
				}
				break;
			}
			case rdoRuntime::RDOType::t_real:
			{
				rdoRuntime::RDOValue default = param->hasDefault() ? param->getDefault() : rdoRuntime::RDOValue();
				if (param->hasDiap())
				{
					rdoParse::LPRDOTypeRangeRange pRange     = rdo::Factory<rdoParse::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParse::RDOParserSrcInfo());
					rdoParse::LPRDOTypeRealRange  pRealRange = rdo::Factory<rdoParse::RDOTypeRealRange>::create(pRange);
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(pRealRange, default, rdoParse::RDOParserSrcInfo());
				}
				else
				{
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(rdoParse::g_real, default, rdoParse::RDOParserSrcInfo());
				}
				break;
			}
			case rdoRuntime::RDOType::t_enum:
			{
				rdoParse::LPRDOEnumType pEnum = rdo::Factory<rdoParse::RDOEnumType>::create();
				STL_FOR_ALL_CONST(rdoRuntime::RDOEnumType, param->getEnum(), enum_it)
				{
					pEnum->add(rdoParse::RDOValue::getIdentificator(*enum_it));
				}
				rdoRuntime::RDOValue default = param->hasDefault() ? param->getDefault() : rdoRuntime::RDOValue();
				pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(pEnum, default, rdoParse::RDOParserSrcInfo());
				break;
			}
			default:
			{
				delete pRTP;
				return false;
			}
		}
		rdoParse::LPRDORTPParam pParam = rdo::Factory<rdoParse::RDORTPParam>::create(pRTP, pParamType, rdoParse::RDOParserSrcInfo());
		ASSERT(pParam);
		pRTP->addParam(pParam);
	}
	rtp.m_exist = true;
	rtp.m_id    = pRTP->getNumber();
	m_list.push_back(rtp);
	return true;
}

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
// ---- ������������� ������� �� ������������� � ������
// ---- �������� ��� ��������� ������������� � ������ �������
// --------------------------------------------------------------------
RDOResource::RDOResource(CREF(rdoParse::RDORSSResource) rss)
	: rdoParse::RDOParserSrcInfo(rss.src_info())
	, m_name (rss.name()                       )
	, m_rtp  (*rss.getType()                   )
	, m_exist(true                             )
	, m_id   (rss.getID()                      )
{
	if (m_rtp.m_params.size() == rss.params().size())
	{
		ruint index = 0;
		STL_FOR_ALL_CONST(RDOResType::ParamList::List, m_rtp.m_params, param_it)
		{
			m_params[param_it->name()] = rss.params()[index];
			index++;
		}
	}
}

RDOResource::Params::const_iterator RDOResource::operator[] (CREF(tstring) param) const
{
	return m_params.find(param);
}

REF(RDOResource::Params::mapped_type) RDOResource::operator[] (CREF(tstring) param)
{
	RDOResource::Params::iterator param_it = m_params.find(param);
	if (param_it != m_params.end())
	{
		return param_it->second;
	}
	else
	{
		static Params::mapped_type tmpValue;
		return tmpValue;
	}
}

CPTR(rdoParse::RDORSSResource) RDOResource::getParserResource(CREF(rdoParse::RDOParser) parser) const
{
	if (!exist())
		return NULL;

	return parser.findRSSResource(name());
}

rbool RDOResource::fillParserResourceParams(PTR(rdoParse::RDORSSResource) toParserRSS) const
{
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, getType().m_params, param_it)
	{
		RDOResource::Params::const_iterator value_it = operator[](param_it->name());
		if (value_it == end())
			return false;
//! TODO: �� ���� ��� ����������
		//rdoRuntime::RDOValue value = param_it->type()->value_cast(value_it->second);
		////! TODO: � ������ ��� toParserRSS->src_info(), � �� value_it->src_info() ?
		//toParserRSS->addParam(rdoParse::RDOValue(value, value.type(), toParserRSS->src_info()));
	}
	return true;
}

// --------------------------------------------------------------------
// ---- ������������� *������* �������
// --------------------------------------------------------------------
RDOResource::RDOResource(CREF( RDOResType) rtp, CREF(tstring) name)
	: m_name (name                                  )
	, m_rtp  (rtp                                   )
	, m_exist(false                                 )
	, m_id   (rdoParse::RDORSSResource::UNDEFINED_ID)
{
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, m_rtp.m_params, param_it)
	{
		rdoRuntime::RDOValue value(*param_it->type());
		if (param_it->hasDefault())
		{
			value = param_it->getDefault();
		}
		else if (param_it->hasDiap())
		{
			value = param_it->getMin();
		}
		m_params[param_it->name()] = value;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
// ---- �������� ��� �������, ������� ���� � �������
// --------------------------------------------------------------------
RDOResourceList::RDOResourceList(PTR(rdoParse::RDOParser) parser)
	: RDOList<RDOResource>(parser)
{
	STL_FOR_ALL_CONST(rdoParse::RDOParser::RSSResourceList, m_parser->getRSSResources(), rss_it)
	{
		RDOResource rss(**rss_it);
		m_list.push_back(rss);
	}
}

} // rdoMBuilder
