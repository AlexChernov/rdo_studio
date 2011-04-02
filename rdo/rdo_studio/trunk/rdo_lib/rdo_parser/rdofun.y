/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofun.y
 * author   : �������� ����, ������ ������
 * date     : 20.02.2003
 * bref     : ��������� �������, �������� � �������������������
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type
%token RDO_permanent
%token RDO_Parameters
%token RDO_integer
%token RDO_real
%token RDO_End
%token RDO_temporary
%token RDO_IDENTIF
%token RDO_INT_CONST
%token RDO_REAL_CONST
%token RDO_such_as
%token RDO_dblpoint
%token RDO_Resources
%token RDO_trace
%token RDO_no_trace
%token RDO_IDENTIF_COLON
%token RDO_Constant
%token RDO_Body
%token RDO_Function
%token RDO_Type
%token RDO_algorithmic
%token RDO_table
%token RDO_list
%token RDO_Exist
%token RDO_Not_Exist
%token RDO_For_All
%token RDO_Not_For_All
%token RDO_neq
%token RDO_leq
%token RDO_geq
%token RDO_NoCheck
%token RDO_Calculate_if
%token RDO_or
%token RDO_and
%token RDO_Sequence
%token RDO_uniform
%token RDO_exponential
%token RDO_normal
%token RDO_by_hist
%token RDO_enumerative

%token RDO_Pattern
%token RDO_operation
%token RDO_rule
%token RDO_keyboard
%token RDO_Relevant_resources
%token RDO_Keep
%token RDO_Create
%token RDO_Erase
%token RDO_NonExist
%token RDO_IDENTIF_NoChange
%token RDO_Time
%token RDO_Choice
%token RDO_from
%token RDO_first
%token RDO_Convert_begin
%token RDO_Convert_end
%token RDO_Convert_rule
%token RDO_Convert_event
%token RDO_with_max
%token RDO_with_min
%token RDO_set
%token RDO_IDENTIF_NoChange_NoChange
%token RDO_Operations

%token RDO_Results
%token RDO_watch_par
%token RDO_watch_state
%token RDO_watch_quant
%token RDO_watch_value
%token RDO_get_value

%token RDO_Show_mode
%token RDO_Frame_number
%token RDO_Show_rate
%token RDO_Run_StartTime
%token RDO_Trace_StartTime
%token RDO_Trace_EndTime
%token RDO_Terminate_if
%token RDO_Break_point
%token RDO_Seed
%token RDO_NoShow
%token RDO_Monitor
%token RDO_Animation
%token RDO_NoChange

%token RDO_Decision_point
%token RDO_search
%token RDO_trace_stat
%token RDO_trace_tops
%token RDO_trace_all
%token RDO_Condition
%token RDO_Term_condition
%token RDO_Evaluate_by
%token RDO_Compare_tops
%token RDO_NO
%token RDO_YES
%token RDO_Activities
%token RDO_value_before
%token RDO_value_after
%token RDO_some
%token RDO_Process
%token RDO_SEIZE
%token RDO_GENERATE
%token RDO_TERMINATE
%token RDO_ADVANCE
%token RDO_RELEASE
%token RDO_if
%token RDO_for
%token RDO_result
%token RDO_CF
%token RDO_Priority
%token RDO_prior
%token RDO_Parent
%token RDO_PlusEqual
%token RDO_MinusEqual
%token RDO_MultiplyEqual
%token RDO_DivideEqual
%token RDO_array
%token RDO_event
%token RDO_Planning
%token RDO_else
%token RDO_IncrEqual
%token RDO_DecrEqual
%token RDO_Stopping
%token RDO_Start
%token RDO_Stop
%token RDO_WatchStart
%token RDO_WatchStop

%token RDO_Frame
%token RDO_Show_if
%token RDO_Back_picture
%token RDO_Show
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
%token RDO_ellipse
%token RDO_triang
%token RDO_active
%token RDO_ruler
%token RDO_space
%token RDO_color_transparent
%token RDO_color_last
%token RDO_color_white
%token RDO_color_black
%token RDO_color_red
%token RDO_color_green
%token RDO_color_blue
%token RDO_color_cyan
%token RDO_color_magenta
%token RDO_color_yellow
%token RDO_color_gray

%token RDO_IDENTIF_RELRES
%token RDO_typedef
%token RDO_enum

%token RDO_STRING_CONST
%token RDO_STRING_CONST_BAD
%token RDO_IDENTIF_BAD
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_not
%token RDO_UMINUS
%token RDO_string
%token RDO_bool
%token RDO_BOOL_CONST
%token RDO_Fuzzy
%token RDO_Fuzzy_Term
%token RDO_eq
%token RDO_External_Model
%token RDO_QUEUE
%token RDO_DEPART
%token RDO_ASSIGN

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
#include "rdo_lib/rdo_parser/rdo_type_param_suchas.h"
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
	: RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue) name = P_RDOVALUE($1);
		PARSER->checkFunctionName(name->src_info());
		LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($2);
		ASSERT(pParamType);
		LPRDOFUNConstant pConstant = rdo::Factory<RDOFUNConstant>::create(name->src_info(), pParamType, RDOVALUE($3));
		ASSERT(pConstant);
		RUNTIME->setConstValue(pConstant->getNumber(), pConstant->getDefault().value());
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
	: RDO_Function RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue) name = reinterpret_cast<PTR(RDOValue)>($2);
		PARSER->checkFunctionName(name->src_info());
		LPRDOTypeParam pRetType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pRetType);
		LPRDOParam pReturn = rdo::Factory<RDOParam>::create(name->src_info(), pRetType, RDOVALUE($4));
		ASSERT(pReturn);
		LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(name->src_info(), pReturn);
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
	| fun_func_params RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue)  name  = P_RDOVALUE($2);
		LPRDOTypeParam pType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pType);
		LPRDOParam pParam = rdo::Factory<RDOParam>::create(name->src_info(), pType, RDOVALUE($4));
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
	: RDO_integer param_type_range
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
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPRDOTypeParam pType  = rdo::Factory<RDOTypeParam>::create(pArray, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as
	{
		LPRDOTypeParamSuchAs pTypeSuchAs = PARSER->stack().pop<RDOTypeParamSuchAs>($1);
		ASSERT(pTypeSuchAs);
		LPRDOTypeParam pType = pTypeSuchAs.object_parent_cast<RDOTypeParam>();
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;

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
		LPRDORTPParam pRTPParam = pResType->findRTPParam(param);
		if (!pRTPParam)
		{
			PARSER->error().error(@4, rdo::format(_T("������ �� ����������� �������� �������: %s.%s"), type.c_str(), param.c_str()));
		}
		LPRDOParam pParam = pRTPParam.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� �������������� ���������: %s"), constName.c_str()));
		}
		LPRDOParam pParam = pConstant.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
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
	| '=' param_value
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

param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST {
		$$ = $1;
	}
	| RDO_STRING_CONST {
		$$ = $1;
	}
	| RDO_IDENTIF {
		$$ = $1;
	}
	| RDO_BOOL_CONST {
		$$ = $1;
	}
	| param_array_value {
		$$ = $1;
	}
	;

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType->type(), RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
	}
	;

param_array_value
	:	'[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), pArrayValue->getArrayType(), RDOParserSrcInfo(@2)));
	}
	|'[' array_item error {
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getAsString().c_str()));
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
		RDOParserSrcInfo src_info(@1, @2);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not(src_info.src_pos());
		ASSERT(pLogicNot);
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
	| param_array_value					 { $$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
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
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOValue(pArithm->type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(info.src_pos(), pArithm->createCalc())));
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
