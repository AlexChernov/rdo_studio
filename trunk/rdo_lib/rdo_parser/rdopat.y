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
#include "rdopat.h"
#include "rdortp.h"
#include "rdofun.h"
#include <rdotrace.h>

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

%%

pat_main:
			| pat_main pat_pattern;
			| error {
				PARSER->error( @1, "����������� ������" );
			};

pat_header:	RDO_Pattern RDO_IDENTIF_COLON RDO_operation pat_trace {
				std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				$$ = (int)(new RDOPatternOperation( PARSER, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_irregular_event pat_trace {
				std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				$$ = (int)(new RDOPatternIrregEvent( PARSER, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_rule pat_trace {
				std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				$$ = (int)(new RDOPatternRule( PARSER, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			}
			| RDO_Pattern RDO_IDENTIF_COLON RDO_keyboard pat_trace {
				std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				$$ = (int)(new RDOPatternKeyboard( PARSER, RDOParserSrcInfo( @2, name, RDOParserSrcInfo::psi_align_bytext ), $4 != 0 ));
			};
			| RDO_Pattern error {
				PARSER->error( @2, "��������� ��� �������" );
			}
			| RDO_Pattern RDO_IDENTIF_COLON error {
				PARSER->error( @2, @3, "��������� ��� �������" );
//			}
//			| RDO_Pattern RDO_IDENTIF_COLON RDO_irregular_event error {
//				PARSER->error( @2, @3, "��������� ������� �����������" );
//				PARSER->error( @2, @3, "��������� ������� �����������, �������� ���������� ��� ����������� �������� �������" );
			};

pat_trace:	/* empty */		{ $$ = 0; }
			| RDO_trace		{ $$ = 1; }
			| RDO_no_trace	{ $$ = 0; };

pat_params_begin: pat_header RDO_Parameters { $$ = $1; };

pat_params:	pat_params_begin RDO_IDENTIF_COLON param_type {
				RDOPATPattern*   pattern    = reinterpret_cast<RDOPATPattern*>($1);
				std::string      param_name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				RDORTPParamType* param_type = reinterpret_cast<RDORTPParamType*>($3);
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam( pattern, RDOParserSrcInfo( @2, param_name, RDOParserSrcInfo::psi_align_bytext ), param_type );
				pattern->add( param );
				if ( param_type->typeID() == rdoRuntime::RDOType::t_enum ) {
					static_cast<RDORTPEnumParamType*>(param_type)->enum_name = rdo::format( "%s.%s", pattern->name().c_str(), param_name.c_str() );
				}
			}
			| pat_params RDO_IDENTIF_COLON param_type {
				RDOPATPattern*   pattern    = reinterpret_cast<RDOPATPattern*>($1);
				std::string      param_name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				RDORTPParamType* param_type = reinterpret_cast<RDORTPParamType*>($3);
				RDOFUNFunctionParam* param = new RDOFUNFunctionParam( pattern, RDOParserSrcInfo( @2, param_name, RDOParserSrcInfo::psi_align_bytext ), param_type );
				pattern->add( param );
				if ( param_type->typeID() == rdoRuntime::RDOType::t_enum ) {
					static_cast<RDORTPEnumParamType*>(param_type)->enum_name = rdo::format( "%s.%s", pattern->name().c_str(), param_name.c_str() );
				}
			}
			| pat_params_begin error {
				if ( @1.last_line != @2.last_line ) {
					PARSER->error( @2, "��������� ��� ��������� �������" );
				} else {
					PARSER->error( @2, rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin RDO_IDENTIF error {
				if ( @2.last_line != @3.last_line ) {
					PARSER->error( @2, @3, "��������� ���������" );
				} else {
					PARSER->error( @2, @3, rdo::format("��������� ���������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin RDO_IDENTIF_COLON error {
				if ( @2.last_line != @3.last_line ) {
					PARSER->error( @2, @3, "��������� ��� ��������� �������" );
				} else {
					PARSER->error( @2, @3, rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params error {
				if ( @1.last_line != @2.last_line ) {
					PARSER->error( @2, "��������� ��� ��������� �������" );
				} else {
					PARSER->error( @2, rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params RDO_IDENTIF error {
				if ( @2.last_line != @3.last_line ) {
					PARSER->error( @2, @3, "��������� ���������" );
				} else {
					PARSER->error( @2, @3, rdo::format("��������� ���������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params RDO_IDENTIF_COLON error {
				if ( @2.last_line != @3.last_line ) {
					PARSER->error( @2, @3, "��������� ��� ��������� �������" );
				} else {
					PARSER->error( @2, @3, rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			};

pat_params_end:	pat_params RDO_Relevant_resources   { $$ = $1; }
				| pat_header RDO_Relevant_resources { $$ = $1; }
				| pat_header error {
					PARSER->error( @2, "��������� �������� ����� $Relevant_resources" );
				};

pat_rel_res:	pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @5, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @5, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResource::ConvertStatus)$4, (rdoRuntime::RDOResource::ConvertStatus)$5, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @5, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @5, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @4, rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResource::ConvertStatus)$4, @4 );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @4, rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr((rdoRuntime::RDOResource::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), (rdoRuntime::RDOResource::ConvertStatus)$4, @4 );
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_begin_pos = @3;
							std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
							rdo::toLower( str );
							std::string::size_type first_nochange = str.find( "nochange" );
							int i = 0;
							while ( true ) {
								if ( str[i] == '\n' ) {
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								} else if ( str[i] != '\r' ) {
									convertor_begin_pos.first_column++;
								}
								i++;
								if ( i == first_nochange ) break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @3, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @3, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_begin_pos = @3;
							std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
							rdo::toLower( str );
							std::string::size_type first_nochange = str.find( "nochange" );
							int i = 0;
							while ( true ) {
								if ( str[i] == '\n' ) {
									convertor_begin_pos.first_line++;
									convertor_begin_pos.first_column = 0;
								} else if ( str[i] != '\r' ) {
									convertor_begin_pos.first_column++;
								}
								i++;
								if ( i == first_nochange ) break;
							}
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @3, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @3, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @3, rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, convertor_pos );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @3, rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							pattern->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(type_pos, type_name), rdoRuntime::RDOResource::CS_NoChange, convertor_pos );
							break;
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name      = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name     = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							std::string convert_begin = reinterpret_cast<RDOValue*>($4)->value().getIdentificator();
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin.length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), pattern->StrToStatus( convert_begin, convertor_begin_pos ), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name      = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
							std::string type_name     = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
							std::string convert_begin = reinterpret_cast<RDOValue*>($4)->value().getIdentificator();
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin.length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
							static_cast<RDOPatternOperation*>(pattern)->addRelRes( RDOParserSrcInfo(@2, rel_name, RDOParserSrcInfo::psi_align_bytext), RDOParserSrcInfo(@3, type_name), pattern->StrToStatus( convert_begin, convertor_begin_pos ), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
				}
				| pat_params_end error {
					PARSER->error( @2, "������ � �������� ����������� ��������" );
				}
				| pat_rel_res error {
					PARSER->error( @2, "������ � �������� ����������� ��������" );
				}
				| pat_params_end RDO_IDENTIF_COLON error {
					PARSER->error( @2, @3, "��������� ��������� (��� ���� ��� �������)" );
				}
				| pat_rel_res RDO_IDENTIF_COLON error {
					PARSER->error( @2, @3, "��������� ��������� (��� ���� ��� �������)" );
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error {
					if ( PARSER->getLastPATPattern()->isHaveConvertEnd() ) {
						PARSER->error( @3, @4, "��������� ������ ���������� ������" );
					} else {
						PARSER->error( @3, @4, "��������� ������ ����������" );
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error {
					if ( PARSER->getLastPATPattern()->isHaveConvertEnd() ) {
						PARSER->error( @3, @4, "��������� ������ ���������� ������" );
					} else {
						PARSER->error( @3, @4, "��������� ������ ����������" );
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error {
					switch ( PARSER->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @5, "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @5, "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @4, @5, rdo::format("��������� ������ ���������� �����, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
							break;
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error {
					switch ( PARSER->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @5, "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @5, "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @4, @5, rdo::format("��������� ������ ���������� �����, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						}
					}
				}
				| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error {
					switch ( PARSER->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @3, @4, rdo::format("��������� ������ ���������� �����, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						}
					}
				}
				| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error {
					switch ( PARSER->getLastPATPattern()->getType() ) {
						case RDOPATPattern::PT_Rule: {
							PARSER->error( @4, "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							PARSER->error( @4, "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							PARSER->error( @3, @4, rdo::format("��������� ������ ���������� �����, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
							break;
						}
					}
				};

pat_conv:	RDO_Keep			{ $$ = rdoRuntime::RDOResource::CS_Keep;     }
			| RDO_Create		{ $$ = rdoRuntime::RDOResource::CS_Create;   }
			| RDO_Erase			{ $$ = rdoRuntime::RDOResource::CS_Erase;    }
			| RDO_NonExist		{ $$ = rdoRuntime::RDOResource::CS_NonExist; };

pat_common_choice:	pat_rel_res
					| pat_rel_res RDO_first {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							PARSER->error( @2, "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
							pattern->setCommonChoiceFirst();
//							RDOFUNArithm* arithm = new RDOFUNArithm( PARSER, 1, @2 );
//							arithm->setSrcPos( @2 );
//							arithm->setSrcText( "first" );
//							pattern->setCommonChoiceWithMax( arithm );
						}
					}
					| pat_rel_res RDO_with_min fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							PARSER->error( @2, "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @2, @3 );
							arithm->setSrcText( "with_min " + arithm->src_text() );
							pattern->setCommonChoiceWithMin( arithm );
						}
					}
					| pat_rel_res RDO_with_max fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getType() == RDOPATPattern::PT_IE ) {
							PARSER->error( @2, "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @2, @3 );
							arithm->setSrcText( "with_max " + arithm->src_text() );
							pattern->setCommonChoiceWithMax( arithm );
						}
					}
					| pat_rel_res RDO_with_min error {
						PARSER->error( @3, "������ � �������������� ���������" )
					}
					| pat_rel_res RDO_with_max error {
						PARSER->error( @3, "������ � �������������� ���������" )
					};

pat_time:	pat_common_choice RDO_Body {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				switch ( pattern->getType() ) {
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : {
						PARSER->error( @2, "����� $Body ��������� �������� ����� $Time" );
						break;
					}
				}
			}
			| pat_common_choice RDO_Time '=' fun_arithm RDO_Body {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				switch ( pattern->getType() ) {
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : break;
					case RDOPATPattern::PT_Rule     : {
						PARSER->error( @2, "���� $Time �� ������������ � ������������� �������" );
//						getParser()->error( "Rule must have no $Time field" );
					}
				}
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($4);
				arithm->setSrcPos( @2, @4 );
				arithm->setSrcText( "$Time = " + arithm->src_text() );
				pattern->setTime( arithm );
			}
			| pat_common_choice RDO_Time '=' fun_arithm error {
				PARSER->error( @4, @5, "��������� �������� ����� $Body" );
			}
			| pat_common_choice RDO_Time '=' error {
				PARSER->error( @4, "������ � �������������� ���������" )
			}
			| pat_common_choice RDO_Time error {
				PARSER->error( @2, @3, "����� ��������� ����� $Time ��������� ���� ���������" )
			}
			| pat_common_choice error {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				switch ( pattern->getType() ) {
					case RDOPATPattern::PT_Rule: {
						PARSER->error( @2, rdo::format("��������� $Body, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						break;
					}
					case RDOPATPattern::PT_IE       :
					case RDOPATPattern::PT_Operation:
					case RDOPATPattern::PT_Keyboard : {
						PARSER->error( @2, rdo::format("��������� $Time, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
						break;
					}
				}
			};

pat_body:	pat_time RDO_IDENTIF {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				std::string    name    = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				pattern->addRelResBody( RDOParserSrcInfo( @2, name ) );
			}
			| pat_convert RDO_IDENTIF {
				RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
				std::string    name    = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				pattern->addRelResBody( RDOParserSrcInfo( @2, name ) );
			}
			| pat_time error {
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				PARSER->error( @2, rdo::format("����������� ����������� ������: %s", str.c_str()) );
			}
			| pat_convert error {
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				PARSER->error( @2, rdo::format("����������� ����������� ������: %s", str.c_str()) );
			};

pat_res_usage:	pat_body pat_choice pat_order {
					RDOPATChoiceFrom* choice_from = reinterpret_cast<RDOPATChoiceFrom*>($2);
					choice_from->setSrcPos( @2 );
					RDOPATChoiceOrder* choice_order = reinterpret_cast<RDOPATChoiceOrder*>($3);
					choice_order->setSrcPos( @3 );
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					pattern->addRelResUsage( choice_from, choice_order );
				};

pat_choice: /* empty */ {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceEmpty;
				$$ = (int) new RDOPATChoiceFrom( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice NoCheck" ), RDOPATChoiceFrom::ch_empty );
			}
			| pat_choice_nocheck {
				$$ = (int) new RDOPATChoiceFrom( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice NoCheck" ), RDOPATChoiceFrom::ch_nocheck );
			}
			| pat_choice_from fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				$$ = (int) new RDOPATChoiceFrom( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "Choice from " + logic->src_text() ), RDOPATChoiceFrom::ch_from, logic );
			}
			| pat_choice_from error {
				PARSER->error( @2, "������ � ���������� ���������" );
			};

pat_choice_nocheck: RDO_Choice RDO_NoCheck {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceNoCheck;
			};

pat_choice_from: RDO_Choice RDO_from {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceFrom;
			};

pat_order:	/* empty */ {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderEmpty;
				$$ = (int) new RDOPATChoiceOrder( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty );
			}
			| pat_choice_first {
				$$ = (int) new RDOPATChoiceOrder( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "first" ), rdoRuntime::RDOSelectResourceCalc::order_first );
			}
			| pat_choice_with_min fun_arithm {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				$$ = (int) new RDOPATChoiceOrder( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "with_min " + arithm->src_text() ), rdoRuntime::RDOSelectResourceCalc::order_with_min, arithm );
			}
			| pat_choice_with_max fun_arithm {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				$$ = (int) new RDOPATChoiceOrder( PARSER->getLastPATPattern()->currRelRes, RDOParserSrcInfo( "with_max " + arithm->src_text() ), rdoRuntime::RDOSelectResourceCalc::order_with_max, arithm );
			}
			| pat_choice_with_min error {
				PARSER->error( @2, "������ � �������������� ���������" )
			}
			| pat_choice_with_max error {
				PARSER->error( @2, "������ � �������������� ���������" )
			};

pat_choice_first: RDO_first {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderFirst;
			};

pat_choice_with_min: RDO_with_min {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMin;
			};

pat_choice_with_max: RDO_with_max {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMax;
			};

pat_convert:	pat_res_usage {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					RDORelevantResource* rel_res = pattern->currRelRes;
					std::string str;
					if ( rel_res->choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
						str = "����� ����� ��������� ����� " + rel_res->choice_order->asString();
					} else if ( rel_res->choice_from->type != RDOPATChoiceFrom::ch_empty ) {
						str = "����� ����� ������� ������";
					} else {
						str = "����� ����� �����";
					}
					if ( rel_res->begin != rdoRuntime::RDOResource::CS_NoChange && rel_res->begin != rdoRuntime::RDOResource::CS_Erase && rel_res->begin != rdoRuntime::RDOResource::CS_NonExist ) {
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : PARSER->error( @1, rdo::format("%s ��������� �������� ����� Convert_event ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s", str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
							case RDOPATPattern::PT_Rule     : PARSER->error( @1, rdo::format("%s ��������� �������� ����� Convert_rule ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s", str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : PARSER->error( @1, rdo::format("%s ��������� �������� ����� Convert_begin ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s", str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
						}
//						PARSER->error( "Converter needed for \"" + *rel_res->name() + "\" relevant resource in pattern \"" + getName() + "\"" );
					}
					if ( rel_res->end != rdoRuntime::RDOResource::CS_NoChange && rel_res->end != rdoRuntime::RDOResource::CS_Erase && rel_res->end != rdoRuntime::RDOResource::CS_NonExist ) {
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : PARSER->error( @1, "���������� ������" ); break;
							case RDOPATPattern::PT_Rule     : PARSER->error( @1, "���������� ������" ); break;
							case RDOPATPattern::PT_Operation:
							case RDOPATPattern::PT_Keyboard : PARSER->error( @1, rdo::format("%s ��������� �������� ����� Convert_end ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s", str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->begin).c_str(), reinterpret_cast<RDOLexer*>(lexer)->YYText()) ); break;
						}
					}
				}
				| pat_res_usage convert_begin pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						PARSER->error( @2, rdo::format("�������� ����� Convert_begin ����� ���� ������������ � ������������ ��� ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->name().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					static_cast<RDOPatternOperation*>(pattern)->addRelResConvertBeginEnd( $3 != 0, par_set, false, NULL, @2, @2, @3, @3 );
				}
				| pat_res_usage pat_params_set convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						PARSER->error( @3, rdo::format("�������� ����� Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->name().c_str()) );
					}
					pattern->currRelRes->deleteParamSetBegin();
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($5);
					par_set->setSrcPos( @5 );
					static_cast<RDOPatternOperation*>(pattern)->addRelResConvertBeginEnd( false, NULL, $4 != 0, par_set, @3, @3, @4, @4 );
				}
				| pat_res_usage convert_begin pat_trace pat_params_set convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Operation && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						PARSER->error( @2, rdo::format("�������� ����� Convert_begin � Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->name().c_str()) );
					}
					RDOPATParamSet* par_set_begin = reinterpret_cast<RDOPATParamSet*>($4);
					par_set_begin->setSrcPos( @4 );
					RDOPATParamSet* par_set_end = reinterpret_cast<RDOPATParamSet*>($7);
					par_set_end->setSrcPos( @7 );
					static_cast<RDOPatternOperation*>(pattern)->addRelResConvertBeginEnd( $3 != 0, par_set_begin, $6 != 0, par_set_end, @2, @5, @3, @6 );
				}
				| pat_res_usage convert_rule pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_Rule ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Operation: type = "��������"; break;
							case RDOPATPattern::PT_Keyboard : type = "������������ ��������"; break;
						}
						PARSER->error( @2, rdo::format("�������� ����� Convert_rule ����� ���� ������������ � ������������� �������, �� �� � %s '%s'", type.c_str(), pattern->name().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					pattern->addRelResConvert( $3 != 0, par_set, @2, @3 );
				}
				| pat_res_usage convert_event pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getType() != RDOPATPattern::PT_IE ) {
						std::string type = "";
						switch ( pattern->getType() ) {
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
							case RDOPATPattern::PT_Operation: type = "��������"; break;
							case RDOPATPattern::PT_Keyboard : type = "������������ ��������"; break;
						}
						PARSER->error( @2, rdo::format("�������� ����� Convert_event ����� ���� ������������ � ������������ �������, �� �� � %s '%s'", type.c_str(), pattern->name().c_str()) );
					}
					RDOPATParamSet* par_set = reinterpret_cast<RDOPATParamSet*>($4);
					par_set->setSrcPos( @4 );
					pattern->addRelResConvert( $3 != 0, par_set, @2, @3 );
				};

convert_rule:	RDO_Convert_rule {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_event:	RDO_Convert_event {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_begin:	RDO_Convert_begin {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_end:	RDO_Convert_end {
				PARSER->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertEnd;
			};

pat_params_set:	/* empty */	{
					RDOPATParamSet* par_set = PARSER->getLastPATPattern()->currRelRes->createParamSet();
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					par_set->setSrcPos( pos );
					$$ = (int)par_set;
				}
				|	pat_params_set RDO_IDENTIF_set fun_arithm {
					RDOPATParamSet* param_set  = reinterpret_cast<RDOPATParamSet*>($1);
					RDOFUNArithm*   arithm     = reinterpret_cast<RDOFUNArithm*>($3);
					std::string     param_name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					YYLTYPE param_name_pos = @2;
					param_name_pos.last_line   = param_name_pos.first_line;
					param_name_pos.last_column = param_name_pos.first_column + param_name.length();
					param_set->addSet( param_name, param_name_pos, arithm );
				}
				| pat_params_set RDO_IDENTIF_set error {
					PARSER->error( @3, "������ � �������������� ���������" )
				}
				|	pat_params_set RDO_IDENTIF_NoChange {
					RDOPATParamSet* param_set  = reinterpret_cast<RDOPATParamSet*>($1);
					std::string     param_name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					YYLTYPE param_name_pos = @2;
					param_name_pos.last_line   = param_name_pos.first_line;
					param_name_pos.last_column = param_name_pos.first_column + param_name.length();
					param_set->addSet( param_name, param_name_pos );
				};

pat_pattern:	pat_convert RDO_End {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					pattern->end();
				};

// ----------------------------------------------------------------------------
// ---------- �������� ���� ���������
// ----------------------------------------------------------------------------
param_type:		RDO_integer param_int_diap param_int_default_val {
					RDORTPIntDiap*    diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPDefVal*       dv = reinterpret_cast<RDORTPDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| RDO_real param_real_diap param_real_default_val {
					RDORTPRealDiap*    diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPDefVal*        dv = reinterpret_cast<RDORTPDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( PARSER->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 0;
					RDORTPEnum*  enu = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPDefVal* dv = reinterpret_cast<RDORTPDefVal*>($2);
					if ( dv->isExist() )
					{
						enu->findEnumValueWithThrow( dv->value().src_pos(), dv->value().value().getAsString() ); // ���� �� �������, �� ����� ��������� �� ������, �.�. throw
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( PARSER->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as {
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info );
				}
				| param_such_as '=' RDO_INT_CONST {
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' RDO_REAL_CONST {
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' RDO_IDENTIF {
					const RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->name() + "." : "") + param->name() );
					$$ = (int)param->getType()->constructorSuchAs( src_info, *reinterpret_cast<RDOValue*>($3) );
				}
				| param_such_as '=' error {
					PARSER->error( "��������� ������� ��-���������" );
				}
				| param_such_as error {
					PARSER->error( "��������� ��������� �������� ���������-������, ��������, ������� ��-���������" );
				};
/*
				| RDO_integer error {
					PARSER->error( @2, "������ ����� ��������� ����� integer. ��������, �� ������� �������� ��-���������." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| RDO_real error {
					PARSER->error( @2, "������ ����� ��������� ����� real. ��������, �� ������� �������� ��-���������." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					PARSER->error( @2, "������ ����� ������������� ����. ��������, �� ������� �������� ��-���������." );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};
*/
param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, pos );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( PARSER, reinterpret_cast<RDOValue*>($2)->value().getInt(), reinterpret_cast<RDOValue*>($4)->value().getInt(), RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST {
					PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST {
					PARSER->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_INT_RANGE_REAL );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' error {
					PARSER->error( @2, "�������� ����� �������" );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_line   = pos.last_line;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, pos );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']' {
					double min = reinterpret_cast<RDOValue*>($2)->value().getDouble();
					double max = reinterpret_cast<RDOValue*>($4)->value().getDouble();
					RDORTPRealDiap* diap = new RDORTPRealDiap( PARSER, min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_REAL_CONST RDO_dblpoint error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' RDO_INT_CONST RDO_dblpoint error {
					PARSER->error( @4, "�������� ����� �������" );
				}
				| '[' error {
					PARSER->error( @2, "�������� ����� �������" );
//					PARSER->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_RANGE );
				};

param_int_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2) );
					}
					| '=' RDO_REAL_CONST {
						// ����� ����� ���������������� ������������: %f
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, reinterpret_cast<RDOValue*>($2)->value().getDouble() );
					}
					| '=' {
						PARSER->error( @1, "�� ������� �������� ��-��������� ��� ������ ����" );
					}
					| '=' error {
						PARSER->error( @2, "�������� �������� ��-��������� ��� ������ ����" );
					};

param_real_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_REAL_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' RDO_INT_CONST {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' {
						PARSER->error( @1, "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						PARSER->error( @2, "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->getEnums().asString() );
				$$ = $2;
			}
			| '(' param_enum_list {
				PARSER->error( @2, "������������ ������ ������������� �������" );
			};

param_enum_list: RDO_IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( PARSER->getLastParsingObject(), *reinterpret_cast<RDOValue*>($1) );
					enu->setSrcInfo( reinterpret_cast<RDOValue*>($1)->src_info() );
					reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' RDO_IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						enu->add( *reinterpret_cast<RDOValue*>($3) );
						$$ = (int)enu;
					} else {
						PARSER->error( @3, "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list RDO_IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->m_enum_param_cnt >= 1 ) {
						PARSER->error( @1, rdo::format("��������� ������� �����: %s", reinterpret_cast<RDOValue*>($2)->value().getIdentificator().c_str()) );
					} else {
						PARSER->error( @2, "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						PARSER->error( @1, "������ � �������� �������� ������������� ����" );
					} else {
						PARSER->error( @2, rdo::format( "�������� �������� ������������� ����: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' RDO_INT_CONST {
					PARSER->error( @3, "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| param_enum_list ',' RDO_REAL_CONST {
					PARSER->error( @3, "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| RDO_INT_CONST {
					PARSER->error( @1, "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| RDO_REAL_CONST {
					PARSER->error( @1, "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| error {
					PARSER->error( @1, "������ � �������� �������� ������������� ����" );
				};

param_enum_default_val:	/* empty */ {
						$$ = (int)new RDORTPDefVal(PARSER);
					}
					| '=' RDO_IDENTIF {
						$$ = (int)new RDORTPDefVal(PARSER, *reinterpret_cast<RDOValue*>($2));
					}
					| '=' {
						PARSER->error( @1, "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						PARSER->error( @2, "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_such_as:	RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF {
					std::string type  = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					std::string param = reinterpret_cast<RDOValue*>($4)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						PARSER->error( @4, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| RDO_such_as RDO_IDENTIF {
					std::string constName = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDOFUNConstant* const cons = PARSER->findFUNConstant( constName );
					if ( !cons ) {
						PARSER->error( @2, rdo::format("������ �� �������������� ���������: %s", constName.c_str()) );
					}
					$$ = (int)cons->getDescr();
				}
				| RDO_such_as RDO_IDENTIF '.' {
					std::string type = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @3, "�� ������ ���������" );
					}
				}
				| RDO_such_as RDO_IDENTIF '.' error {
					std::string type = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @4, "������ ��� �������� ���������" );
					}
				}
				| RDO_such_as RDO_IDENTIF error {
					std::string type = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
					const RDORTPResType* const rt = PARSER->findRTPResType( type );
					if ( !rt ) {
						PARSER->error( @2, rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						PARSER->error( @2, "����� ����� ���� ������ ���� ������ ��������� ����� �����" );
					}
				}
				| RDO_such_as error {
					PARSER->error( @2, "����� ��������� ����� such_as ���������� ������� ��� � ��������� ������� ��� ������" );
				};
// ----------------------------------------------------------------------------

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
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) + *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) - *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) * *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) / *reinterpret_cast<RDOFUNArithm*>($3)); }
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
				$$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1), *reinterpret_cast<RDOValue*>($3) );
			}
			| RDO_INT_CONST               { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_REAL_CONST              { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_IDENTIF                 { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| '-' fun_arithm %prec RDO_UMINUS {
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( PARSER, RDOValue(reinterpret_cast<RDOFUNArithm*>($2)->type(), info), new rdoRuntime::RDOCalcUMinus( RUNTIME, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ) );
			};

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call:	RDO_IDENTIF '(' ')' {
							RDOFUNParams* fun = new RDOFUNParams( PARSER );
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @3 );
							fun->setSrcText( fun_name + "()" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
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
						std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
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
						std::string type_name = reinterpret_cast<RDOValue*>($3)->value().getIdentificator();
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
