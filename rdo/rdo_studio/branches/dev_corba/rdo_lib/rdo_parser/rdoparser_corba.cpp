#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>
#include "RDOCorba.hh"

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
	// ��� ���� ��������� ��� ���� �������� � ������� ���,
	// ������ � ������� ����� ����� �����, ������� ����� ���� ��������
	// � ��������� RTP � ����������� ���� �����


	//-------------------------���� ������ ������� ��� ��������
	//���������� ����� ����� �������� (������ ���� ���� ��� ������)
	CORBA::Long rtp_count = 1;

	//������ �� ���������� ���������� �������� ������� ���� �������� (2 ������ ���� ��� ������)
	rdoParse::RDOCorba::PARAM_count params_count;
	params_count.length( rtp_count );

	rdoParse::RDOCorba::GetRTP my_rtpList;
	my_rtpList.length( rtp_count);

	//������������� �������� ��������, ����� �� ��� ������� ����� ���
	//�������, ��� ����� ��� ���������
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{
		my_rtpList[i].m_name = "��� ������� �1";
		my_rtpList[i].m_type = rdoParse::RDOCorba::TypeRTP::rt_permanent;
		my_rtpList[i].m_param.length(params_count[i]);

		my_rtpList[i].m_param[0].m_name = "�����";
		my_rtpList[i].m_param[0].m_type = rdoParse::RDOCorba::TypeParam::int_type;
		my_rtpList[i].m_param[0].m_diap_int = 1;
		my_rtpList[i].m_param[0].m_default_int_ch = 1;
		my_rtpList[i].m_param[0].m_min_int = 10;
		my_rtpList[i].m_param[0].m_max_int = 1;
		my_rtpList[i].m_param[0].m_default_int = 5;

		my_rtpList[i].m_param[1].m_name = "������������";
		my_rtpList[i].m_param[1].m_type = rdoParse::RDOCorba::TypeParam::double_type;
		my_rtpList[i].m_param[1].m_diap_double = 1;
		my_rtpList[i].m_param[1].m_default_double_ch = 1;
		my_rtpList[i].m_param[1].m_min_double = 10;
		my_rtpList[i].m_param[1].m_max_double = 1;
		my_rtpList[i].m_param[1].m_default_double = 5;
	}

	//������� � ��������� ����� ��������� ����� ��������
	
	// �������� ������ ���� ��������� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{

		// ������� ����� ��� �������
		rdoMBuilder::RDOResType rtp( my_rtpList[i].m_name );
		// ��������� ��� �����������

		for (CORBA::Long j = 0; j < params_count[i]; j++)
		{
			switch (my_rtpList[i].m_param[j].m_type ){
				case rdoParse::RDOCorba::TypeParam::int_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_int == 1 &&  my_rtpList[i].m_param[j].m_default_int_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_int == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_int_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//��� �� ���������, �� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , rdoRuntime::g_int) );
							break;
						}
   						case 1:{//���� � ��������, � �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						case 2:{//���� ������ ��������� ��������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int ) );
							break;
						}
   						case 3:{//���� ����� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , rdoRuntime::g_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::double_type:{
			
					unsigned int fl = 0;

					if ( my_rtpList[i].m_param[j].m_diap_double == 1 &&  my_rtpList[i].m_param[j].m_default_double_ch == 1) 
						fl = 1;
					else
					{
						if ( my_rtpList[i].m_param[j].m_diap_double == 1 )		
							fl = 2;
						else	
						{
							if ( my_rtpList[i].m_param[j].m_default_double_ch = 1 )
								fl = 3;
						}
					}

					switch (fl){
						case 0:{//��� �� ���������, �� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , rdoRuntime::g_real) );
							break;
						}
   						case 1:{//���� � ��������, � �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						case 2:{//���� ������ ��������� ��������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double ) );
							break;
						}
   						case 3:{//���� ����� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name , rdoRuntime::g_real , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::enum_type:{
		

					//���� �� �������������

					break;
				}
				default: break;
			}
		
		}

		//��������� � ������������
		if ( rtpList.append( rtp ) )
		{
			// �������� �������
		}
		else
		{
			// �������, ��������, ��� � ����� ������ ��� ����
			//parser->error( info, rdo::format("������ �������� ���� �������: %s", rtp_name.c_str()) );
	
		}
	}





/*

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
