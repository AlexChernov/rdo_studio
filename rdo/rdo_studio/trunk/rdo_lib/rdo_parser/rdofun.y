/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofun.y
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
%token RDO_Stopping						385
%token RDO_Start						386
%token RDO_Stop							387

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
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and	
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start fun_list

%%

fun_list
	: fun_consts fun_func_seq
	| error
	{
		if (PARSER->getFUNConstants().empty())
		{
			PARSER->error().error(@1, _T("��������� �������� �������, ������������������ ��� ���������"));
		}
		else
		{
			PARSER->error().error(@1, _T("��������� �������� ������� ��� ������������������"));
		}
	}
	;

// ----------------------------------------------------------------------------
// ---------- ���������
// ----------------------------------------------------------------------------
fun_consts
	: /* empty */
	| RDO_Constant fun_const_body RDO_End
	| RDO_Constant fun_const_body
	{
		PARSER->error().error(@2, _T("����� �������� �������� ��������� �������� ����� $End"));
	}
	;

fun_const_body
	: /* empty */
	| fun_const_body fun_const_param_desc
	| fun_const_body error
	{
		PARSER->error().error(@2, _T("��������� �������� ���������"));
	}
	;

fun_const_param_desc
	: RDO_IDENTIF_COLON param_type
	{
		PTR(RDOValue) name = P_RDOVALUE($1);
		PARSER->checkFunctionName(name->src_info());
		LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($2);
		ASSERT(pParamType);
		if (!pParamType->default().defined())
		{
			PARSER->error().error(@2, _T("��������� ������ ����� ��������"));
		}
		LPRDOFUNConstant pConstant = rdo::Factory<RDOFUNConstant>::create(name->src_text(), pParamType);
		ASSERT(pConstant);
		pConstant->setSrcInfo( name->src_info() );
		RUNTIME->setConstValue(pConstant->getNumber(), pConstant->getType()->default().value());
		$$ = PARSER->stack().push(pConstant);
	}
	| RDO_IDENTIF_COLON
	{
		PARSER->error().error(@1, _T("��������� ��� ���������"));
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, _T("������ �������� ���� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������������������ � �������
// ----------------------------------------------------------------------------
fun_func_seq
	: /* empty */
	| fun_func_seq fun_func_descr
	| fun_func_seq fun_seq_descr
	| fun_func_seq RDO_Constant
	{
		PARSER->error().error(@2, _T("��������� ����� ���� ������� �������, ����� ��������� � ��������������������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
fun_func_descr
	: fun_func_header fun_func_footer
	| fun_func_header error
	{
		LPRDOFUNFunction pFunction = PARSER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		PARSER->error().error(@2, rdo::format(_T("��������� �������� ����� $Type � ��������� ���� ������� '%s'"), pFunction->name().c_str()));
	}
	;

fun_func_header
	: RDO_Function RDO_IDENTIF_COLON param_type
	{
		PTR(RDOValue) name = reinterpret_cast<PTR(RDOValue)>($2);
		PARSER->checkFunctionName( name->src_info() );
		LPRDOTypeParam pRetType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pRetType)
		LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(name->src_info(), pRetType);
		ASSERT(pFunction);
		$$ = PARSER->stack().push(pFunction);
	}
	| RDO_Function RDO_IDENTIF_COLON error
	{
		tstring name = P_RDOVALUE($2)->value().getIdentificator();
		PARSER->error().error(@3, rdo::format(_T("��������� ��� ������������� �������� ������� '%s'"), name.c_str()));
	}
	| RDO_Function error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� $Function ��������� ��� ������"));
	}
	;

fun_func_parameters
	: /* empty */
	| RDO_Parameters fun_func_params
	;

fun_func_params
	: /* empty */
	| fun_func_params RDO_IDENTIF_COLON param_type
	{
		PTR(RDOValue)  name  = P_RDOVALUE($2);
		LPRDOTypeParam pType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pType);
		LPRDOFUNFunctionParam pParam = rdo::Factory<RDOFUNFunctionParam>::create(name->src_info(), pType);
		ASSERT(pParam);
		PARSER->getLastFUNFunction()->add(pParam);
	}
	| fun_func_params RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@3, _T("��������� ��� ��������� �������"));
	}
	| fun_func_params error
	{
		PARSER->error().error(@2, _T("��������� �������� ��������� ������� � ������� <���>: <���>"));
	}
	;

fun_func_footer
	:	RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createAlgorithmicCalc(@5);
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createListCalc();
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createTableCalc(@6);
	}
	| RDO_Type '=' RDO_algorithmic fun_func_parameters RDO_Body fun_func_algorithmic_body error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_algorithmic error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' RDO_list error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' RDO_table error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' error
	{
		PARSER->error().error(@3, _T("����������� ��� �������"));
	}
	| RDO_Type error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� $Type ��������� ��� �������"));
	}
	;

fun_func_algorithmic_body
	: /* empty */
	| fun_func_algorithmic_body fun_func_algorithmic_calc_if
	{
		LPRDOFUNCalculateIf pCalculateIf = PARSER->stack().pop<RDOFUNCalculateIf>($2);
		ASSERT(pCalculateIf);
		PARSER->getLastFUNFunction()->add(pCalculateIf);
	}
	;

fun_func_calc_if
	: RDO_Calculate_if
	| RDO_if
	;

fun_func_calc_name
	: RDO_result
	| RDO_IDENTIF
	{
		tstring name = P_RDOVALUE($1)->value().getIdentificator();
		if ( name != PARSER->getLastFUNFunction()->name() )
		{
			PARSER->error().error(@1, rdo::format(_T("��������� ��� ������� '%s'"), PARSER->getLastFUNFunction()->name().c_str()));
		}
	}
	;

fun_func_algorithmic_calc_if
	: fun_func_calc_if fun_logic fun_func_calc_name '=' fun_arithm
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcText(_T("Calculate_if ") + pLogic->src_text());
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($5);
		ASSERT(pArithm);
		LPRDOFUNCalculateIf pCalculateIf = rdo::Factory<RDOFUNCalculateIf>::create(pLogic, pArithm);
		ASSERT(pCalculateIf);
		$$ = PARSER->stack().push(pCalculateIf);
	}
	| fun_func_calc_if fun_logic fun_func_calc_name '=' error
	{
		PARSER->error().error(@5, _T("������ � �������������� ���������"));
	}
	| fun_func_calc_name '=' fun_arithm
	{
		rdoRuntime::LPRDOCalc pCondition = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		RDOParserSrcInfo logic_src_info(_T("Calculate_if 1 = 1"));
		logic_src_info.setSrcPos( @1.m_first_line, @1.m_first_pos, @1.m_first_line, @1.m_first_pos );
		pCondition->setSrcInfo( logic_src_info );
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCondition, true);
		pLogic->setSrcInfo( logic_src_info );
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		LPRDOFUNCalculateIf pCalculateIf = rdo::Factory<RDOFUNCalculateIf>::create(pLogic, pArithm);
		ASSERT(pCalculateIf);
		$$ = PARSER->stack().push(pCalculateIf);
	}
	| fun_func_calc_name '=' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	| fun_func_calc_if fun_logic fun_func_calc_name error
	{
		PARSER->error().error(@4, _T("��������� '='"));
	}
	| fun_func_calc_name error
	{
		PARSER->error().error(@2, _T("��������� '='"));
	}
	| fun_func_calc_if fun_logic error
	{
		PARSER->error().error(@2, @3, _T("����� ����������� ��������� ��������� <���_�������> = <���������_�������>"));
	}
	| fun_func_calc_if error
	{
		PARSER->error().error(@2, _T("������ � ���������� ���������"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� �������� ����� Calculate_if"));
	}
	;

fun_func_list_body
	: /* empty */
	| fun_func_list_body fun_func_list_value
	;

fun_func_list_value
	: RDO_IDENTIF
	{
		tstring str = P_RDOVALUE($1)->value().getIdentificator();
		LPRDOFUNFunctionListElementIdentif pValue = rdo::Factory<RDOFUNFunctionListElementIdentif>::create(RDOParserSrcInfo(@1, str));
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_REAL_CONST
	{
		LPRDOFUNFunctionListElementReal pValue = rdo::Factory<RDOFUNFunctionListElementReal>::create(@1, P_RDOVALUE($1)->value().getDouble());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_INT_CONST
	{
		LPRDOFUNFunctionListElementInt pValue = rdo::Factory<RDOFUNFunctionListElementInt>::create(@1, P_RDOVALUE($1)->value().getInt());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| '='
	{
		LPRDOFUNFunctionListElementEq pValue = rdo::Factory<RDOFUNFunctionListElementEq>::create(@1);
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
fun_seq_descr
	: fun_seq_uniform
	| fun_seq_exponential
	| fun_seq_normal
	| fun_seq_by_hist
	| fun_seq_enumerative
	;

fun_seq_header
	: RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '='
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		PARSER->checkFunctionName( name->src_info() );
		LPRDOTypeParam pType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pType);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = rdo::Factory<RDOFUNSequence::RDOFUNSequenceHeader>::create(pType, name->src_info());
		ASSERT(pHeader);
		$$ = PARSER->stack().push(pHeader);
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' error
	{
		PARSER->error().error(@6, _T("����� ����� ��������� ��������� ��� ������������������"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type error
	{
		PARSER->error().error(@5, _T("����� ��������� ����� $Type ��������� ���� ��������� � ��� ������������������"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Type"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("����� ����� ������������������ ��������� ��� ������������� ��������"));
	}
	| RDO_Sequence error
	{
		PARSER->error().error(@1, @2, _T("����� ��������� ����� $Sequence ��������� ��� � ��� ���������� ������������������ � ������� '<���> : <���>'"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������� ��������� �����
// ----------------------------------------------------------------------------
fun_seq_uniform
	: fun_seq_header RDO_uniform RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_uniform error
	{
		PARSER->error().error(@3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

fun_seq_exponential
	: fun_seq_header RDO_exponential RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_exponential error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

fun_seq_normal
	: fun_seq_header RDO_normal RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_normal error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �����������
// ----------------------------------------------------------------------------
fun_seq_by_hist_header
	: fun_seq_header RDO_by_hist RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader);
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $Body"));
	}
	| fun_seq_header RDO_by_hist error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $Body"));
	}
	;

fun_seq_by_hist_body_real
	: fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if (pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int)
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST error
	{
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST error
	{
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_End
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( pHeader->src_info(), rdo::format(_T("������������������ '%s' �� ������ ���� ������"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_by_hist_body_enum
	: fun_seq_by_hist_header RDO_IDENTIF RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, RDOVALUE($2), RDOVALUE($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, RDOVALUE($2), RDOVALUE($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(RDOVALUE($2), RDOVALUE($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(RDOVALUE($2), RDOVALUE($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, rdo::format(_T("��������� ������������� ����������� ��� ��������: %s"), P_RDOVALUE($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF error
	{
		PARSER->error().error( @2, @3, rdo::format(_T("��������� ������������� ����������� ��� ��������: %s"), P_RDOVALUE($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getType()->src_text().c_str()) );
	}
	;

fun_seq_by_hist
	: fun_seq_by_hist_body_real RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real error
	{
		PARSER->error().error( @2, _T("������ � �������� ������������������") );
	}
	| fun_seq_by_hist_body_enum error
	{
		PARSER->error().error( @2, _T("������ � �������� ������������������") );
	}
	| fun_seq_by_hist_body_real
	{
		PARSER->error().error( @1, _T("��������� �������� ����� $End") );
	}
	| fun_seq_by_hist_body_enum
	{
		PARSER->error().error( @1, _T("��������� �������� ����� $End") );
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
fun_seq_enumerative
	: fun_seq_enumerative_body RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( pHeader->src_info(), rdo::format(_T("������������������ '%s' �� ������ ���� ������"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_enumerative_header
	: fun_seq_header RDO_enumerative RDO_Body
	| fun_seq_header RDO_enumerative RDO_INT_CONST error
	{
		PARSER->error().error( @3, _T("� ������������������ ���� enumerative ��� ���� ����������") );
	}
	| fun_seq_header RDO_enumerative RDO_Parameters error
	{
		PARSER->error().error( @3, _T("� ������������������ ���� enumerative ��� ����������") );
	}
	| fun_seq_header RDO_enumerative error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� �������� ����� $Body") );
	}
	;

fun_seq_enumerative_body
	: fun_seq_enumerative_header RDO_INT_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_REAL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_BOOL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_STRING_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_IDENTIF
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_INT_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_REAL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_BOOL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_STRING_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_IDENTIF
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body error
	{
		PARSER->error().error(@1, @2, _T("��������� ������� ������������������ ��� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������� ���� ���������
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			if (pRange->getMin().typeID() != rdoRuntime::RDOType::t_int ||
			    pRange->getMax().typeID() != rdoRuntime::RDOType::t_int)
			{
				PARSER->error().error(@2, _T("�������� ������ ���� ������ ���� �������������"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_array param_value_default
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPRDOTypeParam pType  = rdo::Factory<RDOTypeParam>::create(pArray, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum param_value_default
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as param_value_default
	{
		LPRDOTypeParam pTypeSuchAs = PARSER->stack().pop<RDOTypeParam>($1);
		ASSERT(pTypeSuchAs);
		RDOValue default = RDOVALUE($2);
		if (!default.defined())
		{
			if (pTypeSuchAs->default().defined())
			{
				default = pTypeSuchAs->default();
			}
		}
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pTypeSuchAs->type(), default, RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;
	//| RDO_integer error
	//{
	//	PARSER->error().error(@2, _T("������ ����� ��������� ����� integer. ��������, �� ������� �������� ��-���������."));
	//}
	//| RDO_real error
	//{
	//	PARSER->error().error(@2, _T("������ ����� ��������� ����� real. ��������, �� ������� �������� ��-���������."));
	//}
	//| param_type_enum error
	//{
	//	PARSER->error().error(@2, _T("������ ����� ������������� ����. ��������, �� ������� �������� ��-���������."));
	//}

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' error
	{
		PARSER->error().error(@2, _T("�������� ����� �������"));
	}
	;

param_type_enum
	: '(' param_type_enum_list ')'
	{
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($2);
		ASSERT(pEnum);
		$$ = PARSER->stack().push(pEnum);
	}
	| '(' param_type_enum_list error
	{
		PARSER->error().error(@2, _T("������������ ������ ������������� �������"));
	}
	;

param_type_enum_list
	: RDO_IDENTIF
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(RDOVALUE($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($3));
			$$ = PARSER->stack().push(pEnum);
		}
		else
		{
			PARSER->error().error(@3, _T("������ � �������� �������� ������������� ����"));
		}
	}
	| param_type_enum_list RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($2));
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getIdentificator().c_str()));
		}
		else
		{
			PARSER->error().error(@2, _T("������ � �������� �������� ������������� ����"));
		}
	}
	| param_type_enum_list ',' RDO_INT_CONST
	{
		PARSER->error().error(@3, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list ',' RDO_REAL_CONST
	{
		PARSER->error().error(@3, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list RDO_INT_CONST
	{
		PARSER->error().error(@2, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list RDO_REAL_CONST
	{
		PARSER->error().error(@2, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| RDO_INT_CONST
	{
		PARSER->error().error(@1, _T("�������� ������������� ���� �� ����� ���������� � �����"));
	}
	| RDO_REAL_CONST
	{
		PARSER->error().error(@1, _T("�������� ������������� ���� �� ����� ���������� � �����"));
	}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF
	{
		tstring type  = RDOVALUE($2)->getIdentificator();
		tstring param = RDOVALUE($4)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� ����������� ��� �������: %s"), type.c_str()));
		}
		LPRDORTPParam pParam = pResType->findRTPParam(param);
		if (!pParam)
		{
			PARSER->error().error(@4, rdo::format(_T("������ �� ����������� �������� �������: %s.%s"), type.c_str(), param.c_str()));
		}
		$$ = PARSER->stack().push(pParam->getParamType());
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� �������������� ���������: %s"), constName.c_str()));
		}
		$$ = PARSER->stack().push(pConstant->getType());
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = RDOVALUE($2)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� ����������� ��� �������: %s"), type.c_str()));
		}
		else
		{
			PARSER->error().error(@4, _T("������ ��� �������� ���������"));
		}
	}
	| RDO_such_as error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� such_as ���������� ������� ��� � ��������� ������� ��� ������"));
	}
	;

param_value_default
	: /* empty */
	{
		$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
	}
	| '=' RDO_INT_CONST
	{
		$$ = $2;
	}
	| '=' RDO_REAL_CONST
	{
		$$ = $2;
	}
	| '=' RDO_STRING_CONST
	{
		$$ = $2;
	}
	| '=' RDO_IDENTIF
	{
		$$ = $2;
	}
	| '=' RDO_BOOL_CONST
	{
		$$ = $2;
	}
	| '=' error
	{
		RDOParserSrcInfo src_info(@1, @2, true);
		if (src_info.src_pos().point())
		{
			PARSER->error().error(src_info, _T("�� ������� �������� ��-���������"));
		}
		else
		{
			PARSER->error().error(src_info, _T("�������� �������� ��-���������"));
		}
	}
	;

param_array
	: RDO_array '<' param_type '>'
	{
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($2);
		ASSERT(pArray);
		$$ = PARSER->stack().push(pArray);
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
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = PARSER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not();
		ASSERT(pLogicNot);
		pLogicNot->setSrcPos (@1, @2);
		pLogicNot->setSrcText(_T("not ") + pLogic->src_text());
		$$ = PARSER->stack().push(pLogicNot);
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
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = PARSER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + pArithm->src_text());
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOValue(pArithm->type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(pArithm->createCalc())));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
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
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
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
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = PARSER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1), false);
		ASSERT(pTrueLogic);
		pTrueLogic->setSrcPos (@2);
		pTrueLogic->setSrcText(_T("NoCheck"));
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
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
		PTR(RDOValue)  type_name = P_RDOVALUE($3);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(type_name->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
		$$ = PARSER->stack().push(pSelect);
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
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo logicInfo(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + logicInfo.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		ASSERT(pCalc);
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCalc, true);
		ASSERT(pLogic);
		pLogic->setSrcInfo(logicInfo);
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
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
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = PARSER->stack().push(pLogicSelect);
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
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
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
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
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
