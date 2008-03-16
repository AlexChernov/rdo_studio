#include "pch.h"
#include "rdo_resources.h"
#include <rdoparser.h>
#include <rdortp.h>

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
// ---- ������������� ���� ������� �� ������������� � ������
// ---- �������� ��� ��������� ������������� � ������ ���� �������
// --------------------------------------------------------------------
RDOResType::RDOResType( const rdoParse::RDORTPResType& rtp ):
	m_name( rtp.getName() ),
	m_permanent( rtp.isPermanent() )
{
	std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = rtp.getParams().begin();
	while ( param_it != rtp.getParams().end() )
	{
		Param param( **param_it );
		m_params.push_back( param );
		param_it++;
	}
}

// --------------------------------------------------------------------
// ---- ������������� *������* ���� �������
// --------------------------------------------------------------------
RDOResType::RDOResType( const std::string& name ):
	m_name( name ),
	m_permanent( true )
{
}

// --------------------------------------------------------------------
// ---------- RDOResType::Param
// --------------------------------------------------------------------
// ---- ������������� ��������� �� ������������� � ������ ��������
// --------------------------------------------------------------------
RDOResType::Param::Param( const rdoParse::RDORTPParam& param ):
	m_name( param.getName() )
{
	switch ( param.getType()->getType() ) {
		case rdoRuntime::RDOValue::Type::rvt_int: {
			m_type = rdoRuntime::RDOValue( rdoRuntime::RDOValue::Type::rvt_int );
			const rdoParse::RDORTPIntParamType* param_type = static_cast<const rdoParse::RDORTPIntParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_real: {
			m_type = rdoRuntime::RDOValue( rdoRuntime::RDOValue::Type::rvt_real );
			const rdoParse::RDORTPRealParamType* param_type = static_cast<const rdoParse::RDORTPRealParamType*>(param.getType());
			if ( param_type->getDiap().isExist() ) {
				m_min = param_type->getDiap().getMin();
				m_max = param_type->getDiap().getMax();
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_enum: {
			const rdoParse::RDORTPEnumParamType* enumParamType = static_cast<const rdoParse::RDORTPEnumParamType*>(param.getType());
			m_type = rdoRuntime::RDOValue( enumParamType->enu->getEnums() );
			break;
		}
	}
	if ( param.getType()->getDV().isExist() )
	{
		m_default = param.getType()->getDefaultValue( param.getType()->getDV().src_info() );
	}
}

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- �������� ��� �������, ������� ���� � �������
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList( rdoParse::RDOParser* parser ):
	m_parser( parser )
{
	std::vector< rdoParse::RDORTPResType* >::const_iterator rtp_it = m_parser->getRTPResType().begin();
	while ( rtp_it != m_parser->getRTPResType().end() ) {
		RDOResType rtp( **rtp_it );
		m_list.push_back( rtp );
		rtp_it++;
	}
}

} // rdoMBuilder
