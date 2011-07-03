/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofrm.y
 * author   : �������� ����, ������ ������
 * date     : 20.02.2003
 * bref     : ��������� ������ ��������
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
%token RDO_triangular
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

%token RDO_ProcessStart
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
%token RDO_Return
%token RDO_Break
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
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/type/range.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_common/rdoanimation.h"
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

%start frm_main

%%

// ----------------------------------------------------------------------------
// ---------- �������� ����
// ----------------------------------------------------------------------------
frm_main
	: /* empty */
	| frm_main frm_end
	| error
	{
		PARSER->error().error(@1, _T("����������� ������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �����
// ----------------------------------------------------------------------------
frm_begin
	: RDO_Frame RDO_IDENTIF
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(P_RDOVALUE($2)->src_info());
		ASSERT(pFrame);
		$$ = PARSER->stack().push(pFrame);
	}
	| RDO_Frame RDO_IDENTIF RDO_Show_if fun_logic
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(P_RDOVALUE($2)->src_info(), PARSER->stack().pop<RDOFUNLogic>($4));
		ASSERT(pFrame);
		$$ = PARSER->stack().push(pFrame);
	}
	| RDO_Frame RDO_IDENTIF RDO_Show_if error
	{
		PARSER->error().error(@4, _T("������ � ���������� ���������"))
	}
	;

frm_background
	: frm_begin RDO_Back_picture '=' frm_color
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($4);
		if (pBgColor->getColorType() != rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent && pBgColor->getColorType() != rdoRuntime::RDOFRMFrame::RDOFRMColor::color_rgb)
		{
			PARSER->error().error(@4, _T("���� ���� �� ����� ���� ������ ������� �� ��������� ��������"));
		}
		pFrame->frame()->setBackgroundColor(pBgColor);
		$$ = PARSER->stack().push(pFrame);
	}
	| frm_begin RDO_Back_picture '='
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		$$ = PARSER->stack().push(pFrame);
	}
	| frm_begin RDO_Back_picture error
	{
		PARSER->error().error(@3, _T("����� ��������� ����� $Back_picture ��������� ���� ���������"));
	}
	| frm_begin error
	{
		PARSER->error().error(@2, _T("����� ����� ����� ��������� �������� ����� $Back_picture"));
	}
	;

frm_backpicture
	: frm_background RDO_IDENTIF
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->frame()->setBackPicture(P_RDOVALUE($2)->value().getIdentificator());
		$$ = PARSER->stack().push(pFrame);
	}
	| frm_background RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->frame()->setBackPicture(P_RDOVALUE($2)->value().getInt(), P_RDOVALUE($3)->value().getInt());
		$$ = PARSER->stack().push(pFrame);
	}
	| frm_background RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("�������� ��������� ����� ��������, ��������� �������� ����� $Show"));
	}
	| frm_background RDO_IDENTIF error
	{
		PARSER->error().error(@3, _T("�������� ��������� ����� ��������, ��������� �������� ����� $Show"));
	}
	| frm_background RDO_INT_CONST error
	{
		PARSER->error().error(@2, _T("����� ������ ����� ���������� ������� ��� ������"));
	}
	| frm_background
	{
		PARSER->error().error(@1, _T("���������� ������� ��� ������� �������� ��� ������ �����"));
	}
	;

frm_show
	: RDO_Show
	{
		LPRDOFRMFrame pFrame = PARSER->getLastFRMFrame();
		ASSERT(pFrame);
		pFrame->frame()->startShow();
	}
	| RDO_Show_if fun_logic
	{
		LPRDOFRMFrame pFrame = PARSER->getLastFRMFrame();
		pFrame->frame()->startShow(PARSER->stack().pop<RDOFUNLogic>($2)->getCalc());
	}
	| RDO_Show_if error
	{
		PARSER->error().error(@2, _T("������ � ���������� ���������"))
	}
	;

frm_item
	: /* empty */
	| frm_item frm_show
	| frm_item frm_text    {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMText              )>($2));}
	| frm_item frm_bitmap  {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMBitmap            )>($2));}
	| frm_item frm_rect    {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMRect              )>($2));}
	| frm_item frm_line    {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMLine              )>($2));}
	| frm_item frm_ellipse {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMEllipse           )>($2));}
	| frm_item frm_r_rect  {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMRectRound         )>($2));}
	| frm_item frm_triang  {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMTriang            )>($2));}
	| frm_item frm_s_bmp   {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMBitmapStretch     )>($2));}
	| frm_item frm_active  {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMActive            )>($2));}
	| frm_item frm_ruler   {PARSER->getLastFRMFrame()->frame()->addRulet(reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMRulet)>($2));}
	| frm_item frm_space   {PARSER->getLastFRMFrame()->frame()->addItem (reinterpret_cast<PTR(rdoRuntime::RDOFRMSpace             )>($2));}
	;

frm_header
	: frm_backpicture frm_item
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		$$ = PARSER->stack().push(pFrame);
	}
	;

frm_end
	: frm_header RDO_End
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->end();
	}
	;

// ----------------------------------------------------------------------------
// ---------- ��������
// ----------------------------------------------------------------------------
frm_color
	: RDO_color_transparent
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent);
	}
	| RDO_color_last
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
	}
	| RDO_color_white
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 255, 255, 255);
	}
	| RDO_color_black
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 0, 0, 0);
	}
	| RDO_color_red
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 255, 0, 0);
	}
	| RDO_color_green
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 0, 255, 0);
	}
	| RDO_color_blue
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 0, 0, 255);
	}
	| RDO_color_cyan
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 0, 255, 255);
	}
	| RDO_color_magenta
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 255, 0, 255);
	}
	| RDO_color_yellow
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 255, 255, 0);
	}
	| RDO_color_gray
	{
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), 127, 127, 127);
	}
	| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST '>'
	{
		LPRDOFUNArithm pRed   = RDOFUNArithm::generateByConst(RDOVALUE($2));
		LPRDOFUNArithm pGreen = RDOFUNArithm::generateByConst(RDOVALUE($3));
		LPRDOFUNArithm pBlue  = RDOFUNArithm::generateByConst(RDOVALUE($4));
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @2), RDOValue(255, @2), @2);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @2);
			pRed->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @3), RDOValue(255, @3), @3);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @3);
			pGreen->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @4), RDOValue(255, @4), @4);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @4);
			pBlue->checkParamType(pType);
		}
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
	}
	| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("������� ��� ������������ �����, ��������� '>'"));
	}
	| '<' RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error(@3, @4, _T("��������� ����� ������������ �����"));
	}
	| '<' RDO_INT_CONST error
	{
		PARSER->error().error(@2, @3, _T("��������� ������� ������������ �����"));
	}
	| '<' fun_arithm ',' fun_arithm ',' fun_arithm '>'
	{
		LPRDOFUNArithm pRed   = PARSER->stack().pop<RDOFUNArithm>($2);
		LPRDOFUNArithm pGreen = PARSER->stack().pop<RDOFUNArithm>($4);
		LPRDOFUNArithm pBlue  = PARSER->stack().pop<RDOFUNArithm>($6);
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @2), RDOValue(255, @2), @2);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @2);
			pRed->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @4), RDOValue(255, @4), @4);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @4);
			pGreen->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0, @6), RDOValue(255, @6), @6);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @6);
			pBlue->checkParamType(pType);
		}
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame(), pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
	}
	| '<' fun_arithm ',' fun_arithm ',' fun_arithm error
	{
		PARSER->error().error(@6, _T("������� ��� ������������ �����, ��������� '>'"));
	}
	| '<' fun_arithm ',' fun_arithm ',' error
	{
		PARSER->error().error(@5, @6, _T("��������� ����� ������������ �����"));
	}
	| '<' fun_arithm ',' fun_arithm error
	{
		PARSER->error().error(@4, _T("����� ������� ������������ ����� ��������� �������"));
	}
	| '<' fun_arithm ',' error
	{
		PARSER->error().error(@3, @4, _T("��������� ������� ������������ �����"));
	}
	| '<' fun_arithm error
	{
		PARSER->error().error(@2, _T("����� ������� ������������ ����� ��������� �������"));
	}
	| '<' error
	{
		PARSER->error().error(@1, _T("����� '<' ��������� ������� ������������ �����"));
	}
	;

frm_postype
	: /* empty */
	{
		$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute;
	}
	| '+'           
	{
		$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta;
	}
	| '*'
	{
		$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::mult;
	}
	| '#' RDO_INT_CONST
	{
		int rilet_id = P_RDOVALUE($2)->value().getInt();
		if (rilet_id <= 0)
		{
			PARSER->error().error(@2, _T("����� ������� ������ ���� ������ ����"));
		}
		if (!RUNTIME->lastFrame()->findRulet(rilet_id))
		{
			PARSER->error().error(@2, rdo::format(_T("������� � ������� '%d' �� ����������"), rilet_id));
		}
		$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet + rilet_id;
	}
	| '#' error
	{
		PARSER->error().error(@1, _T("����� '#' ��������� ������������� ����� �������"));
	}
	;

frm_postype_xy
	: frm_postype
	| '='
	{
		$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::gabarit;
	}
	;

frm_postype_wh
	: frm_postype;
	| '='
	{
		PARSER->error().error(@1, _T("������ ������������ ������ ������������ ��� ������ ��� ������"));
	}
	;

frm_position_xy
	: fun_arithm frm_postype_xy 
	{
		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<RDOFUNArithm>($1)->createCalc();
		if ($2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet)
		{
			$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), pCalc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet);
		}
		else
		{
			$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), pCalc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2);
		}
	}
	;

frm_position_wh
	: fun_arithm frm_postype_wh
	{
		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<RDOFUNArithm>($1)->createCalc();
		if ($2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet)
		{
			$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), pCalc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet);
		}
		else
		{
			$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), pCalc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2);
		}
	}
	;

frm_ruler
	: RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy ']'
	{
		CPTR(rdoRuntime::RDOFRMFrame::RDOFRMRulet) pRulet = RUNTIME->lastFrame()->findRulet(P_RDOVALUE($3)->value().getInt());
		if (pRulet)
		{
			PARSER->error().push_only(@3, rdo::format(_T("������� � ������� '%d' ��� ����������"), P_RDOVALUE($3)->value().getInt()));
			PARSER->error().push_only(pRulet->src_info(), _T("��. ������ �����������"));
			PARSER->error().push_done();
		}
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		if (x->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute)
		{
			PARSER->error().error(@5, _T("��������� ������� ������ ���� �����������"));
		}
		if (y->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute)
		{
			PARSER->error().error(@7, _T("��������� ������� ������ ���� �����������"));
		}
		$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMRulet(RDOParserSrcInfo(@1), P_RDOVALUE($3)->value().getInt(), x, y);
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@7, _T("��������� ']'"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� X"));
	}
	| RDO_ruler '[' RDO_INT_CONST error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_ruler '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ����� �������"));
	}
	| RDO_ruler error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_space
	: RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		$$ = (int)new rdoRuntime::RDOFRMSpace(RUNTIME->lastFrame(), x, y, width, height);
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� ']'"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_space '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_space '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_space '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_space error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_text_align
	: /* empty */ {$$ = rdoAnimation::RDOTextElement::TETA_LEFT;  }
	| '<'         {$$ = rdoAnimation::RDOTextElement::TETA_LEFT;  }
	| '='         {$$ = rdoAnimation::RDOTextElement::TETA_CENTER;}
	| '>'         {$$ = rdoAnimation::RDOTextElement::TETA_RIGHT; }
	;

frm_text_common
	: RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ','
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($13);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text);
		$$ = (int)new rdoRuntime::RDOFRMText(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
/*
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ','
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text);
		$$ = (int)new rdoRuntime::RDOFRMText(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ','
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text);
		$$ = (int)new rdoRuntime::RDOFRMText(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
*/
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@13, _T("��������� �������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� ���� ������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� �������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� ����"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_text '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_text '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_text '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_text error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_text
	: frm_text_common frm_text_align fun_arithm ']'
	{
		reinterpret_cast<PTR(rdoRuntime::RDOFRMText)>($1)->setText((rdoAnimation::RDOTextElement::TextAlign)$2, PARSER->stack().pop<RDOFUNArithm>($3)->createCalc());
	}
	| frm_text_common frm_text_align RDO_STRING_CONST ']'
	{
		reinterpret_cast<PTR(rdoRuntime::RDOFRMText)>($1)->setText((rdoAnimation::RDOTextElement::TextAlign)$2, P_RDOVALUE($3)->value().getString());
	}
	| frm_text_common frm_text_align fun_arithm error
	{
		PARSER->error().error(@3, _T("��������� ']'"));
	}
	| frm_text_common frm_text_align RDO_STRING_CONST error
	{
		PARSER->error().error(@3, _T("��������� ']'"));
	}
	| frm_text_common frm_text_align error
	{
		PARSER->error().error(@2, _T("��������� �������� ������� ��� ������"));
	}
	;

frm_bitmap
	: RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		$$ = (int)new rdoRuntime::RDOFRMBitmap(RUNTIME->lastFrame(), x, y, P_RDOVALUE($7)->value().getIdentificator());
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		$$ = (int)new rdoRuntime::RDOFRMBitmap(RUNTIME->lastFrame(), x, y, P_RDOVALUE($7)->value().getIdentificator(), P_RDOVALUE($9)->value().getIdentificator());
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF error
	{
		PARSER->error().error(@9, _T("��������� ']'"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ��� ����� ��� ��������"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF error
	{
		PARSER->error().error(@7, _T("��������� ']'"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ��� ��������"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_bitmap '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_bitmap '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_bitmap '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_bitmap error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_s_bmp
	: RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		$$ = (int)new rdoRuntime::RDOFRMBitmapStretch(RUNTIME->lastFrame(), x, y, width, height, P_RDOVALUE($11)->value().getIdentificator());
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		$$ = (int)new rdoRuntime::RDOFRMBitmapStretch(RUNTIME->lastFrame(), x, y, width, height, P_RDOVALUE($11)->value().getIdentificator(), P_RDOVALUE($13)->value().getIdentificator());
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF error
	{
		PARSER->error().error(@13, _T("��������� ']'"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� ��� ����� ��� ��������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF error
	{
		PARSER->error().error(@11, _T("��������� ']'"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ��� ��������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_s_bmp '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_s_bmp '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_s_bmp error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_rect
	: RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($13);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRect(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRect(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRect(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRect(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = new rdoRuntime::RDOFRMFrame::RDOFRMPosition(RUNTIME->lastFrame(), rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor  = new rdoRuntime::RDOFRMFrame::RDOFRMColor(RUNTIME->lastFrame());
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRect(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@13, _T("��������� ']'"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� ���� ����� ��������������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� �������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� ����"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_rect '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_rect '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_rect '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_rect error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_r_rect
	: RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($13);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMRectRound(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@13, _T("��������� ']'"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� ���� ����� ��������������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� �������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� ����"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_r_rect '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_r_rect '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_r_rect '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_r_rect error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_ellipse
	: RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($13);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMEllipse(RUNTIME->lastFrame(), x, y, width, height, pBgColor, pFgColor);
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@13, _T("��������� ']'"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� ���� ����� �������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� �������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� ����"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_ellipse '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_ellipse '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_ellipse '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_ellipse error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_line
	: RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) color = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($11);
		color->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMLine(RUNTIME->lastFrame(), x, y, width, height, color);
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� ']'"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� �����"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� Y2"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� X2"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_line '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� Y1"));
	}
	| RDO_line '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_line '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� X1"));
	}
	| RDO_line error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_triang
	: RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color ']'
	{
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x1 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($3);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y1 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($5);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x2 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($7);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y2 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($9);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x3 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($11);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y3 = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($13);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pBgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($15);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor) pFgColor = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMColor)>($17);
		pBgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg);
		pFgColor->setColorType(rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg);
		$$ = (int)new rdoRuntime::RDOFRMTriang(RUNTIME->lastFrame(), x1, y1, x2, y2, x3, y3, pBgColor, pFgColor);
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@17, _T("��������� ']'"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' error
	{
		PARSER->error().error(@16, @17, _T("��������� ���� �����"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error
	{
		PARSER->error().error(@15, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@14, @15, _T("��������� ���� ������������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@13, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@12, @13, _T("��������� Y3"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@11, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� X3"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� Y2"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� X2"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_triang '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� Y1"));
	}
	| RDO_triang '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_triang '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� X1"));
	}
	| RDO_triang error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	}
	;

frm_active
	: RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		tstring opr_name = P_RDOVALUE($2)->value().getIdentificator();
		LPRDODPTFreeActivity pActivity = PARSER->findDPTFreeActivity(opr_name);
		if (!pActivity)
		{
			PARSER->error().error(@2, rdo::format(_T("���������� '%s' �� �������"), opr_name.c_str()));
		}
		else
		{
			if (pActivity->pattern()->getType() != RDOPATPattern::PT_Keyboard)
			{
				PARSER->error().push_only(@2, rdo::format(_T("���������� '%s' ������ ���� ������������"), pActivity->name().c_str()));
				PARSER->error().push_only(pActivity->src_info(), _T("��. ���������"));
				PARSER->error().push_only(pActivity->pattern()->src_info(), _T("��. �������"));
				PARSER->error().push_done();
			}
		}
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) x      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($4);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) y      = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($6);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) width  = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($8);
		PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition) height = reinterpret_cast<PTR(rdoRuntime::RDOFRMFrame::RDOFRMPosition)>($10);
		$$ = (int)new rdoRuntime::RDOFRMActive(RUNTIME->lastFrame(), x, y, width, height, opr_name);
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		PARSER->error().error(@10, _T("��������� ']'"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		PARSER->error().error(@9, @10, _T("��������� ������"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		PARSER->error().error(@8, _T("��������� �������"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@7, @8, _T("��������� ������"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@6, _T("��������� �������"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' error
	{
		PARSER->error().error(@5, @6, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy error
	{
		PARSER->error().error(@4, _T("��������� �������"));
	}
	| RDO_active RDO_IDENTIF '[' error
	{
		PARSER->error().error(@3, @4, _T("��������� ���������� �� ��� X"));
	}
	| RDO_active RDO_IDENTIF error
	{
		PARSER->error().error(@2, _T("��������� '['"));
	}
	| RDO_active error
	{
		PARSER->error().error(@1, _T("��������� ��� ������������ ��������"));
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
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1), RDOVALUE($3))); }
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
		info.setSrcPos(@1, @2);
		$$ = PARSER->stack().push(pArithm->uminus(info.src_pos()));
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
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(RDOParserSrcInfo(@2, _T("NoCheck")));
		ASSERT(pTrueLogic);
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
		RDOParserSrcInfo info(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + info.src_text() + _T(")"));
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(info);
		ASSERT(pTrueLogic);
		pSelect->initSelect(pTrueLogic);
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
