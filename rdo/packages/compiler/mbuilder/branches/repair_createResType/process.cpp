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

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
void BlockForQueue::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(parser::RDOParser::s_parser());
	// �������� ������
	RDOResource rss(rtp, res_name);
	// ������� ��� � �������
	rssList.append<parser::RDORSSResource>(rss);
}

rbool BlockForQueue::checkType(RDOResType rtp, CREF(parser::RDOParserSrcInfo) info)
{
	UNUSED(info);

	// "�����_�������"
	tstring rtp_param_name = rdo::runtime::RDOPROCQueue::getQueueParamName();
	// ��� ������, �������� ��� �� ������� ��������� "�����_�������"
	if (!rtp.m_params[rtp_param_name].exist())
		parser::RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� integer '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	if (param.typeID() != rdo::runtime::RDOType::t_int)
		parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� int"), rtp.name().c_str(), rtp_param_name.c_str()));

	parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp.name());
	ASSERT(pResType);
	pResType->setType(parser::RDORTPResType::procRes);

	return true;
}

RDOResType BlockForQueue::createType(CREF(tstring) rtp_name, CREF(parser::RDOParserSrcInfo) info)
{
	// "�����_�������"
	tstring rtp_param_name = rdo::runtime::RDOPROCQueue::getQueueParamName();
	// �������� ����� ������� �� ���������
	parser::LPRDOValue pDefaultValue = rdo::Factory<parser::RDOValue>::create(
		rdo::explicit_value<ruint>(rdo::runtime::RDOPROCQueue::getDefaultValue()),
		info
	);
	ASSERT(pDefaultValue);
	// �������� ������ ���� ����� ��������
	RDOResTypeList rtpList(parser::RDOParser::s_parser());
	// �������� ��� �������
	RDOResType rtp(rtp_name);
	// �������� �������� ���� integer
	RDOResType::Param param(rtp_param_name, rdo::Factory<parser::RDOType__int>::create(), pDefaultValue);
	rtp.m_params.append(param);
	// ������� ��� �������
	if (!rtpList.append(rtp))
	{
		parser::RDOParser::s_parser()->error().error(info, rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
	}
	else
	{
		parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp_name);
		ASSERT(pResType);
		pResType->setType(parser::RDORTPResType::procRes);
	}
	return rtp;
}

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
rbool BlockForSeize::checkType(RDOResType rtp, CREF(parser::RDOParserSrcInfo) info)
{
	// "���������"
	tstring rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	tstring rtp_state_free = rdo::runtime::RDOPROCBlockForSeize::getStateEnumFree();
	// "�����"
	tstring rtp_state_buzy = rdo::runtime::RDOPROCBlockForSeize::getStateEnumBuzy();
	// ��� ������, �������� ��� �� ������� ������������� ���������
	if (!rtp.m_params[rtp_param_name].exist())
		parser::RDOParser::s_parser()->error().error(info, rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if (param.typeID() != rdo::runtime::RDOType::t_enum)
		parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����"), rtp.name().c_str(), rtp_param_name.c_str()));

	// ������ �������� ���� ��������
	if (!param.getEnum()->getEnums()->exist(rtp_state_free) || !param.getEnum()->getEnums()->exist(rtp_state_buzy))
		parser::RDOParser::s_parser()->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s"), rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

	parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp.name());
	ASSERT(pResType);
	pResType->setType(parser::RDORTPResType::procRes);

	return true;
}

void BlockForSeize::createRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(parser::RDOParser::s_parser());
	// �������� ������
	RDOResource rss(rtp, res_name);
	// ������� ��� � �������
	rssList.append<parser::RDORSSResource>(rss);
}

void BlockForSeize::reobjectRes(RDOResType rtp, CREF(tstring) res_name)
{
	// �������� ������ ���� ��������
	RDOResourceList rssList(parser::RDOParser::s_parser());
	// �������� ������
	RDOResource rssNew(rtp, res_name);
	// ������� ��� � �������
	rssList.replace<parser::RDORSSResource>(rssNew);
}

RDOResType BlockForSeize::createType(CREF(tstring) rtp_name, CREF(parser::RDOParserSrcInfo) info)
{
	// "���������"
	tstring rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	tstring rtp_state_free = rdo::runtime::RDOPROCBlockForSeize::getStateEnumFree();
	parser::LPRDOValue pDefaultValue = rdo::Factory<parser::RDOValue>::create(
		rdo::explicit_value<tstring>(rtp_state_free),
		info
	);
	ASSERT(pDefaultValue);
	pDefaultValue->setSrcText(rtp_state_free);
	// "�����"
	tstring rtp_state_buzy = rdo::runtime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// �������� ������ ���� ����� ��������
	RDOResTypeList rtpList(parser::RDOParser::s_parser());
	// �������� ��� �������
	RDOResType rtp(rtp_name);
	// �������� �������� ������������� ���� - "���������"
	RDOResType::Param param(
		rtp_param_name,
		rdo::runtime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
		pDefaultValue
	);
	rtp.m_params.append(param);
	// ������� ��� �������
	if (!rtpList.append(rtp))
	{
		parser::RDOParser::s_parser()->error().error(info, rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
	}
	else
	{
		parser::LPRDORTPResType pResType = parser::RDOParser::s_parser()->findRTPResType(rtp_name);
		ASSERT(pResType);
		pResType->setType(parser::RDORTPResType::procRes);
	}
	return rtp;
}

CLOSE_COMPILER_MBUILDER_NAMESPACE
