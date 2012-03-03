#include "pch.h"

#ifdef CORBA_ENABLE

#include "rdoparser_corba.h"
#include "rdortp.h"
#include <rdo_resources.h>
#include <rdoruntime_object.h>
#include "RDOCorba.hh"
#include "rdosmr.h"

#define file1 "C:\\RTP.txt"
#define file2 "C:\\RSS.txt"

namespace rdoParse
{
void print_RTP (rdoParse::RDOCorba::GetRTP_var &my_rtpList)
{
		
	FILE *f1;
	f1=fopen(file1,"w");
	for (unsigned int i = 0; i < my_rtpList->length(); i++)
	{
		fprintf(f1,"\n���������� � ���� ������� �%d:\n", i+1);
		fprintf(f1,"   ��� ���� ������� �%d: %s \n", i+1, my_rtpList[i].m_name);
						
		if (my_rtpList[i].m_type==rdoParse::RDOCorba::rt_permanent)
			fprintf(f1,"   ��� ���� �������: tr_permanent\n");
		else
			fprintf(f1,"   ��� ���� �������: tr_temporary\n");
			
		for (unsigned int j = 0; j != my_rtpList[i].m_param_count; j++)
		{
			fprintf(f1,"\n   ���������� � ��������� �%d:\n", j+1);
			fprintf(f1,"      ��� ���������:  %s \n", my_rtpList[i].m_param[j].m_name );
				
			switch (my_rtpList[i].m_param[j].m_type )
			{
			case rdoParse::RDOCorba::int_type:{
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
			case rdoParse::RDOCorba::double_type:{
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
				case rdoParse::RDOCorba::enum_type:{
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
	
	for (unsigned int i = 0; i < my_rssList->length(); i++)
	{
		
		fprintf(f2,"\n���������� � ������� �%d:\n\n", i+1);
		fprintf(f2,"��� �������: %s/ ��� �������: %s\n", my_rssList[i].m_name, my_rssList[i].m_type);
		
		for (unsigned int j = 0; j != my_rssList[i].m_param_count; j++)
		{
			switch (my_rssList[i].m_param[j].m_type)
			{
			case rdoParse::RDOCorba::int_type:{
			
				fprintf(f2,"  �������� ���������: %s = %d\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_int);
				break;
				}
			case rdoParse::RDOCorba::double_type:{
					fprintf(f2,"  �������� ���������: %s = %f\n", my_rssList[i].m_param[j].m_name, my_rssList[i].m_param[j].m_double);
					break;
				}
			case rdoParse::RDOCorba::enum_type:{

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
			TRACE( "Failed to narrow the root naming context.\n" );
			return CORBA::Object::_nil();
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		TRACE( "Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service.\n" );
		return 0;
	}
	catch(CORBA::ORB::InvalidName& ex) {
		// This should not happen!
		TRACE( "Service required is invalid [does not exist].\n" );
		return CORBA::Object::_nil();
	}
	
	// Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);
	name[0].id = (const char*) "RDO"; // string copied
	name[0].kind = (const char*) "RDO_context"; // string copied
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
		TRACE( "Context not found." );
	}
	catch(CORBA::TRANSIENT& ex) {
		TRACE( "Caught system exception TRANSIENT -- unable to contact the naming service. Make sure the naming server is running and that omniORB is configured correctly. \n" );
	}
	catch(CORBA::SystemException& ex) {
		TRACE1( "Caught a CORBA:: %s while using the naming service.\n" , ex._name() );
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

			//�������� � ���� �� � ��� �����
			print_RTP ( my_rtpList );
			//*************************************************************
			//��������� � ������������ ����� �������� � ������� � �����������
	
			// �������� ������ ���� ��������� ����� ��������
			rdoMBuilder::RDOResTypeList rtpList( m_parser );
	
			for (unsigned int i = 0; i < my_rtpList->length(); i++)
			{
				rdoMBuilder::RDOResType rtp( my_rtpList[i].m_name.in() );
				// ��������� ��� �����������

				for (unsigned int j = 0; j != my_rtpList[i].m_param_count; j++)
				{
					switch ( my_rtpList[i].m_param[j].m_type ){
						case rdoParse::RDOCorba::int_type:{
			
							rdoMBuilder::RDOResType::Param par_int( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_int );
								
							if ( my_rtpList[i].m_param[j].m_diap_int == 1 )
								par_int.setDiap (my_rtpList[i].m_param[j].m_min_int , my_rtpList[i].m_param[j].m_max_int);
								
							if ( my_rtpList[i].m_param[j].m_default_int_ch == 1 )
								par_int.setDefault( my_rtpList[i].m_param[j].m_default_int );
								
							rtp.m_params.append( par_int );
							break;
						}
						case rdoParse::RDOCorba::double_type:{
			
							rdoMBuilder::RDOResType::Param par_double( my_rtpList[i].m_param[j].m_name.in() , rdoRuntime::g_real );
								
							if ( my_rtpList[i].m_param[j].m_diap_double == 1 )
								par_double.setDiap (my_rtpList[i].m_param[j].m_min_double , my_rtpList[i].m_param[j].m_max_double);
								
							if ( my_rtpList[i].m_param[j].m_default_double_ch == 1 )
								par_double.setDefault( my_rtpList[i].m_param[j].m_default_double );
							
							rtp.m_params.append( par_double );
							break;
						}
						case rdoParse::RDOCorba::enum_type:{
								
							//�������� ������ �������� ��������� ������������� ����
							rdoRuntime::RDOEnumType::Enums enumList;

							for (CORBA::Long k = 0; k < my_rtpList[i].m_param[j].m_var_enum_count ; k++)
							{
								enumList.push_back ( my_rtpList[i].m_param[j].m_var_enum[k].pd_data );
							}
							
							// �������� �������� ������������� ����
							rdoMBuilder::RDOResType::Param par_enum( my_rtpList[i].m_param[j].m_name.in() , new rdoRuntime::RDOEnumType( m_parser->runtime() , enumList) );

							//���������, ���� ���� �������� �� ���������
							if ( my_rtpList[i].m_param[j].m_default_enum_ch == 1)
							{
								std::string temp_string;
								temp_string = my_rtpList[i].m_param[j].m_default_enum.in();
								par_enum.setDefault( temp_string );
							}
								rtp.m_params.append( par_enum );
							break;
						}
						default: break;
					}
				}

				//��������� � ������������ ����� ��������
				if ( rtpList.append( rtp ) )
				{
					// �������� �������
					TRACE( "��� ���� ��� �������� ���������!!!\n" );
				}
				else
				{
					// �������, ��������, ��� � ����� ������ ��� ����
					TRACE( "���-�� ���� ������ ��� ��� ������� ��� ����������!!!\n" );
				}
			}

			// ������ ��� ���� �������� (������������� ��� �����)
			rdoMBuilder::RDOResTypeList::List::const_iterator rtp_it = rtpList.begin();
			while ( rtp_it != rtpList.end() )
			{
				TRACE1("rtp.name = %s\n", rtp_it->name().c_str());
				rdoMBuilder::RDOResType::ParamList::List::const_iterator param_it = rtp_it->m_params.begin();
							
				while ( param_it != rtp_it->m_params.end() )
				{
					std::string info = rdo::format("  param: %s: %s", param_it->name().c_str() , param_it->typeStr().c_str() );
					if ( param_it->hasDiap() )
					{
						info = rdo::format("%s [%s..%s]", info.c_str(), param_it->getMin().getAsString().c_str(), param_it->getMax().getAsString().c_str());
					}
					if ( param_it->hasDefault() )
					{
						info = rdo::format("%s = %s", info.c_str(), param_it->getDefault().getAsString().c_str());
					}
					TRACE1( "%s\n", info.c_str() );

					if ( param_it->typeID() ==  rdoRuntime::RDOType::t_enum )
					{
						rdoRuntime::RDOEnumType::CIterator enum_it = param_it->getEnum().begin();
						while ( enum_it != param_it->getEnum().end() )
						{
							TRACE1( "  - enum - %s\n", enum_it->c_str() );
							enum_it++;
						}
					}
					param_it++;
				}
				rtp_it++;
			}

			//*************************************************************
			CORBA::Long rss_count = 0;
		
			rdoParse::RDOCorba::GetRSS_var tmp_rss = rdocorbaref->getRDORSSPlist( rss_count );
			rdoParse::RDOCorba::GetRSS_var my_rssList ( tmp_rss );
		
			//�������� � ���� �� � ��� �����:
			print_RSS ( my_rssList );
				
			//*************************************************************
			//��������� � ������������ �������� � ������� � �����������

			// ���������� ��������� ������� � rssList
			rdoMBuilder::RDOResourceList rssList( m_parser );
			
			for (unsigned int i = 0; i < my_rssList->length(); i++)
			{
				// ����� ��� ������� �� ���������� ����� � ������� ������ ���������� ����
				rdoMBuilder::RDOResType _rtp = rtpList[my_rssList[i].m_type.in()];
				rdoMBuilder::RDOResource rss( _rtp , my_rssList[i].m_name.in() );
				
				//rdoMBuilder::RDOResource::Params::const_iterator it_param = rss.begin();
	
				for (unsigned int j = 0; j != my_rssList[i].m_param_count; j++)
				{
					//���������� ��������� �������� ���������� �������
					switch ( my_rssList[i].m_param[j].m_type ){
						case rdoParse::RDOCorba::int_type:{
							rss[ my_rssList[i].m_param[j].m_name.in() ] = my_rssList[i].m_param[j].m_int;
							break;
						}
						case rdoParse::RDOCorba::double_type:{
							rss[ my_rssList[i].m_param[j].m_name.in() ] = my_rssList[i].m_param[j].m_double;
							break;
						}
						case rdoParse::RDOCorba::enum_type:{
							std::string temp_string;
							temp_string = my_rssList[i].m_param[j].m_enum.in();
							rss[ my_rssList[i].m_param[j].m_name.in() ] = temp_string;
							break;
						}
						default: break;
					}												 

				 }
			
				//��������� � ������������ ��������
				if ( rssList.append<rdoParse::RDORSSResource>( rss ) )
				{
					// �������� �������
					TRACE( "��� ���� ������ ���������!!!\n" );
				}
				else
				{
					// �������
					TRACE( "���-�� ���� ������ ��� ������ ��� ����������!!!\n" );
				} 
		
			}
	
			// ������ ��� ������� ��� �����
			//rdoMBuilder::RDOResourceList rssList( &parser );
			rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();
			while ( rss_it != rssList.end() )
			{
				TRACE2("rss.name = %s: %s\n", rss_it->name().c_str(), rss_it->getType().name().c_str());
				rdoMBuilder::RDOResource::Params::const_iterator param_it = rss_it->begin();
				while ( param_it != rss_it->end() )
				{
					TRACE2("  %s = %s\n", param_it->first.c_str(), param_it->second.getAsString().c_str());
					param_it++;
				}
				rss_it++;
			}
	
			//*************************************************************
			orb->destroy();
	
		}	
		catch(CORBA::TRANSIENT&) {
			TRACE( "Caught system exception TRANSIENT -- unable to contact the server.\n" );
		}
		catch(CORBA::SystemException& ex) {
			TRACE1( "Caught a CORBA:: %s\n" , ex._name() );
		}
		catch(CORBA::Exception& ex) {
			TRACE1( "Caught CORBA::Exception: %s\n" , ex._name() );
		}
		catch(omniORB::fatalException& fe) {
			TRACE3( "Caught omniORB::fatalException: file: %s line: %d mesg: %s\n" , fe.file() , fe.line() , fe.errmsg() ); 
		}
	
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
void RDOParserCorbaRSS::parse()
{
/*
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
*/
}



} // namespace rdoParse

#endif // CORBA_ENABLE
