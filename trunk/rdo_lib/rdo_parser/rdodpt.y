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
%token RDO_irregular_event				300
%token RDO_rule							301
%token RDO_keyboard						302
%token RDO_Relevant_resources			303
%token RDO_Keep							304
%token RDO_Create						305
%token RDO_Erase						306
%token RDO_NonExist						307
%token RDO_IDENTIF_NoChange				308
%token RDO_Time							309
%token RDO_Choice						310
%token RDO_from							311
%token RDO_first						312
%token RDO_Convert_begin				313
%token RDO_Convert_end					314
%token RDO_Convert_rule					315
%token RDO_Convert_event				316
%token RDO_with_max						317
%token RDO_with_min						318
%token RDO_IDENTIF_set					319
%token RDO_IDENTIF_NoChange_NoChange	320
%token RDO_Operations					321
	
%token RDO_Results						322
%token RDO_watch_par					323
%token RDO_watch_state					324
%token RDO_watch_quant					325
%token RDO_watch_value					326
%token RDO_get_value					327

%token RDO_Model_name					328
%token RDO_Resource_file				329
%token RDO_OprIev_file					330
%token RDO_Frame_file					331
%token RDO_Statistic_file				332
%token RDO_Results_file					333
%token RDO_Trace_file					334
%token RDO_Show_mode					335
%token RDO_Frame_number					336
%token RDO_Show_rate					337
%token RDO_Run_StartTime				338
%token RDO_Trace_StartTime				339
%token RDO_Trace_EndTime				340
%token RDO_Terminate_if					341
%token RDO_Break_point					342
%token RDO_Seed							343
%token RDO_NoShow						344
%token RDO_Monitor						345
%token RDO_Animation					346
%token RDO_NoChange						347

%token RDO_Decision_point				348
%token RDO_search						349
%token RDO_trace_stat					350
%token RDO_trace_tops					351
%token RDO_trace_all					352
%token RDO_Condition					353
%token RDO_Term_condition				354
%token RDO_Evaluate_by					355
%token RDO_Compare_tops					356
%token RDO_NO							357
%token RDO_YES							358
%token RDO_Activities					359
%token RDO_value_before					360
%token RDO_value_after					361
%token RDO_some							362
%token RDO_Process						363
%token RDO_SEIZE						364
%token RDO_GENERATE						365
%token RDO_TERMINATE					366
%token RDO_ADVANCE						367
%token RDO_RELEASE						368
%token RDO_if							369
%token RDO_result						370

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

%token RDO_QUOTED_IDENTIF				430
%token RDO_QUOTED_IDENTIF_BAD			431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdofun.h"
#include "rdodpt.h"
#include "rdortp.h"
#include "rdorss.h"

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

%start dpt_main

%%

// ----------------------------------------------------------------------------
// ---------- DPT
// ----------------------------------------------------------------------------
dpt_main:
		| dpt_main dpt_search_end
		| dpt_main dpt_some_end
		| dpt_main dpt_free_end
		| dpt_main dpt_process_end
		| error {
			PARSER->error( @1, "��������� �������� ����� ��� ���������� ����� �����������" );
		};

// ----------------------------------------------------------------------------
// ---------- DPT Search
// ----------------------------------------------------------------------------
dpt_search_trace:		/* empty */ {
							$$ = rdoRuntime::RDODPTSearchTrace::DPT_no_trace;
						}
						| RDO_no_trace {
							$$ = rdoRuntime::RDODPTSearchTrace::DPT_no_trace;
						}
						| RDO_trace {
							PARSER->error( @1, "������ ������� ����������� �� ������������ � ����� ���� search" );
						}
						| RDO_trace_stat {
							$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_stat;
						}
						| RDO_trace_tops {
							$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_tops;
						}
						| RDO_trace_all {
							$$ = rdoRuntime::RDODPTSearchTrace::DPT_trace_all;
						};

dpt_search_begin:		RDO_Decision_point RDO_IDENTIF_COLON RDO_search dpt_search_trace {
							std::string name = *reinterpret_cast<std::string*>($2);
							$$ = (int)new RDODPTSearch( PARSER, RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext), *reinterpret_cast<rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag*>(&$4) );
						}
						| RDO_Decision_point RDO_IDENTIF_COLON error {
							PARSER->error( @2, @3, "��������� ��� �����" );
						}
						| RDO_Decision_point RDO_IDENTIF error {
							PARSER->error( @2, "��������� ���������" );
						}
						| RDO_Decision_point error {
							PARSER->error( @1, @2, "����� ��������� ����� $Decision_point ��������� ��� �����" );
						};

dpt_search_condition:	dpt_search_begin RDO_Condition fun_logic {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setCondition((RDOFUNLogic *)$3);
						}
						| dpt_search_begin RDO_Condition RDO_NoCheck {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setCondition();
						}
						| dpt_search_begin RDO_Condition error {
							PARSER->error( @2, @3, "����� ��������� ����� $Condition ��������� ������� ������ ������ (��������� �������)" );
						}
						| dpt_search_begin error {
							PARSER->error( @2, "��������� �������� ����� $Condition" );
						};

dpt_search_term:		dpt_search_condition RDO_Term_condition fun_logic {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setTermCondition((RDOFUNLogic *)$3);
						}
						| dpt_search_condition RDO_Term_condition RDO_NoCheck {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setTermCondition();
						}
						| dpt_search_condition RDO_Term_condition error {
							PARSER->error( @2, @3, "����� ��������� ����� $Term_condition ��������� ������� ��������� ������ (�������� �������)" );
						}
						| dpt_search_condition error {
							PARSER->error( @2, "��������� �������� ����� $Term_condition" );
						};

dpt_search_evaluate:	dpt_search_term RDO_Evaluate_by fun_arithm {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setEvaluateBy((RDOFUNArithm *)$3);
						}
						| dpt_search_term RDO_Evaluate_by error {
							PARSER->error( @2, @3, "����� ��������� ����� $Evaluate_by ��������� ��������� �������, ��������, 0 ��� ������ � ������" );
						}
						| dpt_search_term error {
							PARSER->error( @2, "��������� �������� ����� $Evaluate_by" );
						};

dp_searcht_compare:		dpt_search_evaluate RDO_Compare_tops '=' RDO_NO {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setCompareTops(false);
						}
						| dpt_search_evaluate RDO_Compare_tops '=' RDO_YES {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->setCompareTops(true);
						}
						| dpt_search_evaluate RDO_Compare_tops '=' error {
							PARSER->error( @3, @4, "��������� ����� ����������� ���������� ������ (YES ��� NO)" );
						}
						| dpt_search_evaluate RDO_Compare_tops error {
							PARSER->error( @2, @3, "��������� ���� ���������" );
						}
						| dpt_search_evaluate error {
							PARSER->error( @2, "��������� �������� ����� $Compare_tops" );
						};

dpt_search_descr_param:	/* empty */
						| dpt_search_descr_param RDO_IDENTIF {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							std::string param = *reinterpret_cast<std::string*>($2);
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_search_descr_param RDO_INT_CONST {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							int         param = $2;
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_search_descr_param RDO_REAL_CONST {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							double      param = *reinterpret_cast<double*>($2);
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_search_descr_param '*' {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							dpt->getLastActivity()->addParam( @2 );
						};

dpt_search_descr_value:	RDO_value_before fun_arithm {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							dpt->getLastActivity()->setValue( rdoRuntime::RDODPTSearch::Activity::vt_before, reinterpret_cast<RDOFUNArithm*>($2), @1 );
						}
						| RDO_value_after fun_arithm {
							RDODPTSearch* dpt = PARSER->getLastDPTSearch();
							dpt->getLastActivity()->setValue( rdoRuntime::RDODPTSearch::Activity::vt_after, reinterpret_cast<RDOFUNArithm*>($2), @1 );
						}
						| RDO_value_before error {
							PARSER->error( @1, @2, "������ � �������������� ���������" );
						}
						| RDO_value_after error {
							PARSER->error( @1, @2, "������ � �������������� ���������" );
						};

dpt_search_name:		RDO_IDENTIF_COLON RDO_IDENTIF {
							RDODPTSearch* dpt   = PARSER->getLastDPTSearch();
							std::string name    = *reinterpret_cast<std::string*>($1);
							std::string pattern = *reinterpret_cast<std::string*>($2);
							$$ = (int)dpt->addNewActivity( RDOParserSrcInfo(@1, name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@2, pattern) );
						}
						| RDO_IDENTIF_COLON error {
							PARSER->error( @1, @2, "��������� ��� �������" );
						}
						| RDO_IDENTIF {
							PARSER->error( @1, "��������� ':'" );
						}
						| error {
							PARSER->error( @1, "��������� ��� ����������" );
						};

dpt_searcht_activity:	/* empty */
						| dpt_searcht_activity dpt_search_name dpt_search_descr_param dpt_search_descr_value {
							RDODPTSearchActivity* activity = reinterpret_cast<RDODPTSearchActivity*>($2);
							activity->endParam( @3 );
						}
						| dpt_searcht_activity dpt_search_name dpt_search_descr_param error {
							PARSER->error( @3, @4, "��������� �������� ����� value before ��� value after � ��������� ���������� �������" );
						};

dpt_search_header:		dp_searcht_compare RDO_Activities dpt_searcht_activity {
						}
						| dp_searcht_compare error {
							PARSER->error( @1, @2, "����� ������ ����������� ���������� ������ ��������� �������� ����� $Activities" );
						};

dpt_search_end:			dpt_search_header RDO_End {
							RDODPTSearch* dpt = reinterpret_cast<RDODPTSearch*>($1);
							dpt->end();
						}
						| dpt_search_header {
							PARSER->error( @1, "��������� �������� ����� $End" );
						};

// ----------------------------------------------------------------------------
// ---------- DPT Some
// ----------------------------------------------------------------------------
dpt_some_trace:			/* empty */ {
							$$ = 1;
						}
						| RDO_no_trace {
							$$ = 1;
						}
						| RDO_trace {
							$$ = 2;
						}
						| RDO_trace_stat {
							PARSER->error( @1, "������ ������� ����������� �� ������������ � ����� ���� some" );
						}
						| RDO_trace_tops {
							PARSER->error( @1, "������ ������� ����������� �� ������������ � ����� ���� some" );
						}
						| RDO_trace_all {
							PARSER->error( @1, "������ ������� ����������� �� ������������ � ����� ���� some" );
						};

dpt_some_begin:			RDO_Decision_point RDO_IDENTIF_COLON RDO_some dpt_some_trace {
							// TODO: � ��� ������� ����������� ��� some ?
							std::string name = *reinterpret_cast<std::string*>($2);
							$$ = (int)new RDODPTSome( PARSER, RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext) );
						};

dpt_some_condition:		dpt_some_begin RDO_Condition fun_logic {
							RDODPTSome* dpt = reinterpret_cast<RDODPTSome*>($1);
							dpt->setCondition( reinterpret_cast<RDOFUNLogic*>($3) );
						}
						| dpt_some_begin RDO_Condition RDO_NoCheck {
							RDODPTSome* dpt = reinterpret_cast<RDODPTSome*>($1);
							dpt->setCondition();
						}
						| dpt_some_begin RDO_Condition error {
							PARSER->error( @2, @3, "����� ��������� ����� $Condition ��������� ������� ������� �����" );
						}
						| dpt_some_begin {
							RDODPTSome* dpt = reinterpret_cast<RDODPTSome*>($1);
							dpt->setCondition();
						};

dpt_some_name:			RDO_IDENTIF_COLON RDO_IDENTIF {
							RDODPTSome* dpt     = PARSER->getLastDPTSome();
							std::string name    = *reinterpret_cast<std::string*>($1);
							std::string pattern = *reinterpret_cast<std::string*>($2);
							$$ = (int)dpt->addNewActivity( RDOParserSrcInfo(@1, name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@2, pattern) );
						}
						| RDO_IDENTIF_COLON error {
							PARSER->error( @1, @2, "��������� ��� �������" );
						};

dpt_some_descr_param:	/* empty */
						| dpt_some_descr_param RDO_IDENTIF {
							RDODPTSome*  dpt   = PARSER->getLastDPTSome();
							std::string  param = *reinterpret_cast<std::string*>($2);
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_some_descr_param RDO_INT_CONST {
							RDODPTSome* dpt   = PARSER->getLastDPTSome();
							int         param = $2;
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_some_descr_param RDO_REAL_CONST {
							RDODPTSome* dpt   = PARSER->getLastDPTSome();
							double      param = *reinterpret_cast<double*>($2);
							dpt->getLastActivity()->addParam( param, @2 );
						}
						| dpt_some_descr_param '*' {
							RDODPTSome* dpt = PARSER->getLastDPTSome();
							dpt->getLastActivity()->addParam( @2 );
						}
						| dpt_some_descr_param error {
							PARSER->error( @1, @2, "������ �������� ��������� �������" )
						};

dpt_some_activity:		/* empty */
						| dpt_some_activity dpt_some_name dpt_some_descr_param {
							RDODPTSomeActivity* activity = reinterpret_cast<RDODPTSomeActivity*>($2);
							activity->endParam( @3 );
						};

dpt_some_header:		dpt_some_condition RDO_Activities dpt_some_activity {
						}
						| dpt_some_condition error {
							PARSER->error( @1, @2, "��������� �������� ����� $Activities" );
						};

dpt_some_end:			dpt_some_header RDO_End {
							RDODPTSome* dpt = reinterpret_cast<RDODPTSome*>($1);
							dpt->end();
						}
						| dpt_some_header {
							PARSER->error( @1, "��������� �������� ����� $End" );
						};

// ----------------------------------------------------------------------------
// ---------- DPT Free
// ----------------------------------------------------------------------------
dpt_free_header:			RDO_Activities {
								$$ = (int)new RDODPTFree( PARSER, @1 );
							};

dpt_free_activity:			/* empty */
							| dpt_free_activity dpt_free_activity_name dpt_free_activity_param dpt_free_activity_keys {
							};

dpt_free_activity_name:		RDO_IDENTIF_COLON RDO_IDENTIF {
								RDODPTFree* dpt     = PARSER->getLastDPTFree();
								std::string name    = *reinterpret_cast<std::string*>($1);
								std::string pattern = *reinterpret_cast<std::string*>($2);
								$$ = (int)dpt->addNewActivity( RDOParserSrcInfo(@1, name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@2, pattern) );
							}
							| RDO_IDENTIF_COLON error {
								PARSER->error( @1, @2, "��������� ��� �������" );
							};

dpt_free_activity_param:	/* empty */
							| dpt_free_activity_param RDO_IDENTIF {
								RDODPTFree*  dpt   = PARSER->getLastDPTFree();
								std::string  param = *reinterpret_cast<std::string*>($2);
								dpt->getLastActivity()->addParam( param, @2 );
							}
							| dpt_free_activity_param RDO_INT_CONST {
								RDODPTFree*  dpt   = PARSER->getLastDPTFree();
								int          param = $2;
								dpt->getLastActivity()->addParam( param, @2 );
							}
							| dpt_free_activity_param RDO_REAL_CONST {
								RDODPTFree*  dpt   = PARSER->getLastDPTFree();
								double       param = *reinterpret_cast<double*>($2);
								dpt->getLastActivity()->addParam( param, @2 );
							}
							| dpt_free_activity_param '*' {
								RDODPTFree*  dpt = PARSER->getLastDPTFree();
								dpt->getLastActivity()->addParam( @2 );
							}
							| dpt_free_activity_param error {
								PARSER->error( @1, @2, "������ �������� ��������� �������" )
							};

dpt_free_activity_keys:		/* empty */
							| dpt_free_activity_keys RDO_QUOTED_IDENTIF {
								RDODPTFreeActivity* activity = PARSER->getLastDPTFree()->getLastActivity();
								std::string         key      = *reinterpret_cast<std::string*>($2);
								activity->addHotKey( key, @2 );
							}
							| dpt_free_activity_keys '+' RDO_QUOTED_IDENTIF {
								RDODPTFreeActivity* activity = PARSER->getLastDPTFree()->getLastActivity();
								std::string         key      = *reinterpret_cast<std::string*>($3);
								activity->addHotKey( key, @3 );
							};

dpt_free_end:				dpt_free_header dpt_free_activity RDO_End {
							}
							| dpt_free_header error {
								PARSER->error( @1, "��������� �������� ����� $End" );
							};

// ----------------------------------------------------------------------------
// ---------- $Process
// ----------------------------------------------------------------------------
dpt_process_end:	RDO_Process error RDO_End; /* �������� ��� $Process */

// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
fun_logic:	fun_arithm '=' fun_arithm         { $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm RDO_neq fun_arithm   { $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm RDO_leq fun_arithm   { $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm RDO_geq fun_arithm   { $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic RDO_and fun_logic     { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic RDO_or fun_logic      { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| '[' fun_logic error {
				PARSER->error( @2, "��������� ������������� ������" );
			}
			| '(' fun_logic error {
				PARSER->error( @2, "��������� ������������� ������" );
			}
			| RDO_not fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group {
			}
			| fun_select_logic {
			};

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')' {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call {
			}
			| fun_select_arithm {
			}
			| RDO_IDENTIF '.' RDO_IDENTIF {
				$$ = (int)new RDOFUNArithm( PARSER, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| RDO_INT_CONST               { $$ = (int)new RDOFUNArithm( PARSER, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| RDO_REAL_CONST              { $$ = (int)new RDOFUNArithm( PARSER, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| RDO_IDENTIF                 { $$ = (int)new RDOFUNArithm( PARSER, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec RDO_UMINUS {
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( PARSER, reinterpret_cast<RDOFUNArithm*>($2)->typeID(), new rdoRuntime::RDOCalcUMinus( RUNTIME, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			};

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call:	RDO_IDENTIF '(' ')' {
							RDOFUNParams* fun = new RDOFUNParams( PARSER );
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @3 );
							fun->setSrcText( fun_name + "()" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' error {
							PARSER->error( @3, "������ � ���������� �������" );
						};

fun_arithm_func_call_pars:	fun_arithm {
								RDOFUNParams* fun = new RDOFUNParams( PARSER );
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
								fun->setSrcText( arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
								fun->setSrcText( fun->src_text() + ", " + arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars error {
								PARSER->error( @2, "������ � �������������� ���������" );
							}
							| fun_arithm_func_call_pars ',' error {
								PARSER->error( @3, "������ � �������������� ���������" );
							};

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_group_header:	fun_group_keyword '(' RDO_IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						$$ = (int)(new RDOFUNGroupLogic( PARSER, (RDOFUNGroupLogic::FunGroupType)$1, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) ));
					}
					| fun_group_keyword '(' error {
						PARSER->error( @3, "��������� ��� ����" );
					}
					| fun_group_keyword error {
						PARSER->error( @1, "����� ����� ������� ��������� ������������� ������" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header RDO_NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( RUNTIME, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						PARSER->error( @2, "��������� ������������� ������" );
					}
					| fun_group_header RDO_NoCheck error {
						PARSER->error( @2, "��������� ������������� ������" );
					}
					| fun_group_header error {
						PARSER->error( @1, @2, "������ � ���������� ���������" )
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	RDO_Select '(' RDO_IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( PARSER, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) );
						select->setSrcText( "Select(" + type_name + ": " );
						$$ = (int)select;
					}
					| RDO_Select '(' error {
						PARSER->error( @3, "��������� ��� ����" );
					}
					| RDO_Select error {
						PARSER->error( @1, "��������� ������������� ������" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header RDO_NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( RUNTIME, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						PARSER->error( @2, "��������� ������������� ������" );
					}
					| fun_select_header RDO_NoCheck error {
						PARSER->error( @2, "��������� ������������� ������" );
					}
					| fun_select_header error {
						PARSER->error( @1, @2, "������ � ���������� ���������" )
					};

fun_select_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( (RDOFUNGroupLogic::FunGroupType)$3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' fun_select_keyword '(' error {
						PARSER->error( @4, @5, "������ � ���������� ���������" )
					}
					| fun_select_body '.' fun_select_keyword error {
						PARSER->error( @3, "��������� ������������� ������" );
					}
					| fun_select_body '.' RDO_Empty '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body '.' RDO_Empty '(' error {
						PARSER->error( @4, "��������� ������������� ������" );
					}
					| fun_select_body '.' RDO_Empty error {
						PARSER->error( @3, "��������� ������������� ������" );
					}
					| fun_select_body '.' error {
						PARSER->error( @2, @3, "��������� ����� ������ ��������" );
					}
					| fun_select_body error {
						PARSER->error( @1, "��������� '.' (�����) ��� ������ ������ ������ ��������" );
					};

fun_select_arithm:	fun_select_body '.' RDO_Size '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body '.' RDO_Size error {
						PARSER->error( @3, "��������� ������������� ������" );
					}
					| fun_select_body '.' RDO_Size '(' error {
						PARSER->error( @4, "��������� ������������� ������" );
					};

%%

}
