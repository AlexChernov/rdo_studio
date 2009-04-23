#include "pch.h"
#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>
#include "RDOCorba.hh"
#include "rdosmr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifdef CORBA_ENABLE

#define file1 "C:\\RTP.txt"
#define file2 "C:\\RSS.txt"

namespace rdoParse
{
void print_RTP (rdoParse::RDOCorba::GetRTP_var &my_rtpList)
{
		
	FILE *f1;
	f1=fopen(file1,"w");
	for (CORBA::Long i = 0; i < my_rtpList->length(); i++)
	{
		fprintf(f1,"\n���������� � ���� ������� �%d:\n", i+1);
		fprintf(f1,"   ��� ���� ������� �%d: %s \n", i+1, my_rtpList[i].m_name);
						
		if (my_rtpList[i].m_type==rdoParse::RDOCorba::TypeRTP::rt_permanent)
			fprintf(f1,"   ��� ���� �������: tr_permanent\n");
		else
			fprintf(f1,"   ��� ���� �������: tr_temporary\n");
			
		for (CORBA::Long j = 0; j < my_rtpList[i].m_param_count; j++)
		{
			fprintf(f1,"\n   ���������� � ��������� �%d:\n", j+1);
			fprintf(f1,"      ��� ���������:  %s \n", my_rtpList[i].m_param[j].m_name );
				
			switch (my_rtpList[i].m_param[j].m_type )
			{
			case rdoParse::RDOCorba::TypeParam::int_type:{
					fprintf(f1,"      ��� ���������: integer\n");
					if ( my_rtpList[i].m_param[j].m_diap_int == 1 )
					{
						fprintf(f1,"      ����������� ��������: %d \n", my_rtpList[i].m_param[j].m_min_int );
						fprintf(f1,"      ������������ ��������: %d \n", my_rtpList[i].m_param[j].m_max_int );
					}
					if ( my_rtpList[i].m_param[j].m_default_int_ch == 1 )
					{
						fprintf(f1,"      �������� �� ���������: %d \n", my_rtpList[i].m_param[j].m_default_int );
					}
					break;
			}
			case rdoParse::RDOCorba::TypeParam::double_type:{
					fprintf(f1,"      ��� ���������: real\n");

					if ( my_rtpList[i].m_param[j].m_diap_double == 1 )
					{
						fprintf(f1,"      ����������� ��������: %f \n", my_rtpList[i].m_param[j].m_min_double );
						fprintf(f1,"      ������������ ��������: %f \n", my_rtpList[i].m_param[j].m_max_double );
					}
					if ( my_rtpList[i].m_param[j].m_default_double_ch == 1 )
					{
						fprintf(f1,"      �������� �� ���������: %f \n", my_rtpList[i].m_param[j].m_default_double );
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::enum_type:{
					fprintf(f1,"      ��� ���������: enum\n");
					fprintf(f1,"      �������� ���������:");
										
					CORBA::Long k = 0;
			
					while ( k!= my_rtpList[i].m_param[j].m_var_enum_count )
					{
						fprintf(f1,"\t%s", my_rtpList[i].m_param[j].m_var_enum[k].pd_data );
						k++;
					}
					
					fprintf(f1,"\n");
					
					if ( my_rtpList[i].m_param[j].m_default_enum_ch == 1 )
					{
						fprintf(f1,"\n      �������� �� ���������: %s \n", my_rtpList[i].m_param[j].m_default_enum );
					}
					break;
				}
				default: break;
			}
		}
	}
				
	fclose(f1);
}

void print_RSS (rdoParse::RDOCorba::GetRSS_var &my_rssList)
{
	FILE *f2;
	f2=fopen(file2,"w");
	
	for (CORBA::Long i = 0; i < my_rssList->length(); i++)
	{
		
		fprintf(f2,"\n���������� � ������� �%d:\n\n", i+1);
		fprintf(f2,"��� �������: %s/ ��� �������: %s\n", my_rssList[i].m_name, my_rssList[i].m_type);
		
		for (CORBA::Long j = 0; j < my_rssList[i].m_param_count; j++)
		{
			switch (my_rssList[i].m_param[j].m_type)
			{
			case rdoParse::RDOCorba::TypeParam::int_type:{
			
				fprintf(f2,"  �������� ���������: %s = %d\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_int);
				break;
				}
			case rdoParse::RDOCorba::TypeParam::double_type:{
					fprintf(f2,"  �������� ���������: %s = %f\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_double);
					break;
				}
			case rdoParse::RDOCorba::TypeParam::enum_type:{

				fprintf(f2,"  �������� ���������: %s = %s\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_enum);
				break;
					}
			default: break;
			}
		}
	}
	
	fclose(f2);
}

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb, const char* ObjectName)
{
	CosNaming::NamingContext_var rootContext;

	try {
		
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;

		obj = orb->resolve_initial_references("NameService");
		
		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);


		if( CORBA::is_nil(rootContext) ) {
			cerr << "Failed to narrow the root naming context." << endl;
			return CORBA::Object::_nil();
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
		<< "with the location" << endl
		<< "of the naming service." << endl;
		return 0;
	}
	catch(CORBA::ORB::InvalidName& ex) {
		// This should not happen!
		cerr << "Service required is invalid [does not exist]." << endl;
		return CORBA::Object::_nil();
	}
	
	// Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);
	name[0].id = (const char*) "test"; // string copied
	name[0].kind = (const char*) "my_context"; // string copied
	name[1].id = (const char*) ObjectName;
	name[1].kind = (const char*) "Object";
	
	// Note on kind: The kind field is used to indicate the type
	// of the object. This is to avoid conventions such as that used
	// by files (name.type -- e.g. test.ps = postscript etc.)
	try {
		
		// Resolve the name to an object reference.
		return rootContext->resolve(name);
	}
	catch(CosNaming::NamingContext::NotFound& ex) {
		// This exception is thrown if any of the components of the
		// path [contexts or the object] aren�t found:
		cerr << "Context not found." << endl;
	}
	catch(CORBA::TRANSIENT& ex) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
		<< "naming service." << endl
		<< "Make sure the naming server is running and that omniORB is "
		<< "configured correctly." << endl;
	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name()
		<< " while using the naming service." << endl;
		return 0;
	}

	return CORBA::Object::_nil();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
void RDOParserCorbaRTP::parse()
{
	// ��� ���� ��������� ��� ���� �������� � ������� ���,
	// ������ � ������� ����� ����� �����, ������� ����� ���� ��������
	// � ��������� RTP � ����������� ���� �����


	
		rdoParse::RDOParserSMRInfo parser;
		parser.parse();

		
		rdoParse::RDOSMR::StringTable tmp = parser.getSMR()->getExternalModelList();

		rdoParse::RDOSMR::StringTable::const_iterator it = tmp.begin();
	
		const char* left;
		const char* right;

		while ( it != tmp.end() )
		{
			left = it->first.c_str();
			right = it->second.c_str();
		
			try {
		
				int argc = 0;

				CORBA::ORB_var orb = CORBA::ORB_init(argc, NULL);
				CORBA::Object_var obj = getObjectReference(orb, left); //����� ����� right

				rdoParse::RDOCorba_var rdocorbaref = rdoParse::RDOCorba::_narrow(obj);
				
				//*************************************************************
				CORBA::Long rtp_count = 0;
				
				rdoParse::RDOCorba::GetRTP_var tmp_rtp = rdocorbaref->getRDORTPlist( rtp_count );
				rdoParse::RDOCorba::GetRTP_var my_rtpList ( tmp_rtp );

				//�������� � ���� �� �
				print_RTP ( my_rtpList );

				//*************************************************************
				CORBA::Long rss_count = 0;
		
				rdoParse::RDOCorba::GetRSS_var tmp_rss = rdocorbaref->getRDORSSPlist( rss_count );
				rdoParse::RDOCorba::GetRSS_var my_rssList ( tmp_rss );
		
				//�������� � ���� �� �:
				print_RSS ( my_rssList );
				//*************************************************************
				orb->destroy();
		
				}	
			catch(CORBA::TRANSIENT&) {
				cerr << "Caught system exception TRANSIENT -- unable to contact the "
				<< "server." << endl;
			}
			catch(CORBA::SystemException& ex) {
				cerr << "Caught a CORBA::" << ex._name() << endl;
			}
			catch(CORBA::Exception& ex) {
				cerr << "Caught CORBA::Exception: " << ex._name() << endl;
			}
			catch(omniORB::fatalException& fe) {
				cerr << "Caught omniORB::fatalException:" << endl;
				cerr << " file: " << fe.file() << endl;
				cerr << " line: " << fe.line() << endl;
				cerr << " mesg: " << fe.errmsg() << endl;
			}
	
			//����� ����� ��������� � ����
			
			
			
			
			
			it++;
		}
			



	
/*

	//������� � ��������� ����� ��������� ����� ��������
	
	// �������� ������ ���� ��������� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( m_parser );
	
	for (CORBA::Long i = 0; i < rtp_count; i++)
	{

		// ������� ����� ��� �������
		rdoMBuilder::RDOResType rtp( my_rtpList[i].m_name.in() );
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
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int) );
							break;
						}
   						case 1:{//���� � ��������, � �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int , my_rtpList[i].m_param[j].m_default_int ) );
							break;
						}
						case 2:{//���� ������ ��������� ��������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int ) );
							break;
						}
   						case 3:{//���� ����� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int , my_rtpList[i].m_param[j].m_default_int ) );
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
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real) );
							break;
						}
   						case 1:{//���� � ��������, � �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						case 2:{//���� ������ ��������� ��������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double ) );
							break;
						}
   						case 3:{//���� ����� �������� �� ���������
							rtp.m_params.append( rdoMBuilder::RDOResType::Param( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real , my_rtpList[i].m_param[j].m_default_double ) );
							break;
						}
						default: break;
					}
					break;
				}
				case rdoParse::RDOCorba::TypeParam::enum_type:{
		

					//���� �� �������������, � ����





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

*/

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
			unsigned int fl = 0;

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

//#endif // CORBA_ENABLE