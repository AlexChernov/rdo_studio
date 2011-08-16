/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \indent    4T
  \brief     �����������
*/

#ifndef _LIB_RUNTIME_CALC_H_
#define _LIB_RUNTIME_CALC_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class    RDOCalc
  \brief    ����������� "�����������", �� �� ����.
  \details  C��������������� ������ ������� �� ������������������� ������, ��� �� ��� � ���������������� ��������� ������� �� ��������������� ������. ������ ����� ��������� �������, ����������� ����.
*/
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
//! \details ������� ��� ��������������� �������� �����
DECLARE_FACTORY(RDOCalc)

public:
	//! ������ �������� �����. �������� ��������� �������, ����� ������� � ����� ������������� ��������
	//! \param pRuntime - ��������� �� ������ runtime'�.
	//!                   ������������ ��� ������� � �� ������, ���������� �������, ��������� ������ � ��.
	//! \result ����������� ������ ��������
	REF(RDOValue) calcValue(CREF(LPRDORuntime) pRuntime);

	//! ��������� ���� ������ ��� �������� (���������� ������ � ��������� �� ������������).
	//! ������������� ���� ����� � ����� ������ �������������.
	//! \param pCalc - ����, � ������� ��������� ��������� �������
	//! \result true, ���� ����� ����������. �������� �� ��������� false
	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	//! ������� ������ ������� ��������� �������� ����� � ���������� ��� � \ref doCalc
	RDOValue m_value;

	//! ����������� �������� �����.
	//! ����� ������ ���� ���������� � �������� (�������� ������).
	//! \param pRuntime - ��������� �� ������ runtime'�.
	//!                   ������������ ��� ������� � �� ������, ���������� �������, ��������� ������ � ��.
	//! \exception RDORuntimeException
	//! \result ����������� ������ ��������
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime) = 0;
};

/******************************************************************************//**
 * \def     DECALRE_ICalc
 * \brief   ���������� ���������� ICalc
 *********************************************************************************/
#define DECALRE_ICalc \
private:              \
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

/******************************************************************************//**
 * \def     CALC_SUB
 * \brief   �����-�������
 *********************************************************************************/
#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

/******************************************************************************//**
 * \def     CALC
 * \brief   �����-������� �� RDOCalc
 *********************************************************************************/
#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

/******************************************************************************//**
 * \class   RDOCalcConst
 * \brief   ���������
 *********************************************************************************/
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	CREF(RDOValue) getValue() const;

private:
	RDOCalcConst(CREF(RDOValue) value);
	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetResParam
 * \brief   ��������� ����������� �������
 *********************************************************************************/
CALC(RDOCalcGetResParam)
{
DECLARE_FACTORY(RDOCalcGetResParam)
protected:
	RDOCalcGetResParam(int _resNumb, int _parNumb);

	int m_resID;
	int m_paramID;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetTempResParamFRM
 * \brief   ��������� ���������� ������� ��� FRM
 *********************************************************************************/
CALC_SUB(RDOCalcGetTempResParamFRM, RDOCalcGetResParam)
	AND IMPLEMENTATION_OF(INotify)
{
DECLARE_FACTORY(RDOCalcGetTempResParamFRM)
private:
	RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb);
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	virtual void notify(ruint message, PTR(void) param);
};

/******************************************************************************//**
 * \class   RDOCalcGetUnknowResParam
 * \brief   ��������� ��������������� �������
 *********************************************************************************/
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetGroupResParam
 * \brief   ��������� ������ ���������� ��������
 * \todo    ��� ���?
 *********************************************************************************/
CALC(RDOCalcGetGroupResParam)
{
DECLARE_FACTORY(RDOCalcGetGroupResParam)
private:
	RDOCalcGetGroupResParam(int parNumb);

	int m_parNumb;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOSetResourceParamCalc
 * \brief   ���������� ����� ��� ������� ��������
 *********************************************************************************/
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_resNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOSetPatternParamCalc
 * \brief   ���������� ����� ��� ��������� ��������
 *********************************************************************************/
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(int parNumb, RDOValue val);

	int      m_parNumb;
	RDOValue m_val;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcPatParam
 * \brief   �������� ��������
 *********************************************************************************/
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(int numberOfParam);

	int m_numberOfParam;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetTimeNow
 * \brief   ��������� �������� ���������� �������
 *********************************************************************************/
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetSeconds
 * \brief   ��������� ��������� ������� ������ ������
 *********************************************************************************/
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetTermNow
 * \brief   ��������� �������� ������������� ��������
 *********************************************************************************/
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunCalc
 * \brief   �������
 *********************************************************************************/
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

/******************************************************************************//**
 * \class   RDOFuncTableCalc
 * \brief   ������� ���� �������
 *********************************************************************************/
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) res);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc);

	std::vector<LPRDOCalcConst> m_results;
	LPRDOCalc                   m_pArgCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunListCalc
 * \brief   ������� ���� ������
 *********************************************************************************/
CALC_SUB(RDOFunListCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCaseCalc, CREF(LPRDOCalcConst) pResultCalc);

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue);

	std::vector<LPRDOCalc>       m_cases;
	std::vector<LPRDOCalcConst>  m_results;
	LPRDOCalcConst               m_pDefaultValue;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunAlgorithmicCalc
 * \brief   ��������������� �������
 *********************************************************************************/
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct);

protected:
	RDOFunAlgorithmicCalc();

	std::vector<LPRDOCalc> m_conditions;
	std::vector<LPRDOCalc> m_actions;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunAlgorithmicDiapCalc
 * \brief   ��������������� ������� � ��������� �� �������� ������������� ��������
 *********************************************************************************/
CALC_SUB(RDOFunAlgorithmicDiapCalc, RDOFunAlgorithmicCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value);

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunCalcGroup
 * \brief   ������� ����� ��� ��������� �����
 *********************************************************************************/
CALC_SUB(RDOFunCalcGroup, RDOFunCalc)
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition);
};

/******************************************************************************//**
 * \def     DEFINE_CALC_GROUP
 * \brief   ��������� �����
 *********************************************************************************/
#define DEFINE_CALC_GROUP(CalcName) \
CALC_SUB(RDOFunCalc##CalcName, RDOFunCalcGroup) \
{ \
DECLARE_FACTORY(RDOFunCalc##CalcName) \
private: \
	RDOFunCalc##CalcName(int nResType, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcGroup(nResType, pCondition) \
	{} \
 	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime); \
};

/******************************************************************************//**
 * \class   RDOFunCalcExist
 * \brief   ���������� �������� �������������
 *********************************************************************************/
DEFINE_CALC_GROUP(Exist);

/******************************************************************************//**
 * \class   RDOFunCalcNotExist
 * \brief   ���������� �������� ��������� �������������
 *********************************************************************************/
DEFINE_CALC_GROUP(NotExist);

/******************************************************************************//**
 * \class   RDOFunCalcForAll
 * \brief   ���������� �������� ��������
 *********************************************************************************/
DEFINE_CALC_GROUP(ForAll);

/******************************************************************************//**
 * \class   RDOFunCalcNotForAll
 * \brief   ���������� �������� ��������� ��������
 *********************************************************************************/
DEFINE_CALC_GROUP(NotForAll);

class RDOResource;

/******************************************************************************//**
 * \class   RDOFunCalcSelect
 * \brief   ������� ��������� �������
 *********************************************************************************/
CALC_SUB(RDOFunCalcSelect, RDOFunCalcGroup)
{
DECLARE_FACTORY(RDOFunCalcSelect)
public:
	mutable std::list<LPRDOResource> res_list;
	void prepare(CREF(LPRDORuntime) pRuntime);

private:
	RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition);

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOFunCalcSelectBase
 * \brief   ������� ����� ��� ���������� Select
 *********************************************************************************/
CALC_SUB(RDOFunCalcSelectBase, RDOFunCalc)
{
protected:
	RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition);

	LPRDOFunCalcSelect m_pSelect;
	LPRDOCalc          m_pCondition;
};

/******************************************************************************//**
 * \def     DEFINE_CALC_SELECT_GROUP
 * \brief   Select-������
 *********************************************************************************/
#define DEFINE_CALC_SELECT_GROUP(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition) \
	{} \
	DECALRE_ICalc; \
};

/******************************************************************************//**
 * \def     DEFINE_CALC_SELECT_METHOD
 * \brief   Select-�����
 *********************************************************************************/
#define DEFINE_CALC_SELECT_METHOD(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL) \
	{} \
	DECALRE_ICalc; \
};

/******************************************************************************//**
 * \class   RDOFunCalcSelectExist
 * \brief   ������� �������������
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP(Exist);

/******************************************************************************//**
 * \class   RDOFunCalcSelectNotExist
 * \brief   �������� ������� �������������
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (NotExist );

/******************************************************************************//**
 * \class   RDOFunCalcSelectForAll
 * \brief   �������� ������� ��� ������� �������� ���������
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (ForAll   );

/******************************************************************************//**
 * \class   RDOFunCalcSelectNotForAll
 * \brief   �������� �������� ������� ��� ������� �������� ���������
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (NotForAll);

/******************************************************************************//**
 * \class   RDOFunCalcSelectEmpty
 * \brief   �������� ��������� �� �������
 *********************************************************************************/
DEFINE_CALC_SELECT_METHOD(Empty    );

/******************************************************************************//**
 * \class   RDOFunCalcSelectSize
 * \brief   ������ ���������
 *********************************************************************************/
DEFINE_CALC_SELECT_METHOD(Size     );

/******************************************************************************//**
 * \class   RDOCalcFuncParam
 * \brief   �������� �������
 *********************************************************************************/
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info);

	int m_param_number;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcGetConst
 * \brief   ��������� ��������
 *********************************************************************************/
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(int number);

	int m_number;
	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcSetConst
 * \brief   ��������� ���������
 *********************************************************************************/
CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc);

	int        m_number;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcFunctionCall
 * \brief   ����� �������
 *********************************************************************************/
CALC(RDOCalcFunctionCall)
{
DECLARE_FACTORY(RDOCalcFunctionCall)
public:
	void addParameter(CREF(LPRDOCalc) pCalc);
	void setFunctionCalc(CREF(LPRDOFunCalc) pFunction);

private:
	RDOCalcFunctionCall(CREF(LPRDOFunCalc) pFunction);

	std::vector<LPRDOCalc>  m_parameters;
	LPRDOFunCalc            m_pFunction;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcCreateNumberedResource
 * \brief   �������� ��� rdo_converter
 *********************************************************************************/
CALC(RDOCalcCreateNumberedResource)
{
DECLARE_FACTORY(RDOCalcCreateNumberedResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(CREF(LPRDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	int                    m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcCreateProcessResource
 * \brief   �������� ��� rdo_converter
 *********************************************************************************/
CALC(RDOCalcCreateProcessResource)
{
DECLARE_FACTORY(RDOCalcCreateProcessResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(CREF(LPRDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateProcessResource(rsint _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	rsint                  m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * \class   RDOCalcCreateResource
 * \brief   �������� ������ �������
 *********************************************************************************/
CALC(RDOCalcCreateResource)
{
DECLARE_FACTORY(RDOCalcCreateResource)
private:
	//! relResID == 0 ��� ��������, ����������� ��� ������������� ������
	RDOCalcCreateResource(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID = 0);

	LPIResourceType        m_pResType;
	std::vector<RDOValue>  m_paramsCalcs;
	rbool                  m_traceFlag;
	rbool                  m_permanentFlag;
	ruint                  m_relResID;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdocalc.inl"

#endif // _LIB_RUNTIME_CALC_H_
