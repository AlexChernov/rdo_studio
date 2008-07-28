#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse()
{
	rdoRuntime::RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition1 = new rdoRuntime::RDOFuzzySetDefinitionRangeDiscret( m_parser->runtime() );
	fuzzy_setDefinition1->append(10, 50);
	rdoRuntime::RDOFuzzyType* fuzzy_type1  = new rdoRuntime::RDOFuzzyType( fuzzy_setDefinition1 );

	rdoRuntime::RDOFuzzyValue fuzzy_value1(*fuzzy_type1);
	fuzzy_value1.append(10, 0.7)(20, 1.0)(50, 0.6);

	rdoRuntime::RDOValue rdo_value1( fuzzy_value1 );
	rdoRuntime::RDOValue rdo_value2( fuzzy_value1.u_minus() );
	rdoRuntime::RDOValue rdo_value3( fuzzy_value1.u_obr() );

	std::string str1 = (rdo_value1 + rdo_value2).getAsString();
	std::string str2 = (rdo_value1 * rdo_value3).getAsString();

/*
	rdoRuntime::RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition1 = new rdoRuntime::RDOFuzzySetDefinitionRangeDiscret( m_parser->runtime() );
	fuzzy_setDefinition1->append(1, 7);
	rdoRuntime::RDOFuzzyType* fuzzy_type1  = new rdoRuntime::RDOFuzzyType( fuzzy_setDefinition1 );

	rdoRuntime::RDOFuzzyValue fuzzy_value1(*fuzzy_type1);
	fuzzy_value1.append(3, 0.9)(4, 1.0)(6, 0.6);

	rdoRuntime::RDOFuzzyValue fuzzy_value2(*fuzzy_type1);
	fuzzy_value2.append(3, 0.7)(5, 1.0)(6, 0.4);

	rdoRuntime::RDOValue rdo_value1( fuzzy_value1 );
	rdoRuntime::RDOValue rdo_value2( fuzzy_value2 );
	rdoRuntime::RDOValue rdo_value3 = rdo_value1 && rdo_value2;
*/
	int i = 1;
/*
	// ��� ���� ��������� ��� ���� �������� � ������� ���,
	// ������ � ������� ����� ����� �����, ������� ������ ���� ��������
	// � ��������� RTP � ����������� ���� �����


	// ���������� ���������� ����� ��������
	unsigned int rtp_count = 1;

	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( m_parser );

	for ( unsigned int i = 0; i < rtp_count; i++ )
	{
		// ������� ����� ��� �������
		rdoMBuilder::RDOResType rtp( "MyRTP" );
		// ��������� ��� �����������
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p1", rdoRuntime::g_int) );
		rtp.m_params.append( rdoMBuilder::RDOResType::Param("p2", rdoRuntime::g_int) );
		// �������� ��� � ������ ������������
		if ( rtpList.append( rtp ) )
		{
			// �������� �������
		}
		else
		{
			// �������, ��������, ��� � ����� ������ ��� ����
		}
	}
*/
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse()
{
	// ��� ���� ��������� ��� ������� � ������� ���

	// ���������� ���������� ��������
	unsigned int rss_count = 1;

	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList( m_parser );

	for ( unsigned int i = 0; i < rss_count; i++ )
	{
		// ������� ����� ������
		rdoMBuilder::RDOResource rss( rtpList["�����������"], "MyRSS1" );
		// ��������� ��� ���������
		rss["������������_max"] = 174;
		// ��������� ��� � ������ ������������
		if ( rssList.append<rdoParse::RDORSSResource>( rss ) )
		{
			// �������� �������
		}
		else
		{
			// �������
		}
		// ������ �������� ������� �� �������������
		if ( rssList.exist("qqq") )
		{
		}
	}
}

} // namespace rdoParse
