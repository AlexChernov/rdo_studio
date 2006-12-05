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
%token temporary			263
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
#include "rdosmr.h"
#include "rdofun.h"
#include "rdoruntime.h"

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%start smr_cond

%%

smr_model: Model_name '=' IDENTIF;

smr_descr: smr_model
		|	smr_descr Resource_file		'=' IDENTIF
		|	smr_descr OprIev_file		'=' IDENTIF
		|	smr_descr Frame_file			'=' IDENTIF
		|	smr_descr Statistic_file	'=' IDENTIF
		|	smr_descr Results_file		'=' IDENTIF
		|	smr_descr Trace_file			'=' IDENTIF
		|	smr_descr Show_mode			'=' smr_show_mode		
		|	smr_descr Frame_number		'=' INT_CONST
		|	smr_descr Show_rate			'=' REAL_CONST
		|	smr_descr Run_StartTime		'=' REAL_CONST
		|	smr_descr Trace_StartTime	'=' REAL_CONST
		|	smr_descr Trace_EndTime		'=' REAL_CONST;

smr_show_mode:		NoShow	
					|	Monitor 	
					|	Animation;

smr_cond:	smr_descr
		|	smr_cond Terminate_if fun_logic						{ parser->smr->setTerminateIf((RDOFUNLogic *)$3); @$; }
		|	smr_cond Break_point IDENTIF	fun_logic
		|	smr_cond IDENTIF				'=' fun_arithm		{ parser->smr->setConstValue((std::string *)$2, (RDOFUNArithm *)$4); }
		|	smr_cond IDENTIF '.' IDENTIF	'=' fun_arithm		{ parser->smr->setResParValue((std::string *)$2, (std::string *)$4, (RDOFUNArithm *)$6); }
		|	smr_cond IDENTIF '.' Seed		'=' INT_CONST		{ parser->smr->setSeed((std::string *)$2, $6); };

// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
// ���� �� �������������� RDOErrorPos, �� � ����. ���������� ��� �����������
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
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| '(' fun_logic ')'					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| not_keyword fun_logic				{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setErrorPos( @1.first_line, @1.first_column, @2.last_line, @2.last_column );
				$$ = (int)logic->operator_not();
			}
			| fun_group							{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_logic					{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($1);
				logic->setErrorPos( @1 );
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
				arithm->setErrorPos( @1.first_line, @1.first_column, @3.last_line, @3.last_column );
				$$ = $2;
			}
			| fun_arithm_func_call			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| fun_select_arithm				{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
				arithm->setErrorPos( @1 );
				$$ = $1;
			}
			| IDENTIF '.' IDENTIF			{
				$$ = (int)new RDOFUNArithm( reinterpret_cast<std::string*>($1), reinterpret_cast<std::string*>($3), @1, @3 );
			}
			| INT_CONST						{ $$ = (int)new RDOFUNArithm( (int)$1, @1 );          }
			| REAL_CONST					{ $$ = (int)new RDOFUNArithm( (double*)$1, @1 );      }
			| IDENTIF						{ $$ = (int)new RDOFUNArithm( (std::string*)$1, @1 ); }
			| '-' fun_arithm %prec UMINUS	{
				YYLTYPE error_pos;
				error_pos.first_line   = @1.first_line;
				error_pos.first_column = @1.first_column;
				error_pos.last_line    = @2.last_line;
				error_pos.last_column  = @2.last_column;
				$$ = (int)new RDOFUNArithm( reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), error_pos );
			}
			| error							{
				parser->lexer_loc_set( &(@1) );
				if ( @1.first_line = @1.last_line ) {
					parser->error( rdo::format("����������� �������������: %s", reinterpret_cast<RDOLexer*>(lexer)->YYText()) );
				} else {
					parser->error( "������ � �������������� ���������" );
				}
			};

fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun = ((RDOFUNParams*)$3);
							fun->name_error_pos.setErrorPos( @1 );
							fun->setErrorPos( @1.first_line, @1.first_column, @4.last_line, @4.last_column );
							$$ = (int)fun->createCall((std::string *)$1);
						}
						| IDENTIF '(' error {
							parser->lexer_loc_set( &(@3) );
							parser->error( "������ � ���������� �������" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams();
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @2 );
								fun = fun->addParameter((RDOFUNArithm *)$2);
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun = reinterpret_cast<RDOFUNParams*>($1);
								fun->setErrorPos( @3 );
								fun = fun->addParameter((RDOFUNArithm *)$3);
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
						$$ = (int)(new RDOFUNGroupLogic($1, (std::string *)$3));
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
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic((RDOFUNLogic *)$2));
					}
					| fun_group_header NoCheck ')' {
						RDOFUNLogic* trueLogic = new RDOFUNLogic( new rdoRuntime::RDOCalcConst(1) );
						trueLogic->setErrorPos( @2 );
						$$ = (int)(((RDOFUNGroupLogic *)$1)->createFunLogic( trueLogic ));
					}
					| fun_group_header fun_logic error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					}
					| fun_group_header NoCheck error {
						parser->lexer_loc_set( @2.last_line, @2.last_column );
						parser->error( "��������� ������������� ������" );
					};

fun_select_header:	Select '(' IDENTIF_COLON {
						$$ = (int)new RDOFUNSelect((std::string*)$3);
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
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect((RDOFUNLogic*)$2);
						logic->setErrorPos( @2 );
						$$ = $1;
					}
					| fun_select_header NoCheck ')' {
						RDOFUNLogic* logic = ((RDOFUNSelect*)$1)->createFunSelect();
						logic->setErrorPos( @2 );
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
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, (RDOFUNLogic*)$5 );
						select->setErrorPos( @1.first_line, @1.first_column, @6.last_line, @6.last_column );
						logic->setErrorPos( select->error() );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic* logic = select->createFunSelectEmpty();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						logic->setErrorPos( select->error() );
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
						RDOFUNArithm* arithm = select->createFunSelectSize();
						select->setErrorPos( @1.first_line, @1.first_column, @5.last_line, @5.last_column );
						arithm->setErrorPos( select->error() );
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