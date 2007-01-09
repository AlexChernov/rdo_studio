%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent_kw			258
%token Parameters			259
%token integer				260
%token real					261
%token End					262
%token temporary_kw			263
%token IDENTIF				264
%token INT_CONST			265
%token REAL_CONST			266
%token such_as				267
%token dblpoint				268
%token Resources			269
%token trace_keyword		270
%token no_trace				271
%token IDENTIF_COLON		272
%token Constant				273
%token Body					274
%token Function_keyword		275
%token Type_keyword			276
%token algorithmic			277
%token table_keyword		278
%token list_keyword			279
%token Exist				281
%token Not_Exist			282
%token For_All				283
%token Not_For_All			284
%token neq					285
%token leq					286
%token geq					287
%token NoCheck				288
%token Calculate_if			289
%token or_keyword			290
%token and_keyword			291
%token Sequence				292
%token uniform				293
%token exponential			294
%token normal_keyword		295
%token by_hist				296
%token enumerative			297

%token Pattern				298
%token operation_kw			299
%token irregular_event		300
%token rule_keyword			301
%token keyboard				302
%token Relevant_resources	303
%token Keep					304
%token Create_keyword		305
%token Erase				306
%token NonExist				307
%token IDENTIF_NoChange		308
%token Time					309
%token Choice				310
%token from_keyword			311
%token first_keyword		312
%token Convert_begin		313
%token Convert_end			314
%token Convert_rule			315
%token Convert_event		316
%token with_max				317
%token with_min				318
%token IDENTIF_set			319
%token IDENTIF_NoChange_NoChange	320
%token Operations			321
	
%token Results				322
%token watch_par			323
%token watch_state			324
%token watch_quant			325
%token watch_value			326
%token get_value			327

%token Model_name			328
%token Resource_file		329
%token OprIev_file			330
%token Frame_file			331
%token Statistic_file		332
%token Results_file			333
%token Trace_file			334
%token Show_mode			335
%token Frame_number			336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime		339
%token Trace_EndTime		340
%token Terminate_if			341
%token Break_point			342
%token Seed					343
%token NoShow				344
%token Monitor				345
%token Animation			346
%token NoChange				347

%token Decision_point		348
%token search_keyword		349
%token trace_stat			350
%token trace_tops			351
%token trace_all			352
%token Condition_keyword	353
%token Term_condition		354
%token Evaluate_by			355
%token Compare_tops			356
%token NO					357
%token YES					358
%token Activities			359
%token value_before			360
%token value_after			361
%token some					362
%token Process				363
%token SEIZE				364
%token GENERATE				365
%token TERMINATE			366
%token ADVANCE				367
%token RELEASE				368

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token transparent			406
%token bitmap				407
%token s_bmp				408
%token rect_keyword			409
%token r_rect				410
%token line					411
%token ellipse				412
%token triang				413
%token active				414
%token QUOTED_IDENTIF		415
%token QUOTED_IDENTIF_BAD	416
%token IDENTIF_BAD			417
%token Select				418
%token Size_kw				419
%token Empty_kw				420
%token not_keyword			421
%token UMINUS				422

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"

namespace rdoParse
{
%}

%start rtp_list

%%

rtp_list:	/* empty */
			| rtp_list rtp_res_type
			| error {
				parser->lexer_loc_set( &(@1) );
//				parser->error( rdoSimulator::RDOSyntaxError::UNKNOWN );
				parser->error( "��������� �������� ����� $Resource_type" );
			};

rtp_res_type:	rtp_res_type_hdr Parameters rtp_body End {
					if ( $3 == 0 ) {
						parser->lexer_loc_backup();			
						parser->lexer_loc_set( &(@2) );
						parser->warning( rdo::format( "��� ������� '%s' �� ������� ����������", ((RDORTPResType*)$1)->getName()->c_str() ) );
						parser->lexer_loc_restore();
					}
				}
				| rtp_res_type_hdr Parameters rtp_body {
					parser->lexer_loc_set( @3.last_line, @3.last_column );
					parser->error( "�� ������� �������� ����� $End" );
				}
				| rtp_res_type_hdr error {
					parser->error( "�� ������� �������� ����� $Parameters" );
				};

rtp_res_type_hdr:	Resource_type IDENTIF_COLON rtp_vid_res {
						reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 0;
						std::string *name = (std::string *)$2;
						if( parser->findRTPResType(name) ) {
							parser->lexer_loc_set( @2.first_line, @2.first_column + name->length() );
							parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_RES_TYPE, name->c_str() );
						}
						RDORTPResType *res = new RDORTPResType( name, $3 != 0 );
						$$ = (int)res;
					}
					| Resource_type error {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("������ � �������� ����� ���� �������: %s", str.c_str()) );
					}
					| Resource_type IDENTIF_COLON error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�� ������ ��� �������" );
					};

rtp_vid_res:	permanent_kw	{ $$ = 1; }
				| temporary_kw	{ $$ = 0; };

rtp_body:	/* empty */ {
				$$ = 0; // warning
			}
			| rtp_body rtp_param_desc {
				RDORTPParamDesc *param = (RDORTPParamDesc*)$2;
				parser->getLastRTPResType()->addParam( param );
				$$ = 1; // no warning
			};

rtp_param_desc: IDENTIF_COLON rtp_param_type {
					parser->lexer_loc_backup( &(@1) );
					std::string*     name    = (std::string*)$1;
					RDORTPResParam*  parType = (RDORTPResParam*)$2;
					RDORTPParamDesc* param   = new RDORTPParamDesc( name, parType );
					if ( parType->getType() == RDORTPResParam::pt_enum ) {
						static_cast<RDORTPEnumResParam*>(parType)->enum_name = rdo::format( "%s.%s", parser->getLastRTPResType()->getName()->c_str(), name->c_str() );
					}
					$$ = (int)param;
				}
				| IDENTIF_COLON error {
					if ( parser->lexer_loc_line() == @1.last_line ) {
						std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "�������� ��� ���������: %s", str.c_str() ) );
					} else {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ��� ���������" );
					}
				}
				| error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "������������ �������� ���������" );
				};

rtp_param_type:	integer rtp_int_diap rtp_int_default_val {
					RDORTPIntDiap *diap = (RDORTPIntDiap *)$2;
					RDORTPIntDefVal *dv = (RDORTPIntDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPIntResParam *rp = new RDORTPIntResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| real rtp_real_diap rtp_real_default_val {
					RDORTPRealDiap *diap = (RDORTPRealDiap *)$2;
					RDORTPRealDefVal *dv = (RDORTPRealDefVal *)$3;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@3) );
					RDORTPRealResParam *rp = new RDORTPRealResParam(diap, dv);
					parser->lexer_loc_restore();
					$$ = (int)rp;
				}
				| rtp_enum rtp_enum_default_val {
					reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum *enu = (RDORTPEnum *)$1;
					RDORTPEnumDefVal *dv = (RDORTPEnumDefVal *)$2;
					parser->lexer_loc_backup();
					parser->lexer_loc_set( &(@2) );
					if ( dv->exist ) {
						enu->findValue(dv->value);	 // if no value - Syntax exception will be thrown
					}
					parser->lexer_loc_restore();
					RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, dv);
					$$ = (int)rp;
				}
				| rtp_such_as {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs();
				}
				| rtp_such_as '=' INT_CONST {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs((int)$3);
				}
				| rtp_such_as '=' REAL_CONST {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs((double *)$3);
				}
				| rtp_such_as '=' IDENTIF {
					RDORTPParamDesc *desc = (RDORTPParamDesc *)$1;
					$$ = (int)desc->getType()->constructSuchAs((std::string *)$3);
				}
				| rtp_such_as '=' error {
					parser->error( "��������� ������� ��-���������" );
				}
				| rtp_such_as error {
					parser->error( "��������� ��������� �������� ���������-������, ��������, ������� ��-���������" );
				}
				| integer error {
					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| rtp_enum error {
					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};

rtp_int_diap:	/* empty */ {
					RDORTPIntDiap *diap = new RDORTPIntDiap();
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap *diap = new RDORTPIntDiap($2, $4);
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' REAL_CONST dblpoint INT_CONST {
					parser->lexer_loc_set( &(@2) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint REAL_CONST {
					parser->lexer_loc_set( &(@4) );
					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "�������� ����� �������" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

rtp_real_diap:	/* empty */ {
					RDORTPRealDiap *diap = new RDORTPRealDiap();
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *((double *)$2);
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *((double *)$2);
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *((double *)$4);
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap *diap = new RDORTPRealDiap(min, max);
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' REAL_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint REAL_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint INT_CONST error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' REAL_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' INT_CONST dblpoint error {
					parser->lexer_loc_set( &(@4) );
					parser->error( "�������� ����� �������" );
				}
				| '[' error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "�������� ����� �������" );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

rtp_int_default_val:	/* empty */ {
						$$ = (int)(new RDORTPIntDefVal());
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal($2));
					}
					| '=' REAL_CONST {
						// ����� ����� ���������������� ������������: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "�� ������� �������� ��-��������� ������ ����" );
					}
					| '=' error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "�������� �������� ��-��������� ������ ����" );
					};
//					| '=' error {
//						if ( parser->lexer_loc_line() == @1.first_line ) {
//							std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
//							if ( str.empty() ) {
//								parser->lexer_loc_set( &(@1) );
//								parser->error( "��������� ������������� �������� ��-���������" );
//							} else {
//								parser->lexer_loc_set( &(@2) );
//								parser->error( rdo::format( "�������� �������� ��-���������: %s", str.c_str() ) );
//							}
//						} else {
//							parser->lexer_loc_set( &(@1) );
//							parser->error( "��������� ������������� �������� ��-���������" );
//						}
//					};

rtp_real_default_val:	/* empty */ {
						$$ = (int)(new RDORTPRealDefVal());
					}
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2)));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ������������� ����" );
					};
//					| '=' error {
//						if ( parser->lexer_loc_line() == @1.first_line ) {
//							std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
//							if ( str.empty() ) {
//								parser->lexer_loc_set( &(@1) );
//								parser->error( "��������� ������������ �������� ��-���������" );
//							} else {
//								parser->lexer_loc_set( &(@2) );
//								parser->error( rdo::format( "�������� �������� ��-���������: %s", str.c_str() ) );
//							}
//						} else {
//							parser->lexer_loc_set( &(@1) );
//							parser->error( "��������� ������������ �������� ��-���������" );
//						}
//					};

rtp_enum:	'(' rtp_enum_list ')' {
				$$ = $2;
			}
			| '(' rtp_enum_list {
				parser->lexer_loc_set( &(@2) );
				parser->error( "������������ ������ ������������� �������" );
			};

rtp_enum_list:	IDENTIF {
					RDORTPEnum *enu = new RDORTPEnum((std::string *)$1);
					$$ = (int)enu;
					reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt = 1;
				}
				| rtp_enum_list ',' IDENTIF {
					if ( reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt >= 1 ) {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( &(@3) );
						RDORTPEnum *enu = (RDORTPEnum *)$1;
						enu->add((std::string *)$3);
						parser->lexer_loc_restore();
						$$ = (int)enu;
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| rtp_enum_list IDENTIF {
					if ( reinterpret_cast<RDOLexerRTP*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("��������� ������� �����: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| rtp_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "������ � �������� �������� ������������� ����" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "�������� �������� ������������� ����: %s", str.c_str() ) );
					}
				}
				| rtp_enum_list ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| rtp_enum_list ',' REAL_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| INT_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| REAL_CONST {
					parser->lexer_loc_set( &(@1) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| error {
					parser->lexer_loc_set( &(@1) );
					parser->error( "������ � �������� �������� ������������� ����" );
				};

rtp_enum_default_val:	/* empty */ {
						$$ = (int)(new RDORTPEnumDefVal());
					}
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal((std::string *)$2));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ������������� ����" );
					};
//					| '=' error {
//						if ( parser->lexer_loc_line() == @1.first_line ) {
//							std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
//							if ( str.empty() ) {
//								parser->lexer_loc_set( &(@1) );
//								parser->error( "��������� ������������ �������� ��-���������" );
//							} else {
//								parser->lexer_loc_set( &(@2) );
//								parser->error( rdo::format( "�������� �������� ��-���������: %s", str.c_str() ) );
//							}
//						} else {
//							parser->lexer_loc_set( &(@1) );
//							parser->error( "��������� ������������ �������� ��-���������" );
//						}
//					};

rtp_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string* type = (std::string *)$2;
					std::string* param = (std::string *)$4;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					}
					const RDORTPParamDesc *const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type->c_str(), param->c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF '.' {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "�� ������ ��������" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "������ ��� �������� ���������" );
					}
				}
				| such_as IDENTIF error {
					std::string* type = (std::string *)$2;
					const RDORTPResType *const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type->c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "����� ����� ���� ������ ���� ������ �������� ����� �����" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "����� ��������� ����� such_as ���������� ������� ��� � �������� ������� ��� ������" );
				};

%%

}
