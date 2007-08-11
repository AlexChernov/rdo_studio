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
#include "rdopat.h"
#include "rdortp.h"
#include "rdofun.h"
#include <rdotrace.h>

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%%

pat_list:	
			| pat_list pat_pattern;
			| error {
				parser->lexer_loc_set( &(@1) );
				parser->error( "��������� �������� ����� $Pattern" );
			};

pat_header:	  Pattern IDENTIF_COLON operation_kw    pat_trace { $$ = (int)(new RDOPATPatternOperation( parser, *(std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON irregular_event pat_trace { $$ = (int)(new RDOPATPatternEvent(     parser, *(std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON rule_keyword    pat_trace { $$ = (int)(new RDOPATPatternRule(      parser, *(std::string *)$2, $4 != 0 )); }
			| Pattern IDENTIF_COLON keyboard        pat_trace { $$ = (int)(new RDOPATPatternKeyboard(  parser, *(std::string *)$2, $4 != 0 )); };
			| Pattern error {
				parser->lexer_loc_set( &(@2) );
				parser->error( "��������� ��� �������" );
			}
			| Pattern IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				parser->error( "��������� ��� �������" );
//			}
//			| Pattern IDENTIF_COLON irregular_event error {
//				parser->lexer_loc_set( &(@3), &(@4) );
//				parser->error( "��������� ������� �����������" );
//				parser->error( "��������� ������� �����������, �������� ���������� ��� ����������� �������� �������" );
			};

pat_trace:	/* empty */		{ $$ = 0; }
			| trace_keyword	{ $$ = 1; }
			| no_trace		{ $$ = 0; };

pat_params_begin: pat_header Parameters { $$ = $1; };

pat_params:	pat_params_begin IDENTIF_COLON param_type {
				((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam(*(std::string *)$2, (RDORTPParamType *)$3));
				if ( reinterpret_cast<RDORTPParamType*>($3)->getType() == RDORTPParamType::pt_enum ) {
					reinterpret_cast<RDORTPEnumParamType*>($3)->enum_name = rdo::format( "%s.%s", ((RDOPATPattern*)$1)->getName().c_str(), ((std::string*)$2)->c_str() );
				}
				$$ = $1;
			}
			| pat_params IDENTIF_COLON param_type {
				((RDOPATPattern *)$1)->add(new RDOFUNFunctionParam(*(std::string *)$2, (RDORTPParamType *)$3));
				if ( reinterpret_cast<RDORTPParamType*>($3)->getType() == RDORTPParamType::pt_enum ) {
					reinterpret_cast<RDORTPEnumParamType*>($3)->enum_name = rdo::format( "%s.%s", ((RDOPATPattern*)$1)->getName().c_str(), ((std::string*)$2)->c_str() );
				}
				$$ = $1;
			}
			| pat_params_begin error {
				parser->lexer_loc_set( &(@1), &(@2) );
				if ( @1.last_line != @2.last_line ) {
					parser->error( "��������� ��� ��������� �������" );
				} else {
					parser->error( rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin IDENTIF error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "��������� ���������" );
				} else {
					parser->error( rdo::format("��������� ���������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params_begin IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "��������� ��� ��������� �������" );
				} else {
					parser->error( rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params error {
				parser->lexer_loc_set( &(@1), &(@2) );
				if ( @1.last_line != @2.last_line ) {
					parser->error( "��������� ��� ��������� �������" );
				} else {
					parser->error( rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params IDENTIF error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "��������� ���������" );
				} else {
					parser->error( rdo::format("��������� ���������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			}
			| pat_params IDENTIF_COLON error {
				parser->lexer_loc_set( &(@2), &(@3) );
				if ( @2.last_line != @3.last_line ) {
					parser->error( "��������� ��� ��������� �������" );
				} else {
					parser->error( rdo::format("��������� ��� ��������� �������, �������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				}
			};

pat_params_end:	pat_params Relevant_resources   { $$ = $1; }
				| pat_header Relevant_resources { $$ = $1; }
				| pat_header error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "��������� �������� ����� $Relevant_resources" );
				};

pat_rel_res:	pat_params_end IDENTIF_COLON IDENTIF pat_conv pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, (rdoRuntime::RDOResourceTrace::ConvertStatus)$5, @2, @3, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, (rdoRuntime::RDOResourceTrace::ConvertStatus)$5, @2, @3, @4, @5 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr((rdoRuntime::RDOResourceTrace::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							pattern->addRelRes( rel_name, type_name, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @2, @3, @4 );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr((rdoRuntime::RDOResourceTrace::ConvertStatus)$4).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							pattern->addRelRes( rel_name, type_name, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @2, @3, @4 );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @2, type_pos, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange pat_conv {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, (rdoRuntime::RDOResourceTrace::ConvertStatus)$4, @2, type_pos, convertor_pos, @4 );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
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
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, rdoRuntime::RDOResourceTrace::CS_NoChange, @2, type_pos, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
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
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							YYLTYPE convertor_end_pos = @3;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, rdoRuntime::RDOResourceTrace::CS_NoChange, @2, type_pos, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							pattern->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, @2, type_pos, convertor_pos );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3) );
							parser->error( rdo::format("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����", RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).c_str()) );
							break;
						}
						case RDOPATPattern::PT_IE  : 
						case RDOPATPattern::PT_Rule: {
							std::string rel_name  = *reinterpret_cast<std::string*>($2);
							std::string type_name = *reinterpret_cast<std::string*>($3);
							YYLTYPE type_pos = @3;
							type_pos.last_line   = type_pos.first_line;
							type_pos.last_column = type_pos.first_column + type_name.length();
							YYLTYPE convertor_pos = @3;
							convertor_pos.first_line   = convertor_pos.last_line;
							convertor_pos.first_column = convertor_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							pattern->addRelRes( rel_name, type_name, rdoRuntime::RDOResourceTrace::CS_NoChange, @2, type_pos, convertor_pos );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end IDENTIF_COLON IDENTIF IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string  rel_name      = *reinterpret_cast<std::string*>($2);
							std::string  type_name     = *reinterpret_cast<std::string*>($3);
							std::string* convert_begin = reinterpret_cast<std::string*>($4);
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin->length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, pattern->StrToStatus( *convert_begin, convertor_begin_pos ), rdoRuntime::RDOResourceTrace::CS_NoChange, @2, @3, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_rel_res IDENTIF_COLON IDENTIF IDENTIF_NoChange {
					// ��������� ��� ie,rule,opr,key
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					switch ( pattern->getPatType() ) {
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							std::string  rel_name      = *reinterpret_cast<std::string*>($2);
							std::string  type_name     = *reinterpret_cast<std::string*>($3);
							std::string* convert_begin = reinterpret_cast<std::string*>($4);
							YYLTYPE convertor_begin_pos = @4;
							convertor_begin_pos.last_line   = convertor_begin_pos.first_line;
							convertor_begin_pos.last_column = convertor_begin_pos.first_column + convert_begin->length();
							YYLTYPE convertor_end_pos = @4;
							convertor_end_pos.first_line   = convertor_end_pos.last_line;
							convertor_end_pos.first_column = convertor_end_pos.last_column - RDOPATPattern::StatusToStr(rdoRuntime::RDOResourceTrace::CS_NoChange).length();
							static_cast<RDOPATPatternOperation*>(pattern)->addRelRes( rel_name, type_name, pattern->StrToStatus( *convert_begin, convertor_begin_pos ), rdoRuntime::RDOResourceTrace::CS_NoChange, @2, @3, convertor_begin_pos, convertor_end_pos );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� ������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( rdo::format("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������") );
							break;
						}
					}
					$$ = $1;
				}
				| pat_params_end error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ � �������� ����������� ��������" );
				}
				| pat_rel_res error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ � �������� ����������� ��������" );
				}
				| pat_params_end IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "��������� ��������� (��� ���� ��� �������)" );
				}
				| pat_rel_res IDENTIF_COLON error {
					parser->lexer_loc_set( &(@2), &(@3) );
					parser->error( "��������� ��������� (��� ���� ��� �������)" );
				}
				| pat_params_end IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "��������� ������ ���������� ������" );
					} else {
						parser->error( "��������� ������ ����������" );
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF error {
					parser->lexer_loc_set( &(@3), &(@4) );
					if ( parser->getLastPATPattern()->isHaveConvertEnd() ) {
						parser->error( "��������� ������ ���������� ������" );
					} else {
						parser->error( "��������� ������ ����������" );
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4), &(@5) );
							parser->error( "��������� ������ ���������� �����" );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF pat_conv error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@5) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@4), &(@5) );
							parser->error( "��������� ������ ���������� �����" );
							break;
						}
					}
				}
				| pat_params_end IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "��������� ������ ���������� �����" );
							break;
						}
					}
				}
				| pat_rel_res IDENTIF_COLON IDENTIF_NoChange error {
					switch ( parser->getLastPATPattern()->getPatType() ) {
						case RDOPATPattern::PT_Rule: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Body" );
							break;
						}
						case RDOPATPattern::PT_IE: {
							parser->lexer_loc_set( &(@4) );
							parser->error( "��������� ������ ������ (first/with_min/with_max) ��� $Time" );
							break;
						}
						case RDOPATPattern::PT_Operation:
						case RDOPATPattern::PT_Keyboard : {
							parser->lexer_loc_set( &(@3), &(@4) );
							parser->error( "��������� ������ ���������� �����" );
							break;
						}
					}
				};

pat_conv:	Keep				{ $$ = rdoRuntime::RDOResourceTrace::CS_Keep;     }
			| Create_keyword	{ $$ = rdoRuntime::RDOResourceTrace::CS_Create;   }
			| Erase				{ $$ = rdoRuntime::RDOResourceTrace::CS_Erase;    }
			| NonExist			{ $$ = rdoRuntime::RDOResourceTrace::CS_NonExist; };

pat_common_choice:	pat_rel_res
					| pat_rel_res first_keyword {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getPatType() == RDOPATPattern::PT_IE ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
//							((RDOPATPattern *)$1)->setCommonChoiceFirst(); $$ = $1;
							RDOFUNArithm* arithm = new RDOFUNArithm( parser, 1, @2 );
							pattern->setCommonChoiceWithMax( arithm );
						}
						$$ = $1;
					}
					| pat_rel_res with_min fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getPatType() == RDOPATPattern::PT_IE ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @3 );
							pattern->setCommonChoiceWithMin( arithm );
						}
						$$ = $1;
					}
					| pat_rel_res with_max fun_arithm {
						RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
						if ( pattern->getPatType() == RDOPATPattern::PT_IE ) {
							parser->lexer_loc_set( &(@2) );
							parser->error( "� ������������ ������� �� ������������ ������ ������ ����������� ��������" );
						} else {
							RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
							arithm->setSrcPos( @3 );
							pattern->setCommonChoiceWithMax( arithm );
						}
						$$ = $1;
					}
					| pat_rel_res with_min error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� �������������� ���������" );
					}
					| pat_rel_res with_max error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� �������������� ���������" );
					};

pat_time:	pat_common_choice Body {
				$$ = $1;
				if ( ((RDOPATPattern *)$1)->needTime() ) {
					parser->lexer_loc_set( @2.first_line, @2.first_column );
					parser->error( "����� $Body ��������� �������� ����� $Time" );
				}
			}
			| pat_common_choice Time '=' fun_arithm Body {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@4) );
				((RDOPATPattern *)$1)->setTime((RDOFUNArithm *)$4);
				parser->lexer_loc_restore();
				$$ = $1;
			};

pat_body:	pat_time IDENTIF {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@2) );
				((RDOPATPattern *)$1)->addRelResBody(*(std::string *)$2);
				parser->lexer_loc_restore();
				$$ = $1;
			}
			| pat_convert IDENTIF {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( &(@2) );
				((RDOPATPattern *)$1)->addRelResBody(*(std::string *)$2);
				parser->lexer_loc_restore();
				$$ = $1;
			}
			| pat_time error {
				parser->lexer_loc_set( &(@2) );
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( rdo::format("����������� ����������� ������: %s", str.c_str()) );
			}
			| pat_convert error {
				parser->lexer_loc_set( &(@2) );
				std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
				parser->error( rdo::format("����������� ����������� ������: %s", str.c_str()) );
			};

pat_res_usage:	pat_body pat_choice pat_first {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( @3.last_line, @3.last_column );
					RDOPATChoiceFrom* choice_from = reinterpret_cast<RDOPATChoiceFrom*>($2);
					choice_from->setSrcPos( @2 );
					choice_from->setSrcText( choice_from->asString() );
					if ( choice_from->logic ) {
						choice_from->setSrcText( choice_from->src_text() + " " + choice_from->logic->src_text() );					
					}
					RDOPATChoiceOrder* choice_order = reinterpret_cast<RDOPATChoiceOrder*>($3);
					choice_order->setSrcPos( @3 );
					choice_order->setSrcText( choice_order->asString() );
					if ( choice_order->arithm ) {
						choice_order->setSrcText( choice_order->src_text() + " " + choice_order->arithm->src_text() );					
					}
					((RDOPATPattern *)$1)->addRelResUsage( choice_from, choice_order );
					parser->lexer_loc_restore();
					$$ = $1;
				};

pat_choice: /* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceEmpty;
				$$= (int) new RDOPATChoiceFrom( RDOPATChoiceFrom::ch_empty );
			}
			| pat_choice_nocheck {
				$$ = (int) new RDOPATChoiceFrom( RDOPATChoiceFrom::ch_nocheck );
			}
			| pat_choice_from fun_logic {
				$$ = (int) new RDOPATChoiceFrom( RDOPATChoiceFrom::ch_from, (RDOFUNLogic*)$2 );
			};

pat_choice_nocheck: Choice NoCheck {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceNoCheck;
			};

pat_choice_from: Choice from_keyword {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceFrom;
			};

pat_first:	/* empty */ {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderEmpty;
				$$ = (int) new RDOPATChoiceOrder( rdoRuntime::RDOSelectResourceCalc::st_empty );
			}
			| pat_choice_first {
				$$ = (int) new RDOPATChoiceOrder( rdoRuntime::RDOSelectResourceCalc::st_first );
			}
			| pat_choice_with_min fun_arithm {
				$$ = (int) new RDOPATChoiceOrder( rdoRuntime::RDOSelectResourceCalc::st_with_min, (RDOFUNArithm *)$2 );
			}
			| pat_choice_with_max fun_arithm {
				$$ = (int) new RDOPATChoiceOrder( rdoRuntime::RDOSelectResourceCalc::st_with_max, (RDOFUNArithm *)$2 );
			}
			| pat_choice_with_min error {
				parser->lexer_loc_set( &(@1), &(@2) );
				parser->error( "��������� �������������� ���������" );
			}
			| pat_choice_with_max error {
				parser->lexer_loc_set( &(@1), &(@2) );
				parser->error( "��������� �������������� ���������" );
			};

pat_choice_first: first_keyword {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderFirst;
			};

pat_choice_with_min: with_min {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMin;
			};

pat_choice_with_max: with_max {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::choiceOrderWithMax;
			};

pat_convert:	pat_res_usage {
					parser->lexer_loc_backup();
					parser->lexer_loc_set( @1.last_line, @1.last_column );
					((RDOPATPattern *)$1)->addRelResConvert();
					parser->lexer_loc_restore();
					$$ = $1;
				}
				| pat_res_usage convert_begin pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getPatType() != RDOPATPattern::PT_Operation && pattern->getPatType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getPatType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( rdo::format("�������� ����� Convert_begin ����� ���� ������������ � ������������ ��� ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					pattern->addRelResConvert($3 != 0, (RDOPATParamSet *)$4, @2);
					$$ = $1;
				}
				| pat_res_usage convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getPatType() != RDOPATPattern::PT_Operation && pattern->getPatType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getPatType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( rdo::format("�������� ����� Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					static_cast<RDOPATPatternOperation*>(pattern)->addRelResConvertEnd( $3 != 0, (RDOPATParamSet *)$4, @2 );
					$$ = $1;
				}
				| pat_res_usage convert_begin pat_trace pat_params_set convert_end pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getPatType() != RDOPATPattern::PT_Operation && pattern->getPatType() != RDOPATPattern::PT_Keyboard ) {
						std::string type = "";
						switch ( pattern->getPatType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
						}
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( rdo::format("�������� ����� Convert_begin � Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					static_cast<RDOPATPatternOperation*>(pattern)->addRelResConvertBeginEnd($3 != 0, (RDOPATParamSet *)$4, $6 != 0, (RDOPATParamSet *)$7, @2, @5);
					$$ = $1;
				}
				| pat_res_usage convert_rule pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getPatType() != RDOPATPattern::PT_Rule ) {
						std::string type = "";
						switch ( pattern->getPatType() ) {
							case RDOPATPattern::PT_IE       : type = "������������ �������"; break;
							case RDOPATPattern::PT_Operation: type = "��������"; break;
							case RDOPATPattern::PT_Keyboard : type = "������������ ��������"; break;
						}
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( rdo::format("�������� ����� Convert_rule ����� ���� ������������ � ������������� �������, �� �� � %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					pattern->addRelResConvert($3 != 0, (RDOPATParamSet *)$4, @2);
					$$ = $1;
				}
				| pat_res_usage convert_event pat_trace pat_params_set {
					RDOPATPattern* pattern = reinterpret_cast<RDOPATPattern*>($1);
					if ( pattern->getPatType() != RDOPATPattern::PT_IE ) {
						std::string type = "";
						switch ( pattern->getPatType() ) {
							case RDOPATPattern::PT_Rule     : type = "������������� �������"; break;
							case RDOPATPattern::PT_Operation: type = "��������"; break;
							case RDOPATPattern::PT_Keyboard : type = "������������ ��������"; break;
						}
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( rdo::format("�������� ����� Convert_event ����� ���� ������������ � ������������ �������, �� �� � %s '%s'", type.c_str(), pattern->getName().c_str()) );
					}
					pattern->addRelResConvert($3 != 0, (RDOPATParamSet *)$4, @2);
					$$ = $1;
				};

convert_rule:	Convert_rule {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_event:	Convert_event {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_begin:	Convert_begin {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertBegin;
			};

convert_end:	Convert_end {
				parser->getLastPATPattern()->currRelRes->currentState = RDORelevantResource::convertEnd;
			};

pat_params_set:	/* empty */	{
					$$ = (int) new RDOPATParamSet( parser->getLastPATPattern()->currRelRes ); 
				}
				|	pat_params_set IDENTIF_set fun_arithm	{
					RDOPATParamSet* param_set  = reinterpret_cast<RDOPATParamSet*>($1);
					RDOFUNArithm*   arithm     = reinterpret_cast<RDOFUNArithm*>($3);
					std::string*    param_name = reinterpret_cast<std::string*>($2);
					param_set->setSrcText( *param_name + " set " + arithm->src_text() );
					param_set->addSet( *param_name, @2, arithm );
					$$ = $1;
				}
				|	pat_params_set IDENTIF_NoChange			{
					RDOPATParamSet* param_set = reinterpret_cast<RDOPATParamSet*>($1);
					std::string*    param_name = reinterpret_cast<std::string*>($2);
					param_set->setSrcText( *param_name + " NoChange" );
					param_set->addSet( *param_name, @2 );
					$$ = $1;
				};

pat_pattern:	pat_convert End { ((RDOPATPattern *)$1)->end(); $$ = $1; };
//				| pat_time  End { ((RDOPATPattern *)$1)->end(); $$ = $1; };

// ----------------------------------------------------------------------------
// ---------- �������� ���� ���������
// ----------------------------------------------------------------------------
param_type:		integer param_int_diap param_int_default_val {
					RDORTPIntDiap* diap = reinterpret_cast<RDORTPIntDiap*>($2);
					RDORTPIntDefVal* dv = reinterpret_cast<RDORTPIntDefVal*>($3);
					RDORTPIntParamType* rp = new RDORTPIntParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| real param_real_diap param_real_default_val {
					RDORTPRealDiap* diap = reinterpret_cast<RDORTPRealDiap*>($2);
					RDORTPRealDefVal* dv = reinterpret_cast<RDORTPRealDefVal*>($3);
					RDORTPRealParamType* rp = new RDORTPRealParamType( parser->getLastParsingObject(), diap, dv, RDOParserSrcInfo( @1, @3 ) );
					$$ = (int)rp;
				}
				| param_enum param_enum_default_val {
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 0;
					RDORTPEnum* enu      = reinterpret_cast<RDORTPEnum*>($1);
					RDORTPEnumDefVal* dv = reinterpret_cast<RDORTPEnumDefVal*>($2);
					if ( dv->isExist() ) {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( dv->src_pos().last_line, dv->src_pos().last_pos );
						enu->findValue( dv->getEnumValue() ); // ���� �� �������, �� ����� ��������� �� ������, �.�. throw
						parser->lexer_loc_restore();
					}
					RDORTPEnumParamType* rp = new RDORTPEnumParamType( parser->getLastParsingObject(), enu, dv, RDOParserSrcInfo( @1, @2 ) );
					$$ = (int)rp;
				}
				| param_such_as {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( src_info );
				}
				| param_such_as '=' INT_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( (int)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' REAL_CONST {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(double*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' IDENTIF {
					RDORTPParam* param = reinterpret_cast<RDORTPParam*>($1);
					RDOParserSrcInfo src_info( @1, @3 );
					src_info.setSrcText( "such_as " + (param->getResType() ? param->getResType()->getName() + "." : "") + param->getName() );
					$$ = (int)param->getType()->constructSuchAs( *(std::string*)$3, src_info, RDOParserSrcInfo( @3 ) );
				}
				| param_such_as '=' error {
					parser->error( "��������� ������� ��-���������" );
				}
				| param_such_as error {
					parser->error( "��������� ��������� �������� ���������-������, ��������, ������� ��-���������" );
				}
				| integer error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� integer. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_INT_PARAM_END );
				}
				| real error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ��������� ����� real. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_REAL_PARAM_END );
				}
				| param_enum error {
					parser->lexer_loc_set( &(@2) );
					parser->error( "������ ����� ������������� ����. ��������, �� ������� �������� ��-���������." );
//					parser->error( rdoSimulator::RDOSyntaxError::RTP_WAITING_FOR_ENUM_PARAM_END );
				};

param_int_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_column = pos.last_column;
					RDORTPIntDiap* diap = new RDORTPIntDiap( pos );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					RDORTPIntDiap* diap = new RDORTPIntDiap( $2, $4, RDOParserSrcInfo( @1, @5 ), @4 );
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

param_real_diap:	/* empty */ {
					YYLTYPE pos = @0;
					pos.first_column = pos.last_column;
					RDORTPRealDiap* diap = new RDORTPRealDiap( pos );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint REAL_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' REAL_CONST dblpoint INT_CONST ']' {
					double min = *reinterpret_cast<double*>($2);
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint REAL_CONST ']' {
					double min = $2;
					double max = *reinterpret_cast<double*>($4);
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
					$$ = (int)diap;
				}
				| '[' INT_CONST dblpoint INT_CONST ']' {
					double min = $2;
					double max = $4;
					RDORTPRealDiap* diap = new RDORTPRealDiap( min, max, RDOParserSrcInfo( @1, @5 ), @4 );
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

param_int_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPIntDefVal(pos));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPIntDefVal($2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' REAL_CONST {
						// ����� ����� ���������������� ������������: %f
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_AS_REAL, *(double*)$2 );
					}
					| '=' {
						parser->lexer_loc_set( @1.last_line, @1.last_column );
						parser->error( "�� ������� �������� ��-��������� ��� ������ ����" );
					}
					| '=' error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "�������� �������� ��-��������� ��� ������ ����" );
					};

param_real_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPRealDefVal(pos));
					}
					| '=' REAL_CONST {
						$$ = (int)(new RDORTPRealDefVal(*((double *)$2), RDOParserSrcInfo( @1, @2 )));
					}
					| '=' INT_CONST {
						$$ = (int)(new RDORTPRealDefVal($2, RDOParserSrcInfo( @1, @2 )));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_enum:	'(' param_enum_list ')' {
				RDORTPEnum* enu = reinterpret_cast<RDORTPEnum*>($2);
				enu->setSrcPos( @1, @3 );
				enu->setSrcText( enu->src_text() + ")" );
				$$ = $2;
			}
			| '(' param_enum_list {
				parser->lexer_loc_set( &(@2) );
				parser->error( "������������ ������ ������������� �������" );
			};

param_enum_list: IDENTIF {
					RDORTPEnum* enu = new RDORTPEnum( parser->getLastParsingObject(), *(std::string *)$1 );
					std::string* first = reinterpret_cast<std::string*>($1);
					enu->setSrcText( "(" + *first );
					reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt = 1;
					$$ = (int)enu;
				}
				| param_enum_list ',' IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						RDORTPEnum* enu  = reinterpret_cast<RDORTPEnum*>($1);
						std::string next = *reinterpret_cast<std::string*>($3);
						enu->add( next, @3 );
						enu->setSrcText( enu->src_text() + ", " + next );
						$$ = (int)enu;
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list IDENTIF {
					if ( reinterpret_cast<RDOLexer*>(lexer)->enum_param_cnt >= 1 ) {
						parser->error( rdo::format("��������� ������� �����: %s", ((std::string*)$2)->c_str()) );
					} else {
						parser->error( "������ � �������� �������� ������������� ����" );
					}
				}
				| param_enum_list error {
					std::string str( reinterpret_cast<RDOLexer*>(lexer)->YYText() );
					if ( str.empty() ) {
						parser->lexer_loc_set( &(@1) );
						parser->error( "������ � �������� �������� ������������� ����" );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format( "�������� �������� ������������� ����: %s", str.c_str() ) );
					}
				}
				| param_enum_list ',' INT_CONST {
					parser->lexer_loc_set( &(@3) );
					parser->error( "�������� ������������� ���� �� ����� ���������� � �����" );
				}
				| param_enum_list ',' REAL_CONST {
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

param_enum_default_val:	/* empty */ {
						YYLTYPE pos = @0;
						pos.first_column = pos.last_column;
						$$ = (int)(new RDORTPEnumDefVal(pos));
					}
					| '=' IDENTIF {
						$$ = (int)(new RDORTPEnumDefVal( *(std::string*)$2, RDOParserSrcInfo( @1, @2 ) ));
					}
					| '=' {
						parser->lexer_loc_set( &(@1) );
						parser->error( "�� ������� �������� ��-��������� ��� ������������� ����" );
					}
					| '=' error {
						parser->lexer_loc_set( &(@2) );
						parser->error( "�������� �������� ��-��������� ��� ������������� ����" );
					};

param_such_as:	such_as IDENTIF '.' IDENTIF {
					std::string type  = *reinterpret_cast<std::string*>($2);
					std::string param = *reinterpret_cast<std::string*>($4);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					}
					const RDORTPParam* const rp = rt->findRTPParam( param );
					if ( !rp ) {
						parser->lexer_loc_set( &(@4) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_PARAM, type.c_str(), param.c_str() );
					}
					$$ = (int)rp;
				}
				| such_as IDENTIF {
					std::string constName = *reinterpret_cast<std::string*>($2);
					const RDOFUNConstant* const cons = parser->findFUNConst( constName );
					if ( !cons ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdo::format("������ �� �������������� ���������: %s", constName.c_str()) );
					}
					$$ = (int)(cons->getDescr());
				}
				| such_as IDENTIF '.' {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@3) );
						parser->error( "�� ������ ���������" );
					}
				}
				| such_as IDENTIF '.' error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@4) );
						parser->error( "������ ��� �������� ���������" );
					}
				}
				| such_as IDENTIF error {
					std::string type = *reinterpret_cast<std::string*>($2);
					const RDORTPResType* const rt = parser->findRTPResType( type );
					if ( !rt ) {
						parser->lexer_loc_set( &(@2) );
						parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_SUCHAS_RES_TYPE, type.c_str() );
					} else {
						parser->lexer_loc_set( &(@2) );
						parser->error( "����� ����� ���� ������ ���� ������ ��������� ����� �����" );
					}
				}
				| such_as error {
					if ( @1.last_line == @2.first_line ) {
						parser->lexer_loc_set( @2.first_line, @2.first_column );
					} else {
						parser->lexer_loc_set( &(@1) );
					}
					parser->error( "����� ��������� ����� such_as ���������� ������� ��� � ��������� ������� ��� ������" );
				};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
fun_logic: fun_arithm '=' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm			{ $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic	{ $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| not_keyword fun_logic				{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group							{
				$$ = $1;
			}
			| fun_select_logic					{
				$$ = $1;
			}
			| '[' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "��������� ������������� ������" );
			}
			| '(' fun_logic error {
				parser->lexer_loc_set( @2.last_line, @2.last_column );
				parser->error( "��������� ������������� ������" );
			}
			| error								{
				parser->lexer_loc_set( &(@1) );
				parser->error( "������ � ���������� ���������" );
			};

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')'			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call			{
				$$ = $1;
			}
			| fun_select_arithm				{
				$$ = $1;
			}
			| IDENTIF '.' IDENTIF			{
				$$ = (int)new RDOFUNArithm( parser, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( parser, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( parser, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( parser, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec UMINUS	{
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runTime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			}
			| error							{
				parser->lexer_loc_set( &(@1) );
				if ( @1.first_line = @1.last_line ) {
					parser->error( rdo::format("����������� �������������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				} else {
					parser->error( "������ � �������������� ���������" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($3);
							fun->name_error_pos.setSrcPos( @1 );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( *(std::string*)$1 + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( *(std::string*)$1 );
							$$ = (int)arithm;
						}
						| IDENTIF '(' error {
							parser->lexer_loc_set( &(@3) );
							parser->error( "������ � ���������� �������" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
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
							};

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						parser->lexer_loc_backup();
						parser->lexer_loc_set( @3.first_line, @3.first_column + ((std::string*)$3)->length() );
						$$ = (int)(new RDOFUNGroupLogic( parser, $1, *(std::string *)$3) );
						parser->lexer_loc_restore();
					}
					| fun_group_keyword '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ��� ����" );
					}
					| fun_group_keyword error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ������������� ������" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic((RDOFUNLogic *)$2);
					}
					| fun_group_header NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst( parser->runTime, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	Select '(' IDENTIF_COLON {
						RDOFUNSelect* select = new RDOFUNSelect(parser, *(std::string*)$3);
						select->setSrcText( "Select(" + *(std::string*)$3 + ": " );
						$$ = (int)select;
					}
					| Select '(' error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ��� ����" );
					}
					| Select error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� ������������� ������" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						RDOFUNLogic* logic = select->createFunSelect( flogic );
						logic->setSrcPos( @2 );
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcText( select->src_text() + "NoCheck)" );
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect();
						logic->setSrcPos( @2 );
						$$ = $1;
					}
					| fun_select_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, (RDOFUNLogic*)$5 );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOFUNLogic* logic = select->createFunSelectEmpty();
						$$ = (int)logic;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� '.' (�����) ��� ������ ������ ������ ��������" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� ����� ������ ��������" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->lexer_loc_set( @5.first_line, @5.first_column );
						parser->error( "������ � ���������� ���������" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "��������� ������������� ������" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOFUNArithm* arithm = select->createFunSelectSize();
						$$ = (int)arithm;
					}
					| fun_select_body error {
						parser->lexer_loc_set( &(@1) );
						parser->error( "��������� '.' (�����) ��� ������ ������ ������ ��������" );
					}
					| fun_select_body '.' error {
						parser->lexer_loc_set( &(@2), &(@3) );
						parser->error( "��������� ����� ������ ��������: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->lexer_loc_set( &(@3) );
						parser->error( "��������� ������������� ������" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->lexer_loc_set( &(@4) );
						parser->error( "��������� ������������� ������" );
					};

%%

}
