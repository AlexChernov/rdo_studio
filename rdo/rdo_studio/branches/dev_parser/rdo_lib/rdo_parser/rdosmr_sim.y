/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdosmr_sim.y
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

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
%token RDO_set							319
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
%token RDO_CF							371
%token RDO_Priority						372
%token RDO_prior						373
%token RDO_Parent						374
%token RDO_PlusEqual					375
%token RDO_MinusEqual					376
%token RDO_MultiplyEqual				377
%token RDO_DivideEqual					378
%token RDO_array						379
%token RDO_event						380
%token RDO_Planning						381
%token RDO_else							382
%token RDO_IncrEqual					383
%token RDO_DecrEqual					384

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
%token RDO_typedef						428
%token RDO_enum							429

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
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define P_ARITHM(A)   reinterpret_cast<PTR(RDOFUNArithm)>(A)
#define P_LOGIC(A)    reinterpret_cast<PTR(RDOFUNLogic)>(A)

#define RDOVALUE(A)   (*P_RDOVALUE(A))
#define ARITHM(A)     (*P_ARITHM(A))
#define LOGIC(A)      (*P_LOGIC(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start smr_cond

%%

smr_show_mode
	: RDO_NoShow
	{
		$$ = rdoSimulator::SM_NoShow;
	}
	| RDO_Monitor
	{
		$$ = rdoSimulator::SM_Monitor;
	}
	| RDO_Animation
	{
		$$ = rdoSimulator::SM_Animation;
	}
	;

smr_cond
	: /* empty */
	| smr_cond RDO_IDENTIF '.' RDO_Planning '(' fun_arithm ')'
	{
		tstring           eventName   = RDOVALUE($2)->getIdentificator();
		PTR(RDOFUNArithm) pTimeArithm = P_ARITHM($6);
		LPRDOEvent        pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@2, rdo::format(_T("������� ������������� ����������� �������: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalc pCalcTime = pTimeArithm->createCalc(NULL);
		ASSERT(pCalcTime);

		LPIBaseOperation pBaseOperation = pEvent->getRuntimeEvent();
		ASSERT(pBaseOperation);
		RUNTIME->addTimePoint(pCalcTime->calcValue(RUNTIME).getDouble(), pBaseOperation);
	}
	| smr_cond RDO_Model_name                 '=' RDO_IDENTIF
	| smr_cond RDO_Resource_file              '=' RDO_IDENTIF
	| smr_cond RDO_OprIev_file                '=' RDO_IDENTIF
	| smr_cond RDO_Frame_file                 '=' RDO_IDENTIF
	| smr_cond RDO_Statistic_file             '=' RDO_IDENTIF
	| smr_cond RDO_Results_file               '=' RDO_IDENTIF
	| smr_cond RDO_Trace_file                 '=' RDO_IDENTIF
	| smr_cond RDO_External_Model RDO_IDENTIF '=' RDO_IDENTIF
	| smr_cond RDO_Show_mode                  '=' smr_show_mode
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setShowMode( (rdoSimulator::ShowMode)$4 );
	}
	| smr_cond RDO_Show_mode '=' error
	{
		PARSER->error().error( @3, @4, "��������� ����� ��������" );
	}
	| smr_cond RDO_Show_mode error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Frame_number '=' RDO_INT_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setFrameNumber( reinterpret_cast<RDOValue*>($4)->value().getInt(), @4 );
	}
	| smr_cond RDO_Frame_number '=' error
	{
		PARSER->error().error( @3, @4, "��������� ��������� ����� �����" );
	}
	| smr_cond RDO_Frame_number error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Show_rate '=' RDO_REAL_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setShowRate( reinterpret_cast<RDOValue*>($4)->value().getDouble(), @4 );
	}
	| smr_cond RDO_Show_rate '=' RDO_INT_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setShowRate( reinterpret_cast<RDOValue*>($4)->value().getInt(), @4 );
	}
	| smr_cond RDO_Show_rate '=' error
	{
		PARSER->error().error( @3, @4, "��������� ���������� �����������" );
	}
	| smr_cond RDO_Show_rate error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Run_StartTime '=' RDO_REAL_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setRunStartTime( reinterpret_cast<RDOValue*>($4)->value().getDouble(), @4 );
	}
	| smr_cond RDO_Run_StartTime '=' RDO_INT_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setRunStartTime( reinterpret_cast<RDOValue*>($4)->value().getInt(), @4 );
	}
	| smr_cond RDO_Run_StartTime '=' error
	{
		PARSER->error().error( @3, @4, "��������� ��������� ��������� �����" );
	}
	| smr_cond RDO_Run_StartTime error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Trace_StartTime '=' RDO_REAL_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setTraceStartTime( reinterpret_cast<RDOValue*>($4)->value().getDouble(), @4 );
	}
	| smr_cond RDO_Trace_StartTime '=' RDO_INT_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setTraceStartTime( reinterpret_cast<RDOValue*>($4)->value().getInt(), @4 );
	}
	| smr_cond RDO_Trace_StartTime '=' error
	{
		PARSER->error().error( @3, @4, "��������� ��������� ����� �����������" );
	}
	| smr_cond RDO_Trace_StartTime error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Trace_EndTime '=' RDO_REAL_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setTraceEndTime( reinterpret_cast<RDOValue*>($4)->value().getDouble(), @4 );
	}
	| smr_cond RDO_Trace_EndTime '=' RDO_INT_CONST
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->setTraceEndTime( reinterpret_cast<RDOValue*>($4)->value().getInt(), @4 );
	}
	| smr_cond RDO_Trace_EndTime '=' error
	{
		PARSER->error().error( @3, @4, "��������� �������� ����� �����������" );
	}
	| smr_cond RDO_Trace_EndTime error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_Terminate_if fun_logic
	{
		PARSER->getSMR()->setTerminateIf( reinterpret_cast<RDOFUNLogic*>($3) );
	}
	| smr_cond RDO_Terminate_if error
	{
		PARSER->error().error( @2, @3, "������ ����������� ��������� � ������������ �������" );
	}
	| smr_cond RDO_Break_point RDO_IDENTIF fun_logic
	{
		RDOSMR* smr = PARSER->getSMR();
		smr->insertBreakPoint( reinterpret_cast<RDOValue*>($3)->src_info(), reinterpret_cast<RDOFUNLogic*>($4) );
	}
	| smr_cond RDO_Break_point RDO_IDENTIF error
	{
		PARSER->error().error( @4, "������ ����������� ��������� � ����� ��������" );
	}
	| smr_cond RDO_Break_point error
	{
		PARSER->error().error( @2, @3, "��������� ��� ����� ��������" );
	}
	| smr_cond RDO_IDENTIF '=' fun_arithm
	{
		PARSER->getSMR()->setConstValue( reinterpret_cast<RDOValue*>($2)->src_info(), reinterpret_cast<RDOFUNArithm*>($4) );
	}
	| smr_cond RDO_IDENTIF '=' error
	{
		PARSER->error().error( @3, @4, "������ � �������������� ���������" );
	}
	| smr_cond RDO_IDENTIF error
	{
		PARSER->error().error( @2, "��������� '='" );
	}
	| smr_cond RDO_IDENTIF '.' RDO_IDENTIF '=' fun_arithm
	{
		PARSER->getSMR()->setResParValue( reinterpret_cast<RDOValue*>($2)->src_info(), reinterpret_cast<RDOValue*>($4)->src_info(), reinterpret_cast<RDOFUNArithm*>($6) );
	}
	| smr_cond RDO_IDENTIF '.' RDO_IDENTIF '=' error
	{
		PARSER->error().error( @5, @6, "������ � �������������� ���������" );
	}
	| smr_cond RDO_IDENTIF '.' RDO_IDENTIF error
	{
		PARSER->error().error( @4, "��������� '='" );
	}
	| smr_cond RDO_IDENTIF '.' error
	{
		std::string name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
		const RDORSSResource* res = PARSER->findRSSResource( name );
		if ( res )
		{
			PARSER->error().error( @3, @4, "��������� ��������" );
		}
		else
		{
			const RDOFUNSequence* seq = PARSER->findFUNSequence( name );
			if ( seq )
			{
				PARSER->error().error( @3, @4, "��������� �������� ����� Seed" );
			}
			else
			{
				PARSER->error().error( @2, "����������� �������� ��� ������������������" );
			}
		}
	}
	| smr_cond RDO_IDENTIF '.' RDO_Seed '=' RDO_INT_CONST
	{
		PARSER->getSMR()->setSeed( reinterpret_cast<RDOValue*>($2)->src_info(), reinterpret_cast<RDOValue*>($6)->value().getInt() );
	}
	| smr_cond RDO_IDENTIF '.' RDO_Seed '=' error
	{
		PARSER->error().error( @5, @6, "��������� ���� ����������" );
	}
	| smr_cond RDO_IDENTIF '.' RDO_Seed error
	{
		PARSER->error().error( @4, "��������� '='" );
	}
	| smr_cond error
	{
		PARSER->error().error( @2, "����������� ������" );
	}
	;

// ----------------------------------------------------------------------------
// ---------- ����� ��������� ������ ��� ���� �������� ���
// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
fun_logic_eq
	: RDO_eq { $$ = RDO_eq; }
	;

fun_logic
	: fun_arithm  fun_logic_eq  fun_arithm   { $$ = (int)(ARITHM($1) == ARITHM($3));  }
	| fun_arithm  RDO_neq       fun_arithm   { $$ = (int)(ARITHM($1) != ARITHM($3));  }
	| fun_arithm  '<'           fun_arithm   { $$ = (int)(ARITHM($1) <  ARITHM($3));  }
	| fun_arithm  '>'           fun_arithm   { $$ = (int)(ARITHM($1) >  ARITHM($3));  }
	| fun_arithm  RDO_leq       fun_arithm   { $$ = (int)(ARITHM($1) <= ARITHM($3));  }
	| fun_arithm  RDO_geq       fun_arithm   { $$ = (int)(ARITHM($1) >= ARITHM($3));  }
	| fun_logic   RDO_and       fun_logic    { $$ = (int)(LOGIC($1) && LOGIC($3));    }
	| fun_logic   RDO_or        fun_logic    { $$ = (int)(LOGIC($1) || LOGIC($3));    }
	| fun_arithm                             { $$ = (int)new RDOFUNLogic(ARITHM($1)); }
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		PTR(RDOFUNLogic) logic = P_LOGIC($2);
		logic->setSrcPos (@1, @3);
		logic->setSrcText(_T("[") + logic->src_text() + _T("]"));
		$$ = $2;
	}
	| '(' fun_logic ')'
	{
		PTR(RDOFUNLogic) logic = P_LOGIC($2);
		logic->setSrcPos (@1, @3);
		logic->setSrcText(_T("(") + logic->src_text() + _T(")"));
		$$ = $2;
	}
	| RDO_not fun_logic
	{
		PTR(RDOFUNLogic) logic = P_LOGIC($2);
		PTR(RDOFUNLogic) logic_not = logic->operator_not();
		logic_not->setSrcPos (@1, @2);
		logic_not->setSrcText(_T("not ") + logic->src_text());
		$$ = (int)logic_not;
	}
	| '[' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
	| RDO_REAL_CONST                     { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
	| RDO_BOOL_CONST                     { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
	| RDO_STRING_CONST                   { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
	| RDO_IDENTIF                        { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1)); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1), RDOVALUE($3)); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = (int)new RDOFUNArithm(PARSER, RDOVALUE($1), RDOVALUE($3)); }
	| fun_arithm '+' fun_arithm		     { $$ = (int)(ARITHM($1) + ARITHM($3)); }
	| fun_arithm '-' fun_arithm		     { $$ = (int)(ARITHM($1) - ARITHM($3)); }
	| fun_arithm '*' fun_arithm		     { $$ = (int)(ARITHM($1) * ARITHM($3)); }
	| fun_arithm '/' fun_arithm		     { $$ = (int)(ARITHM($1) / ARITHM($3)); }
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		PTR(RDOFUNArithm) arithm = P_ARITHM($2);
		arithm->setSrcPos (@1, @3);
		arithm->setSrcText(_T("(") + arithm->src_text() + _T(")"));
		$$ = $2;
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + ARITHM($2).src_text());
		$$ = (int)new RDOFUNArithm(PARSER, RDOValue(ARITHM($2).type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(ARITHM($2).createCalc()));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')' 
	{
		PTR(RDOFUNParams) fun = new RDOFUNParams(PARSER);
		tstring fun_name = RDOVALUE($1)->getIdentificator();
		fun->funseq_name.setSrcInfo(RDOParserSrcInfo(@1, fun_name));
		fun->setSrcPos (@1, @3);
		fun->setSrcText(fun_name + _T("()"));
		PTR(RDOFUNArithm) arithm = fun->createCall(fun_name);
		$$ = (int)arithm;
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		PTR(RDOFUNParams) fun      = reinterpret_cast<PTR(RDOFUNParams)>($3);
		tstring           fun_name = RDOVALUE($1)->getIdentificator();
		fun->funseq_name.setSrcInfo(RDOParserSrcInfo(@1, fun_name));
		fun->setSrcPos (@1, @4);
		fun->setSrcText(fun_name + _T("(") + fun->src_text() + _T(")"));
		PTR(RDOFUNArithm) arithm = fun->createCall(fun_name);
		$$ = (int)arithm;
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		PTR(RDOFUNParams) fun    = new RDOFUNParams(PARSER);
		PTR(RDOFUNArithm) arithm = P_ARITHM($1);
		fun->setSrcText  (arithm->src_text());
		fun->addParameter(arithm);
		$$ = (int)fun;
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		PTR(RDOFUNParams) fun    = reinterpret_cast<PTR(RDOFUNParams)>($1);
		PTR(RDOFUNArithm) arithm = P_ARITHM($3);
		fun->setSrcText  (fun->src_text() + _T(", ") + arithm->src_text());
		fun->addParameter(arithm);
		$$ = (int)fun;
	}
	| fun_arithm_func_call_pars error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = (int)(new RDOFUNGroupLogic(PARSER, (RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
	}
	| fun_group_keyword '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, _T("����� ����� ������� ��������� ������������� ������"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		PTR(RDOFUNGroupLogic) groupfun = reinterpret_cast<PTR(RDOFUNGroupLogic)>($1);
		groupfun->setSrcPos(@1, @3);
		$$ = (int)groupfun->createFunLogic(P_LOGIC($2));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		PTR(RDOFUNGroupLogic) groupfun = reinterpret_cast<PTR(RDOFUNGroupLogic)>($1);
		groupfun->setSrcPos(@1, @3);
		PTR(RDOFUNLogic) trueLogic = new RDOFUNLogic(groupfun, rdo::Factory<rdoRuntime::RDOCalcConst>::create(1));
		trueLogic->setSrcPos (@2);
		trueLogic->setSrcText(_T("NoCheck"));
		$$ = (int)groupfun->createFunLogic(trueLogic);
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name  = P_RDOVALUE($3);
		PTR(RDOFUNSelect) select = new RDOFUNSelect(PARSER, type_name->src_info());
		select->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
		$$ = (int)select;
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, _T("��������� ������������� ������"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
		PTR(RDOFUNLogic)  flogic = P_LOGIC($2);
		select->setSrcText(select->src_text() + flogic->src_text() + _T(")"));
		select->initSelect(flogic);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
		RDOParserSrcInfo logic_info(@2, _T("NoCheck"));
		select->setSrcText(select->src_text() + logic_info.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst calc_nocheck = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		PTR(RDOFUNLogic)           flogic       = new RDOFUNLogic(select, calc_nocheck, true);
		flogic->setSrcInfo(logic_info);
		select->initSelect(flogic);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

fun_select_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')'
	{
		PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
		select->setSrcPos(@1, @6);
		PTR(RDOFUNLogic) logic = select->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, P_LOGIC($5));
		$$ = (int)logic;
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		PARSER->error().error(@4, @5, _T("������ � ���������� ���������"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
		select->setSrcPos(@1, @5);
		RDOParserSrcInfo empty_info(@3, @5, _T("Empty()"));
		PTR(RDOFUNLogic) logic = select->createFunSelectEmpty(empty_info);
		$$ = (int)logic;
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, _T("��������� ����� ������ ��������"));
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, _T("��������� '.' (�����) ��� ������ ������ ������ ��������"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		PTR(RDOFUNSelect) select = reinterpret_cast<PTR(RDOFUNSelect)>($1);
		select->setSrcPos(@1, @5);
		RDOParserSrcInfo size_info(@3, @5, _T("Size()"));
		PTR(RDOFUNArithm) arithm = select->createFunSelectSize(size_info);
		$$ = (int)arithm;
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE
