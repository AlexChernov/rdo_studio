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
%token if_keyword			369
%token result_keyword		370

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token bitmap				406
%token s_bmp				407
%token rect_keyword			408
%token r_rect				409
%token line					410
%token ellipse				411
%token triang				412
%token active				413
%token ruler				414
%token space_kw				415
%token color_transparent_kw	416
%token color_last_kw		417
%token color_white_kw		418
%token color_black_kw		419
%token color_red_kw			420
%token color_green_kw		421
%token color_blue_kw		422
%token color_cyan_kw		423
%token color_magenta_kw		424
%token color_yellow_kw		425
%token color_gray_kw		426

%token QUOTED_IDENTIF		430
%token QUOTED_IDENTIF_BAD	431
%token IDENTIF_BAD			432
%token Select				433
%token Size_kw				434
%token Empty_kw				435
%token not_keyword			436
%token UMINUS				437

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoopr.h"
#include "rdopat.h"
#include "rdoparser.h"

namespace rdoParse 
{
%}

%%

opr_main:	opr_end
			| error {
				if ( !parser->isHaveKWOperations() ) {
					parser->error( @1, "��������� �������� ����� $Operations" );
				} else if ( parser->isHaveKWOperationsEnd() ) {
					parser->error( @1, "�������� ��� ����������" );
				} else {
					parser->error( @1, rdoSimulator::RDOSyntaxError::UNKNOWN );
				}
			};

opr_header:	Operations {
				parser->setHaveKWOperations( true );
			};

opr_body:	opr_header IDENTIF_COLON IDENTIF {
				std::string name = *reinterpret_cast<std::string*>($2);
				RDOParserSrcInfo pattern( @3, *reinterpret_cast<std::string*>($3) );
				RDOOPROperation* opr = new RDOOPROperation( parser, RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext), pattern );
				$$ = (int)opr;
			}
			| opr_param IDENTIF_COLON IDENTIF {
				RDOOPROperation* opr = reinterpret_cast<RDOOPROperation*>($1);
				opr->endParam( @1 );
				std::string name = *reinterpret_cast<std::string*>($2);
				RDOParserSrcInfo pattern( @3, *reinterpret_cast<std::string*>($3) );
				opr = new RDOOPROperation( parser, RDOParserSrcInfo(@2, name, RDOParserSrcInfo::psi_align_bytext), pattern );
				$$ = (int)opr;
			}
			| opr_header IDENTIF_COLON error {
				parser->error( @2, @3, "��������� ��� �������" );
			}
			| opr_param IDENTIF_COLON error {
				parser->error( @2, @3, "��������� ��� �������" );
			}
			| opr_header error {
				parser->error( @2, "��������� ��� ��������" );
			};

opr_keyb:	opr_body
			| opr_keyb QUOTED_IDENTIF {
				RDOOPROperation* opr = reinterpret_cast<RDOOPROperation*>($1);
				std::string      key = *reinterpret_cast<std::string*>($2);
				opr->addHotKey( key, @2 );
			}
			| opr_keyb '+' QUOTED_IDENTIF {
				RDOOPROperation* opr = reinterpret_cast<RDOOPROperation*>($1);
				std::string      key = *reinterpret_cast<std::string*>($3);
				opr->addHotKey( key, @3 );
			};

opr_param:	opr_param IDENTIF {
				RDOOPROperation* opr   = reinterpret_cast<RDOOPROperation*>($1);
				std::string      param = *reinterpret_cast<std::string*>($2);
				opr->addParam( param, @2 );
			}
			| opr_param INT_CONST {
				RDOOPROperation* opr   = reinterpret_cast<RDOOPROperation*>($1);
				int              param = $2;
				opr->addParam( param, @2 );
			}
			| opr_param REAL_CONST {
				RDOOPROperation* opr   = reinterpret_cast<RDOOPROperation*>($1);
				double           param = *reinterpret_cast<double*>($2);
				opr->addParam( param, @2 );
			}
			| opr_param '*' {
				RDOOPROperation* opr = reinterpret_cast<RDOOPROperation*>($1);
				opr->addParam( @2 );
			}
			| opr_keyb;

opr_end:	opr_param End {
				RDOOPROperation* opr = reinterpret_cast<RDOOPROperation*>($1);
				opr->endParam( @1 );
				parser->setHaveKWOperationsEnd( true );
			};

%%

}
