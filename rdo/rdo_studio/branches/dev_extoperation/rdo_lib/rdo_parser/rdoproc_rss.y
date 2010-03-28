%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type				257
%token RDO_permanent					258
%token RDO_Parameters					259
%token RDO_integer						260
%token RDO_real							261
%token RDO_End							262
%token RDO_temporary					263
%token RDO_IDENTIF						264
%token RDO_INT_CONST					265
%token RDO_REAL_CONST					266
%token RDO_such_as						267
%token RDO_dblpoint						268
%token RDO_Resources					269
%token RDO_trace						270
%token RDO_no_trace						271
%token RDO_IDENTIF_COLON				272
%token RDO_Constant						273
%token RDO_Body							274
%token RDO_Function						275
%token RDO_Type							276
%token RDO_algorithmic					277
%token RDO_table						278
%token RDO_list							279
%token RDO_Exist						281
%token RDO_Not_Exist					282
%token RDO_For_All						283
%token RDO_Not_For_All					284
%token RDO_neq							285
%token RDO_leq							286
%token RDO_geq							287
%token RDO_NoCheck						288
%token RDO_Calculate_if					289
%token RDO_or							290
%token RDO_and							291
%token RDO_Sequence						292
%token RDO_uniform						293
%token RDO_exponential					294
%token RDO_normal						295
%token RDO_by_hist						296
%token RDO_enumerative					297

%token RDO_Pattern						298
%token RDO_operation					299
%token RDO_action						300
%token RDO_irregular_event				301
%token RDO_rule							302
%token RDO_keyboard						303
%token RDO_Relevant_resources			304
%token RDO_Keep							305
%token RDO_Create						306
%token RDO_Erase						307
%token RDO_NonExist						308
%token RDO_IDENTIF_NoChange				309
%token RDO_Time							310
%token RDO_Choice						311
%token RDO_from							312
%token RDO_first						313
%token RDO_Convert_begin				314
%token RDO_Convert_end					315
%token RDO_Convert_rule					316
%token RDO_Convert_event				317
%token RDO_with_max						318
%token RDO_with_min						319
%token RDO_set							320
%token RDO_IDENTIF_NoChange_NoChange	321
%token RDO_Operations					322
	
%token RDO_Results						323
%token RDO_watch_par					324
%token RDO_watch_state					325
%token RDO_watch_quant					326
%token RDO_watch_value					327
%token RDO_get_value					328

%token RDO_Model_name					329
%token RDO_Resource_file				330
%token RDO_OprIev_file					331
%token RDO_Frame_file					332
%token RDO_Statistic_file				333
%token RDO_Results_file					334
%token RDO_Trace_file					335
%token RDO_Show_mode					336
%token RDO_Frame_number					337
%token RDO_Show_rate					338
%token RDO_Run_StartTime				339
%token RDO_Trace_StartTime				340
%token RDO_Trace_EndTime				341
%token RDO_Terminate_if					342
%token RDO_Break_point					343
%token RDO_Seed							344
%token RDO_NoShow						345
%token RDO_Monitor						346
%token RDO_Animation					347
%token RDO_NoChange						348

%token RDO_Decision_point				349
%token RDO_search						350
%token RDO_trace_stat					351
%token RDO_trace_tops					352
%token RDO_trace_all					353
%token RDO_Condition					354
%token RDO_Term_condition				355
%token RDO_Evaluate_by					356
%token RDO_Compare_tops					357
%token RDO_NO							358
%token RDO_YES							359
%token RDO_Activities					360
%token RDO_value_before					361
%token RDO_value_after					362
%token RDO_some							363
%token RDO_Process						364
%token RDO_SEIZE						365
%token RDO_GENERATE						366
%token RDO_TERMINATE					367
%token RDO_ADVANCE						368
%token RDO_RELEASE						369
%token RDO_if							370
%token RDO_result						371
%token RDO_CF							372
%token RDO_Priority						373
%token RDO_prior						374
%token RDO_Parent						375
%token RDO_PlusEqual					376
%token RDO_MinusEqual					377
%token RDO_MultiplyEqual				378
%token RDO_DivideEqual					379

%token RDO_Frame						400
%token RDO_Show_if						401
%token RDO_Back_picture					402
%token RDO_Show							403
%token RDO_frm_cell						404
%token RDO_text							405
%token RDO_bitmap						406
%token RDO_s_bmp						407
%token RDO_rect							408
%token RDO_r_rect						409
%token RDO_line							410
%token RDO_ellipse						411
%token RDO_triang						412
%token RDO_active						413
%token RDO_ruler						414
%token RDO_space						415
%token RDO_color_transparent			416
%token RDO_color_last					417
%token RDO_color_white					418
%token RDO_color_black					419
%token RDO_color_red					420
%token RDO_color_green					421
%token RDO_color_blue					422
%token RDO_color_cyan					423
%token RDO_color_magenta				424
%token RDO_color_yellow					425
%token RDO_color_gray					426

%token RDO_IDENTIF_RELRES				427

%token RDO_STRING_CONST					430
%token RDO_STRING_CONST_BAD				431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437
%token RDO_string						438
%token RDO_bool							439
%token RDO_BOOL_CONST					440
%token RDO_Fuzzy						441
%token RDO_Fuzzy_Term					442
%token RDO_eq							443
%token RDO_External_Model				444
%token RDO_QUEUE						445
%token RDO_DEPART						446
%token RDO_ASSIGN						447


%{
#include "rdo_lib/rdo_parser/pch.h"

#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdopmd.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"

#define PARSER  reinterpret_cast<rdoParse::RDOLexer*>(lexer)->m_parser
#define RUNTIME PARSER->runtime()

namespace rdoParse 
{
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start dptrtp_main

%%

/* ///////////////////////  GENERAL PART ///////////////////////////// */
dptrtp_main:
	| dptrtp_main RDO_Decision_point error RDO_End /* �������� ��� $Decision_point */
	| dptrtp_main RDO_Activities error RDO_End     /* �������� ��� $Activities     */
	| dptrtp_main dpt_process_end;

/* ///////////////////////  PROCESS ///////////////////////////// */

dpt_process:		dpt_process_header dpt_process_input;

dpt_process_header:	dpt_process_begin dpt_process_condition dpt_process_prior;

dpt_process_begin:	RDO_Process;

dpt_process_condition:	/* empty */
					| RDO_Condition error;

dpt_process_prior:	/* empty */
					| RDO_Priority error;

dpt_process_input:	/* empty */
					| dpt_process_input dpt_process_line
					| dpt_process_input error dpt_process_line;

dpt_process_line:	  RDO_IDENTIF					  {		}
					| RDO_GENERATE					  {		}
					| RDO_TERMINATE					  {		}
					| RDO_ADVANCE					  {		}
					| RDO_ASSIGN					  { 	}
					| RDO_QUEUE dpt_queue_param		  {		}
					| RDO_QUEUE error				  {	PARSER->error(@1, rdo::format("��������� ��� ������� ��� ����� ���������� �� �������"));			}
					| RDO_DEPART dpt_depart_param	  {		}
					| RDO_DEPART error				  { PARSER->error(@1, rdo::format("��������� ��� ������� ��� ����� ���������� �� �������"));			}
					/*| RDO_RELEASE dpt_release_param   {		}
					| RDO_RELEASE error				  { PARSER->error(@1, rdo::format("��������� ��� �������������� �������"));								}
					| RDO_SEIZE dpt_seize_param		  {		}
					| RDO_SEIZE error				  { PARSER->error(@1, rdo::format("��������� ��� ����������� �������"));								}*/
					| RDO_SEIZE dpt_seize_param	  {		}
					| RDO_SEIZE error				  { PARSER->error(@1, rdo::format("��������� ������ ��������, ������������ � ����, ����� �������"));	}
					| RDO_RELEASE dpt_release_param { 	}
					| RDO_RELEASE error			  { PARSER->error(@1, rdo::format("��������� ������ ��������, ������������ � ����, ����� �������"));	};
					
dpt_queue_param:	RDO_IDENTIF 
					{
						// ��� �������
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						// �������� ������ ���� ����� ��������
						rdoMBuilder::RDOResTypeList rtpList( PARSER );
						rdoMBuilder::RDOResType rtp;
						std::string rtp_name = "QDEPART";
						std::string q_name = "�������_"  + res_name;
							// ���� ������ ����������, ����� ��� ��� � ���������
							if (rssList[res_name].exist())
							{
								rtp = rssList[res_name].getType();
								RDOPROCBlockForQueue::checkType(PARSER, rtp, info);
								new RDOPMDWatchPar( PARSER, q_name, 0, res_name, std::string( "�����_�������" ) );
							}
							else
							{
								//���� ��� "QDEPART" ����������
								if ( rtpList[rtp_name].exist() )
								{
									rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
										if( RDOPROCBlockForQueue::checkType(PARSER, rtp_, info) )
										{
											RDOPROCBlockForQueue::createRes( PARSER, rtp_, res_name );
											new RDOPMDWatchPar( PARSER, q_name, 0, res_name, std::string( "�����_�������" ) );
										}
								}
								else
								{
									rdoMBuilder::RDOResType rtp_ = RDOPROCBlockForQueue::createType( PARSER, rtp_name, info );
									if( RDOPROCBlockForQueue::checkType(PARSER, rtp_, info) )
									{
										RDOPROCBlockForQueue::createRes( PARSER, rtp_, res_name );
										new RDOPMDWatchPar( PARSER, q_name, 0, res_name, std::string( "�����_�������" ) );
									}
								}
							}
					}
					| RDO_IDENTIF error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};
dpt_depart_param:	RDO_IDENTIF 
					{
						// ��� �������
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						// �������� ������ ���� ����� ��������
						rdoMBuilder::RDOResTypeList rtpList( PARSER );
						rdoMBuilder::RDOResType rtp;
						std::string rtp_name = "QDEPART";
						// ���� ������ ����������, ����� ��� ��� � ���������
							if (rssList[res_name].exist())
							{
								rtp = rssList[res_name].getType();
								RDOPROCBlockForQueue::checkType(PARSER, rtp, info);
							}
							else
							{
								if ( rtpList[rtp_name].exist() )
								{
									rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
										if( RDOPROCBlockForQueue::checkType(PARSER, rtp_, info) )
										{
											RDOPROCBlockForQueue::createRes( PARSER, rtp_, res_name );
										}
								}
								else
								{	
									rdoMBuilder::RDOResType rtp_ = RDOPROCBlockForQueue::createType( PARSER, rtp_name, info );
									if( RDOPROCBlockForQueue::checkType(PARSER, rtp_, info) )
									{
										RDOPROCBlockForQueue::createRes( PARSER, rtp_, res_name );
									}
								}
							}
					}
					| RDO_IDENTIF error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};
/*dpt_seize_param:	RDO_IDENTIF 
					{
						// ��� �������
						std::string res_name         = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						// ���� ������ ����������, ����� ��� ��� � ���������
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{
							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| RDO_IDENTIF error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};

dpt_release_param:	RDO_IDENTIF 
					{
						// ��� �������
						std::string res_name          = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info  = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						// ���� ������ ���������� ����� ��� ��� � ���������
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{
							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| RDO_IDENTIF error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};
*/
dpt_seize_param:	RDO_IDENTIF
					{
						// ��� �������
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						// ���� ������ ����������, ����� ��� ��� � ���������
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{
							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| dpt_seize_param ',' RDO_IDENTIF
					{
						// ��� �������
						std::string res_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info = @3;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						// ���� ������ ����������, ����� ��� ��� � ���������
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{
							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| dpt_seize_param error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};
dpt_release_param:	RDO_IDENTIF
					{
						// ��� �������
						std::string res_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info  = @1;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{

							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| dpt_release_param ',' RDO_IDENTIF
					{
						// ��� �������
						std::string res_name          = reinterpret_cast<RDOValue*>($3)->value().getIdentificator().c_str();
						const RDOParserSrcInfo& info  = @3;
						// �������� ������ ���� ��������
						rdoMBuilder::RDOResourceList rssList( PARSER );
						rdoMBuilder::RDOResType rtp;
						// ���� ������ ���������� ����� ��� ��� � ���������
						if ( rssList[res_name].exist())
						{
							rtp = rssList[res_name].getType();
							if (RDOPROCBlockForSeize::checkType(PARSER, rtp, info))
							{
								if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(*PARSER)) 
								{
									RDOPROCBlockForSeize::reobjectRes(PARSER, rtp, res_name);
								}
							}
						}
						else
						{
							//������ �� ������, ������������ ��� ���� �� ����� �������
							// ������������ ��� ���� �� ����� �������
							std::string rtp_name( RDOPROCProcess::s_name_prefix + res_name + RDOPROCProcess::s_name_sufix );
							// �������� ������ ���� ����� ��������
							rdoMBuilder::RDOResTypeList rtpList( PARSER );
							// ����� ��� �������
							if ( rtpList[rtp_name].exist() )
							{
								rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
								if (RDOPROCBlockForSeize::checkType(PARSER, rtp_, info))
								{
									RDOPROCBlockForSeize::createRes(PARSER, rtp_, res_name);
								}
							}
							else
							{
								rtp = RDOPROCBlockForSeize::createType(PARSER, rtp_name, info);
								RDOPROCBlockForSeize::createRes(PARSER, rtp, res_name);
							}
						}
					}
					| dpt_release_param error
					{
						PARSER->error( @1, "������ � ����� �������" );
					};	


dpt_process_end:	dpt_process RDO_End
					{
					};

%%

}
