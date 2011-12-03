/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDORTP_H_
#define _RDORTP_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/type/type.h"
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  rtpparse(PTR(void) lexer);
int  rtplex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void rtperror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);
PREDECLARE_POINTER(RDORSSResource);

class RDORTPResType:
	    INSTANCE_OF(RDOParserSrcInfo  )
	AND INSTANCE_OF(boost::noncopyable)
	AND INSTANCE_OF(RDOType           )
{
DECLARE_FACTORY(RDORTPResType);
public:
	typedef std::vector<LPRDORTPParam> ParamList;

	enum { UNDEFINED_PARAM = ~0 };

	rsint getNumber  () const   { return m_number;     };
	rbool isPermanent() const   { return m_permanent;  };
	rbool isTemporary() const   { return !m_permanent; };

	LPRDORSSResource createRes(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info);

	void addParam(CREF(LPRDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID);
	LPRDORTPParam findRTPParam(CREF(tstring) paramName) const;

	ruint           getRTPParamNumber(CREF(tstring) paramName) const;
	CREF(ParamList) getParams        ()                        const { return m_params;          }

	CREF(rdoRuntime::LPIResourceType) getRuntimeResType() const      { return m_pRuntimeResType; }

	template<class T>
	void end()
	{
		rdo::intrusive_ptr<T> pT = rdo::Factory<T>::create(m_number);
		m_pRuntimeResType = pT.template interface_cast<rdoRuntime::IResourceType>();
		ASSERT(m_pRuntimeResType);
	}

	void writeModelStructure(REF(std::ostream) stream) const;

	DECLARE_IType;

private:
	RDORTPResType(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent);
	virtual ~RDORTPResType();

	rdoRuntime::LPIResourceType m_pRuntimeResType;
	const ruint                 m_number;
	const rbool                 m_permanent;
	ParamList                   m_params;
};
DECLARE_POINTER(RDORTPResType);

//// --------------------------------------------------------------------------------
////------------------------------ FOR FUZZY LOGIC ------------------------------	
//// --------------------------------------------------------------------------------
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyMembershiftPoint - ����� �-�� �������������� ��������� �����
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyMembershiftPoint: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyMembershiftPoint(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, double x_value, double y_value):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_x_value(x_value),
//		m_y_value(y_value)
//		{
//		}
//	virtual ~RDORTPFuzzyMembershiftPoint() {}
//
//	double  getX() const { return m_x_value; }
//	double  getY() const { return m_y_value; }
//
//private:
//	double    m_x_value;
//	double    m_y_value;
//};
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyMembershiftFun - �-�� �������������� ��� ��������� �����
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyMembershiftFun: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyMembershiftFun(CREF(LPRDOParser) pParser):
//		RDOParserObject(pParser)
//		{
//		}
//	virtual ~RDORTPFuzzyMembershiftFun() {}
//
//	typedef PTR(RDORTPFuzzyMembershiftPoint) Item;
//	typedef std::vector< Item >              Items;
//
//	void add(Item point)
//	{
//		m_points.push_back(point);
//	}
//	double  getVal() const 
//	{ 
//		return m_value; 
//	}
//
//private:
//	Items m_points;	// �����, ������������ �-�� ��������������
//	double m_value;	// �������� �-�� �������������� ��� ����������� ������� ��������
//};
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyTerm - �������� ������
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyTerm: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyTerm(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_fun (pMembersfift_fun)
//		{
//		}
//	virtual ~RDORTPFuzzyTerm() {}
//
//	CREF(tstring) name       () const { return src_info().src_text(); }
//	double        MemberShift() const { return m_fun->getVal();       }
//
//private:
//	PTR(RDORTPFuzzyMembershiftFun) m_fun;
//};
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyTermsSet - ����� �������� ������ ���������
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyTermsSet(CREF(LPRDOParser) pParser)
//		: RDOParserObject(pParser)
//	{
//	}
//	virtual ~RDORTPFuzzyTermsSet() {}
//
//	typedef PTR(RDORTPFuzzyTerm) Item;
//	typedef std::vector<Item>    Items;
//
//	void add(Item term)
//	{
//		m_terms.push_back(term);
//	}
//	rbool empty() const
//	{
//		return m_terms.empty();
//	}
//
//private:
//	Items m_terms; // ����� �������� ������ ���������
//};
//
//// --------------------------------------------------------------------------------
//// -------------------- RDORTPFuzzyParam
//// --------------------------------------------------------------------------------
//class RDORTPFuzzyParam : public RDOParserObject, public RDOParserSrcInfo
//{
//public:
//	RDORTPFuzzyParam(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyTermsSet) terms_set):
//		RDOParserObject(pParser),
//		RDOParserSrcInfo(src_info),
//		m_set (terms_set)
//		{
//		}
//	virtual ~RDORTPFuzzyParam() {}
//
//	CREF(tstring) name() const { return src_info().src_text(); }
//
//private:
//	PTR(RDORTPFuzzyTermsSet) m_set; // ����� �������� ���������
//};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORTP_H_
