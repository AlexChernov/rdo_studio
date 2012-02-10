/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/process.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
void BlockForQueue::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// �������� ������
	RDOResource rss(rtp, res_name);
	// ������� ��� � �������
	rssList.append<rdoParse::RDORSSResource>(rss);
}

rbool BlockForQueue::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	UNUSED(info);

	// "�����_�������"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// ��� ������, �������� ��� �� ������� ��������� "�����_�������"
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� integer '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if (param.typeID() != rdoRuntime::RDOType::t_int)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� int"), rtp.name().c_str(), rtp_param_name.c_str()));

	return true;
}

RDOResType BlockForQueue::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "�����_�������"
	tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// �������� ����� ������� �� ���������
	rdoParse::LPRDOValue pDefaultValue = rdo::Factory<rdoParse::RDOValue>::create(
		rdo::explicit_value<ruint>(rdoRuntime::RDOPROCQueue::getDefaultValue()),
		info
	);
	ASSERT(pDefaultValue);
	// �������� ������ ���� ����� ��������
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// �������� ��� �������
	RDOResType rtp(rtp_name);
	// �������� �������� ���� integer
	RDOResType::Param param(rtp_param_name, rdo::Factory<rdoParse::RDOType__int>::create(), pDefaultValue);
	rtp.m_params.append(param);
	// ������� ��� �������
	if (!rtpList.append(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
	}
	else
	{
		rdoParse::LPRDORTPResType pResType = rdoParse::RDOParser::s_parser()->findRTPResType(rtp_name);
		ASSERT(pResType);
		pResType->setType(rdoParse::RDORTPResType::procRes);
	}
	return rtp;
}

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
rbool BlockForSeize::checkType(RDOResType rtp, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "���������"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "�����"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();
	// ��� ������, �������� ��� �� ������� ������������� ���������
	if (!rtp.m_params[rtp_param_name].exist())
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if (param.typeID() != rdoRuntime::RDOType::t_enum)
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����"), rtp.name().c_str(), rtp_param_name.c_str()));

	// ������ �������� ���� ��������
	if (!param.getEnum()->getEnums()->exist(rtp_state_free) || !param.getEnum()->getEnums()->exist(rtp_state_buzy))
		rdoParse::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s"), rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

	return true;
}

void BlockForSeize::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// �������� ������
	RDOResource rss(rtp, res_name);
	// ������� ��� � �������
	rssList.append<rdoParse::RDORSSResource>(rss);
	//! \todo ������� ������ ����������
}

void BlockForSeize::reobjectRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(rdoParse::RDOParser::s_parser());
	// �������� ������
	RDOResource rssNew(rtp, res_name);
	// ������� ��� � �������
	rssList.replace<rdoParse::RDORSSResource>(rssNew);
	//! \todo ������� ������ ����������
}

RDOResType BlockForSeize::createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info)
{
	// "���������"
	tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	tstring rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	rdoParse::LPRDOValue pDefaultValue = rdo::Factory<rdoParse::RDOValue>::create(
		rdo::explicit_value<tstring>(rtp_state_free),
		info
	);
	ASSERT(pDefaultValue);
	pDefaultValue->setSrcText(rtp_state_free);
	// "�����"
	tstring rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// �������� ������ ���� ����� ��������
	RDOResTypeList rtpList(rdoParse::RDOParser::s_parser());
	// �������� ��� �������
	RDOResType rtp(rtp_name);
	// �������� �������� ������������� ���� - "���������"
	RDOResType::Param param(
		rtp_param_name,
		rdoRuntime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
		pDefaultValue
	);
	rtp.m_params.append(param);
	// ������� ��� �������
	if (!rtpList.append(rtp))
	{
		rdoParse::RDOParser::s_parser()->error().error(info, rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
	}
	else
	{
		rdoParse::LPRDORTPResType pResType = rdoParse::RDOParser::s_parser()->findRTPResType(rtp_name);
		ASSERT(pResType);
		pResType->setType(rdoParse::RDORTPResType::procRes);
	}
	return rtp;
}

CLOSE_MBUILDER_NAMESPACE
