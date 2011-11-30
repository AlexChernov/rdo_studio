/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.y
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ������ ������ (kopninandrey@gmail.com)
  \date      20.02.2003
  \brief     ��������� ������ ��������
  \indent    4T
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
%token RDO_circle
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
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdofrm.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdocalc.h"
#include "utils/rdoanimation.h"
#include "simulator/runtime/calc/array.h"
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

OPEN_RDO_PARSER_NAMESPACE

typedef rdoRuntime::RDOFRMFrame::RDOFRMColor      RDOFRMColor;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMColor    LPRDOFRMColor;
typedef rdoRuntime::RDOFRMFrame::RDOFRMPosition   RDOFRMPosition;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMPosition LPRDOFRMPosition;
typedef rdoRuntime::RDOFRMFrame::RDOFRMRulet      RDOFRMRulet;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMRulet    LPRDOFRMRulet;
typedef rdoRuntime::RDOFRMText                    RDOFRMText;
typedef rdoRuntime::LPRDOFRMText                  LPRDOFRMText;
typedef rdoRuntime::RDOFRMBitmap                  RDOFRMBitmap;
typedef rdoRuntime::LPRDOFRMBitmap                LPRDOFRMBitmap;
typedef rdoRuntime::RDOFRMBitmapStretch           RDOFRMBitmapStretch;
typedef rdoRuntime::LPRDOFRMBitmapStretch         LPRDOFRMBitmapStretch;
typedef rdoRuntime::RDOFRMRect                    RDOFRMRect;
typedef rdoRuntime::LPRDOFRMRect                  LPRDOFRMRect;
typedef rdoRuntime::RDOFRMRectRound               RDOFRMRectRound;
typedef rdoRuntime::LPRDOFRMRectRound             LPRDOFRMRectRound;
typedef rdoRuntime::RDOFRMCircle                  RDOFRMCircle;
typedef rdoRuntime::LPRDOFRMCircle                LPRDOFRMCircle;
typedef rdoRuntime::RDOFRMEllipse                 RDOFRMEllipse;
typedef rdoRuntime::LPRDOFRMEllipse               LPRDOFRMEllipse;
typedef rdoRuntime::RDOFRMLine                    RDOFRMLine;
typedef rdoRuntime::LPRDOFRMLine                  LPRDOFRMLine;
typedef rdoRuntime::RDOFRMTriang                  RDOFRMTriang;
typedef rdoRuntime::LPRDOFRMTriang                LPRDOFRMTriang;
typedef rdoRuntime::RDOFRMActive                  RDOFRMActive;
typedef rdoRuntime::LPRDOFRMActive                LPRDOFRMActive;
typedef rdoRuntime::RDOFRMSpace                   RDOFRMSpace;
typedef rdoRuntime::LPRDOFRMSpace                 LPRDOFRMSpace;

%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start frm_main

%%

// --------------------------------------------------------------------------------
// -------------------- �������� ����
// --------------------------------------------------------------------------------
frm_main
	: /* empty */
	| frm_main frm_end
	| error
	{
		PARSER->error().error(@1, _T("����������� ������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- �����
// --------------------------------------------------------------------------------
frm_begin
	: RDO_Frame RDO_IDENTIF
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(PARSER->stack().pop<RDOValue>($2)->src_info());
		ASSERT(pFrame);
		$$ = PARSER->stack().push(pFrame);
	}
	| RDO_Frame RDO_IDENTIF RDO_Show_if fun_logic
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(PARSER->stack().pop<RDOValue>($2)->src_info(), PARSER->stack().pop<RDOFUNLogic>($4));
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
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($4);
		ASSERT(pBgColor);
		if (pBgColor->getType() != RDOFRMColor::CT_TRANSPARENT && pBgColor->getType() != RDOFRMColor::CT_RGB)
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
		pFrame->frame()->setBackPicture(PARSER->stack().pop<RDOValue>($2)->value().getIdentificator());
		$$ = PARSER->stack().push(pFrame);
	}
	| frm_background RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFRMFrame pFrame = PARSER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->frame()->setBackPicture(PARSER->stack().pop<RDOValue>($2)->value().getInt(), PARSER->stack().pop<RDOValue>($3)->value().getInt());
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
	| frm_item frm_text    {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMText         >($2));}
	| frm_item frm_bitmap  {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMBitmap       >($2));}
	| frm_item frm_rect    {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMRect         >($2));}
	| frm_item frm_line    {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMLine         >($2));}
	| frm_item frm_circle  {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMCircle       >($2));}
	| frm_item frm_ellipse {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMEllipse      >($2));}
	| frm_item frm_r_rect  {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMRectRound    >($2));}
	| frm_item frm_triang  {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMTriang       >($2));}
	| frm_item frm_s_bmp   {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMBitmapStretch>($2));}
	| frm_item frm_active  {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMActive       >($2));}
	| frm_item frm_ruler   {PARSER->getLastFRMFrame()->frame()->addRulet(PARSER->stack().pop<RDOFRMRulet        >($2));}
	| frm_item frm_space   {PARSER->getLastFRMFrame()->frame()->addItem (PARSER->stack().pop<RDOFRMSpace        >($2));}
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

// --------------------------------------------------------------------------------
// -------------------- ��������
// --------------------------------------------------------------------------------
frm_color
	: RDO_color_transparent
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(RDOFRMColor::CT_TRANSPARENT);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_last
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_white
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(255, 255, 255);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_black
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(0, 0, 0);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_red
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(255, 0, 0);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_green
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(0, 255, 0);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_blue
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(0, 0, 255);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_cyan
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(0, 255, 255);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_magenta
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(255, 0, 255);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_yellow
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(255, 255, 0);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| RDO_color_gray
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(127, 127, 127);
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
	}
	| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST '>'
	{
		LPRDOFUNArithm pRed   = RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($2));
		LPRDOFUNArithm pGreen = RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($3));
		LPRDOFUNArithm pBlue  = RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($4));
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @2), rdo::Factory<RDOValue>::create(255, @2), @2);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @2);
			pRed->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @3), rdo::Factory<RDOValue>::create(255, @3), @3);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @3);
			pGreen->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @4), rdo::Factory<RDOValue>::create(255, @4), @4);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @4);
			pBlue->checkParamType(pType);
		}
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
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
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @2), rdo::Factory<RDOValue>::create(255, @2), @2);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @2);
			pRed->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @4), rdo::Factory<RDOValue>::create(255, @4), @4);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @4);
			pGreen->checkParamType(pType);
		}
		{
			LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(rdo::Factory<RDOValue>::create(0, @6), rdo::Factory<RDOValue>::create(255, @6), @6);
			LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			LPTypeInfo          pType     = rdo::Factory<TypeInfo>::create(pIntRange, @6);
			pBlue->checkParamType(pType);
		}
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
		ASSERT(pColor);
		$$ = PARSER->stack().push(pColor);
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
		$$ = RDOFRMPosition::PT_ABSOLUTE;
	}
	| '+'
	{
		$$ = RDOFRMPosition::PT_DELTA;
	}
	| '*'
	{
		$$ = RDOFRMPosition::PT_MULT;
	}
	| '#' RDO_INT_CONST
	{
		int rilet_id = PARSER->stack().pop<RDOValue>($2)->value().getInt();
		if (rilet_id <= 0)
		{
			PARSER->error().error(@2, _T("����� ������� ������ ���� ������ ����"));
		}
		if (!RUNTIME->lastFrame()->findRulet(rilet_id))
		{
			PARSER->error().error(@2, rdo::format(_T("������� � ������� '%d' �� ����������"), rilet_id));
		}
		$$ = RDOFRMPosition::PT_RULET + rilet_id;
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
		$$ = RDOFRMPosition::PT_GABARIT;
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
		if ($2 >= RDOFRMPosition::PT_RULET)
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, RDOFRMPosition::PT_RULET, $2 - RDOFRMPosition::PT_RULET);
			ASSERT(pPosition);
			$$ = PARSER->stack().push(pPosition);
		}
		else
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, (RDOFRMPosition::PositionType)$2);
			ASSERT(pPosition);
			$$ = PARSER->stack().push(pPosition);
		}
	}
	;

frm_position_wh
	: fun_arithm frm_postype_wh
	{
		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<RDOFUNArithm>($1)->createCalc();
		if ($2 >= RDOFRMPosition::PT_RULET)
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, RDOFRMPosition::PT_RULET, $2 - RDOFRMPosition::PT_RULET);
			ASSERT(pPosition);
			$$ = PARSER->stack().push(pPosition);
		}
		else
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, (RDOFRMPosition::PositionType)$2);
			ASSERT(pPosition);
			$$ = PARSER->stack().push(pPosition);
		}
	}
	;

frm_radius
	: fun_arithm frm_postype_xy
	{
		if ($2 != rdoRuntime::RDOFRMFrame::RDOFRMPosition::PT_ABSOLUTE &&
		    $2 != rdoRuntime::RDOFRMFrame::RDOFRMPosition::PT_DELTA    &&
		    $2 != rdoRuntime::RDOFRMFrame::RDOFRMPosition::PT_MULT
		)
		{
			PARSER->error().error(@2, _T("������ ������������ ������ ������������ ��� �������"));
		}

		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<RDOFUNArithm>($1)->createCalc();
		ASSERT(pCalc);

		LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, (RDOFRMPosition::PositionType)$2);
		ASSERT(pPosition);
		$$ = PARSER->stack().push(pPosition);
	};

frm_ruler
	: RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy ']'
	{
		LPRDOValue pRuletID = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pRuletID);
		LPRDOFRMRulet pRulet = RUNTIME->lastFrame()->findRulet(pRuletID->value().getInt());
		if (pRulet)
		{
			PARSER->error().push_only(@3, rdo::format(_T("������� � ������� '%d' ��� ����������"), pRuletID->value().getInt()));
			PARSER->error().push_only(pRulet->src_info(), _T("��. ������ �����������"));
			PARSER->error().push_done();
		}
		LPRDOFRMPosition pX = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pY = PARSER->stack().pop<RDOFRMPosition>($7);
		ASSERT(pX);
		ASSERT(pY);
		if (pX->getType() != RDOFRMPosition::PT_ABSOLUTE)
		{
			PARSER->error().error(@5, _T("���������� ������� ������ ���� �����������"));
		}
		if (pY->getType() != RDOFRMPosition::PT_ABSOLUTE)
		{
			PARSER->error().error(@7, _T("���������� ������� ������ ���� �����������"));
		}
		pRulet = rdo::Factory<RDOFRMRulet>::create(RDOParserSrcInfo(@1), pRuletID->value().getInt(), pX, pY);
		ASSERT(pRulet);
		$$ = PARSER->stack().push(pRulet);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMSpace pSpace = rdo::Factory<RDOFRMSpace>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight);
		ASSERT(pSpace);
		$$ = PARSER->stack().push(pSpace);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = PARSER->stack().push(pText);
	}
/*
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ','
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdp::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($11);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = PARSER->stack().push(pText);
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ','
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdp::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdp::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = PARSER->stack().push(pText);
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
		LPRDOFRMText pText = PARSER->stack().pop<RDOFRMText>($1);
		ASSERT(pText);
		pText->setText((rdoAnimation::RDOTextElement::TextAlign)$2, PARSER->stack().pop<RDOFUNArithm>($3)->createCalc());
		$$ = PARSER->stack().push(pText);
	}
	| frm_text_common frm_text_align RDO_STRING_CONST ']'
	{
		LPRDOFRMText pText = PARSER->stack().pop<RDOFRMText>($1);
		ASSERT(pText);
		pText->setText((rdoAnimation::RDOTextElement::TextAlign)$2, PARSER->stack().pop<RDOValue>($3)->value().getString());
		$$ = PARSER->stack().push(pText);
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
		LPRDOFRMPosition pX = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY = PARSER->stack().pop<RDOFRMPosition>($5);
		ASSERT(pX);
		ASSERT(pY);
		LPRDOFRMBitmap pBitmap = rdo::Factory<RDOFRMBitmap>::create(RUNTIME->lastFrame(), pX, pY, PARSER->stack().pop<RDOValue>($7)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = PARSER->stack().push(pBitmap);
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY = PARSER->stack().pop<RDOFRMPosition>($5);
		ASSERT(pX);
		ASSERT(pY);
		LPRDOFRMBitmap pBitmap = rdo::Factory<RDOFRMBitmap>::create(RUNTIME->lastFrame(), pX, pY, PARSER->stack().pop<RDOValue>($7)->value().getIdentificator(), PARSER->stack().pop<RDOValue>($9)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = PARSER->stack().push(pBitmap);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMBitmapStretch pBitmap = rdo::Factory<RDOFRMBitmapStretch>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, PARSER->stack().pop<RDOValue>($11)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = PARSER->stack().push(pBitmap);
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMBitmapStretch pBitmap = rdo::Factory<RDOFRMBitmapStretch>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, PARSER->stack().pop<RDOValue>($11)->value().getIdentificator(), PARSER->stack().pop<RDOValue>($13)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = PARSER->stack().push(pBitmap);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = PARSER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = PARSER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = PARSER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = PARSER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		LPRDOFRMPosition pHeight = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = PARSER->stack().push(pRect);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRectRound pRoundRect = rdo::Factory<RDOFRMRectRound>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRoundRect);
		$$ = PARSER->stack().push(pRoundRect);
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

frm_circle
	: RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pRadius = PARSER->stack().pop<RDOFRMPosition>($7);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pRadius);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($9);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($11);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMCircle pCircle = rdo::Factory<RDOFRMCircle>::create(RUNTIME->lastFrame(), pX, pY, pRadius, pBgColor, pFgColor);
		ASSERT(pCircle);
		$$ = PARSER->stack().push(pCircle);
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius ',' frm_color ',' frm_color error
	{
		PARSER->error().error(@11, _T("��������� ']'"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius ',' frm_color ',' error
	{
		PARSER->error().error(@10, @11, _T("��������� ���� ����� �����"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius ',' frm_color error
	{
		PARSER->error().error(@9, _T("��������� �������"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius ',' error
	{
		PARSER->error().error(@8, @9, _T("��������� ���� ����"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' frm_radius error
	{
		PARSER->error().error(@7, _T("��������� �������"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy ',' error
	{
		PARSER->error().error(@6, @7, _T("��������� ������"));
	}
	| RDO_circle '[' frm_position_xy ',' frm_position_xy error
	{
		PARSER->error().error(@5, _T("��������� �������"));
	}
	| RDO_circle '[' frm_position_xy ',' error
	{
		PARSER->error().error(@4, @5, _T("��������� ���������� �� ��� Y"));
	}
	| RDO_circle '[' frm_position_xy error
	{
		PARSER->error().error(@3, _T("��������� �������"));
	}
	| RDO_circle '[' error
	{
		PARSER->error().error(@2, @3, _T("��������� ���������� �� ��� X"));
	}
	| RDO_circle error
	{
		PARSER->error().error(@1, _T("��������� '['"));
	};

frm_ellipse
	: RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMEllipse pEllipse = rdo::Factory<RDOFRMEllipse>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pEllipse);
		$$ = PARSER->stack().push(pEllipse);
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pColor = PARSER->stack().pop<RDOFRMColor>($11);
		ASSERT(pColor);
		pColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMLine pLine = rdo::Factory<RDOFRMLine>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pColor);
		ASSERT(pLine);
		$$ = PARSER->stack().push(pLine);
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
		LPRDOFRMPosition pX1 = PARSER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY1 = PARSER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pX2 = PARSER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pY2 = PARSER->stack().pop<RDOFRMPosition>($9);
		LPRDOFRMPosition pX3 = PARSER->stack().pop<RDOFRMPosition>($11);
		LPRDOFRMPosition pY3 = PARSER->stack().pop<RDOFRMPosition>($13);
		ASSERT(pX1);
		ASSERT(pY1);
		ASSERT(pX2);
		ASSERT(pY2);
		ASSERT(pX3);
		ASSERT(pY3);
		LPRDOFRMColor pBgColor = PARSER->stack().pop<RDOFRMColor>($15);
		LPRDOFRMColor pFgColor = PARSER->stack().pop<RDOFRMColor>($17);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMTriang pTriang = rdo::Factory<RDOFRMTriang>::create(RUNTIME->lastFrame(), pX1, pY1, pX2, pY2, pX3, pY3, pBgColor, pFgColor);
		ASSERT(pTriang);
		$$ = PARSER->stack().push(pTriang);
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
		tstring oprName = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
		LPRDODPTActivity pActivity = PARSER->findDPTActivity(oprName);
		if (!pActivity)
		{
			PARSER->error().error(@2, rdo::format(_T("���������� '%s' �� �������"), oprName.c_str()));
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
		LPRDOFRMPosition pX      = PARSER->stack().pop<RDOFRMPosition>($4);
		LPRDOFRMPosition pY      = PARSER->stack().pop<RDOFRMPosition>($6);
		LPRDOFRMPosition pWidth  = PARSER->stack().pop<RDOFRMPosition>($8);
		LPRDOFRMPosition pHeight = PARSER->stack().pop<RDOFRMPosition>($10);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMActive pActive = rdo::Factory<RDOFRMActive>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, oprName);
		ASSERT(pActive);
		$$ = PARSER->stack().push(pActive);
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

// --------------------------------------------------------------------------------
// -------------------- ����� ��������� ������ ��� ���� �������� ���
// --------------------------------------------------------------------------------
// -------------------- �������� ����������
// --------------------------------------------------------------------------------
param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST
	{
		$$ = $1;
	}
	| RDO_STRING_CONST
	{
		$$ = $1;
	}
	| RDO_IDENTIF
	{
		$$ = $1;
	}
	| RDO_BOOL_CONST
	{
		$$ = $1;
	}
	| param_array_value
	{
		$$ = $1;
	}
	;

param_array_value
	: '[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		RDOParserSrcInfo srcInfo(@1, @3, pArrayValue->getAsString());
		pArrayValue->setSrcInfo(srcInfo);
		pArrayValue->getArrayType()->setSrcInfo(srcInfo);
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create(pArrayValue->getArrayType()->typeInfo(), pArrayValue));
	}
	| '[' array_item error
	{
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(pValue->typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), pValue->value().getAsString().c_str()));
	}
	;

param_value_default
	: /* empty */
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create();
		ASSERT(pValue);
		$$ = PARSER->stack().push(pValue);
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

// --------------------------------------------------------------------------------
// -------------------- ���������� ���������
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- �������������� ���������
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| param_array_value                  { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
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
	| RDO_IDENTIF '.' RDO_Size
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		rdoRuntime::LPRDOCalc pCalc;
		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			pCalc = rdo::Factory<rdoRuntime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArraySize = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArraySize);

		$$ = PARSER->stack().push(pArithmArraySize);
	}
	| RDO_IDENTIF '[' fun_arithm ']'
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		LPRDOFUNArithm pArithmInd = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithmInd);

		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), pValue->value().getIdentificator().c_str()));
		}

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcArrayItem>::create(pArithm->calc(), pArithmInd->calc());
		ASSERT(pCalc);

		LPRDOType pType = pArithm->typeInfo()->type();
		ASSERT(pType);
		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		ASSERT(pArrayType);

		LPTypeInfo pItemType = pArrayType->getItemType();
		ASSERT(pItemType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pItemType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArrayItem = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArrayItem);

		$$ = PARSER->stack().push(pArithmArrayItem);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ������� � ������������������
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' arithm_list ')'
	{
		tstring funName                    = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($3);
		ASSERT(pArithmContainer);

		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create(pArithmContainer);
		ASSERT(pFunParams);

		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pArithmContainer->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

arithm_list
	: /* empty */
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		ASSERT(pArithmContainer);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body
	{};

arithm_list_body
	: fun_arithm
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' fun_arithm
	{
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($1);
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithmContainer->src_text() + _T(", ") + pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ��������� ���������
// --------------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, pValue->src_info()));
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

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(pValue->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + pValue->value().getIdentificator() + _T(": "));
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
