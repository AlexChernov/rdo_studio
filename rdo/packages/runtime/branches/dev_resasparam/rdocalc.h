/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     �����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_H_
#define _LIB_RUNTIME_CALC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/smart_ptr/factory.h"
#include "utils/smart_ptr/interface_ptr.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_memory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

//! \defgroup calc      ����� ������ ������
//! \defgroup calc_base ����������� ����

/*!
  \class    RDOCalc
  \brief    ����������� "�����������", �� �� ����.
  \details  C��������������� ������ ������� �� ������������������� ������, ��� �� ��� � ���������������� ��������� ������� �� ��������������� ������. ������ ����� ��������� �������, ����������� ����.
  \ingroup  calc calc_base
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

#define DECLARE_ICalc \
private:              \
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

//! \def    CALC_SUB
//! \brief  ��������� �����-�������
#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

//! \def    CALC
//! \brief  ��������� �����-������� �� RDOCalc
#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

/*!
  \defgroup calc_const ����������� ����
  \class    RDOCalcConst
  \brief    ���������
  \details  ���������� ����������� ��������
  \ingroup  calc calc_const
*/
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	//! ���������� �������� ���������, ����� ���� ����������� � parser-time
	//! \result �������� ���������
	CREF(RDOValue) getValue() const;

private:
	//! ���������������� ��������� ��������� ���������� �������� ������ \ref RDOCalc::m_value
	//! \param value - �������� ���������
	RDOCalcConst(CREF(RDOValue) value);

	//! ���������� �������� ���������
	//! \param pRuntime - �� ������������
	//! \result �������� ���������
	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetResParam
  \brief   ��������� ����������� �������
*/
CALC(RDOCalcGetResParam)
{
DECLARE_FACTORY(RDOCalcGetResParam)
protected:
	RDOCalcGetResParam(int _resNumb, int _parNumb);

	int m_resID;
	int m_paramID;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetResID
  \brief   ��������� ID ������� �� �����
*/
CALC(RDOCalcGetResID)
{
DECLARE_FACTORY(RDOCalcGetResID)

private:
	LPRDOCalc m_pCalcGetResource;
protected:
	RDOCalcGetResID(CREF(LPRDOCalc) pCalcGetResource);

	DECLARE_ICalc;
};


/*!
  \class   RDOCalcGetResParam
  \brief   ��������� ����������� ������� ��� ��������� �������
*/
/*CALC(RDOCalcGetResParam2)
{
DECLARE_FACTORY(RDOCalcGetResParam2)
protected:
	RDOCalcGetResParam(int _resNumb, int _parNumb);

	int m_resID;
	int m_paramID;

	DECLARE_ICalc;
};
*/
/*!
  \class   RDOCalcGetTempResParamFRM
  \brief   ��������� ���������� ������� ��� FRM
*/
CALC_SUB(RDOCalcGetTempResParamFRM, RDOCalcGetResParam)
	AND IMPLEMENTATION_OF(INotify)
{
DECLARE_FACTORY(RDOCalcGetTempResParamFRM)
private:
	RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb);
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	virtual void notify(ruint message, PTR(void) pParam);
};

/*!
  \class   RDOCalcGetUnknowResParam
  \brief   ��������� ��������������� �������
*/
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetGroupResParam
  \brief   ��������� ������ ���������� ��������
  \todo    ��� ���?
*/
CALC(RDOCalcGetGroupResParam)
{
DECLARE_FACTORY(RDOCalcGetGroupResParam)
private:
	RDOCalcGetGroupResParam(int parNumb);

	int m_parNumb;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetResourceParamCalc
  \brief   ���������� ����� ��� ������� ��������
*/
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_resNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetPatternParamCalc
  \brief   ���������� ����� ��� ��������� ��������
*/
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcPatParam
  \brief   �������� ��������
*/
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(int numberOfParam);

	int m_numberOfParam;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetTimeNow
  \brief   ��������� �������� ���������� �������
*/
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetSeconds
  \brief   ��������� ��������� ������� ������ ������
*/
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetTermNow
  \brief   ��������� �������� ������������� ��������
*/
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECLARE_ICalc;
};

/*!
  \class   RDOFunCalc
  \brief   �������
*/
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

/*!
  \class   RDOFuncTableCalc
  \brief   ������� ���� �������
*/
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) res);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc);

	std::vector<LPRDOCalcConst> m_results;
	LPRDOCalc                   m_pArgCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOFunListCalc
  \brief   ������� ���� ������
*/
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

	DECLARE_ICalc;
};

/*!
  \class   RDOFunAlgorithmicCalc
  \brief   ��������������� �������
*/
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct);

protected:
	RDOFunAlgorithmicCalc();

	std::vector<LPRDOCalc> m_conditions;
	std::vector<LPRDOCalc> m_actions;

	DECLARE_ICalc;
};

/*!
  \class   RDOFunAlgorithmicDiapCalc
  \brief   ��������������� ������� � ��������� �� �������� ������������� ��������
*/
CALC_SUB(RDOFunAlgorithmicDiapCalc, RDOFunAlgorithmicCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value);

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECLARE_ICalc;
};

/*!
/*!
/*!

/*!
  \class   RDOCalcFuncParam
  \brief   �������� �������
*/
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info);

	int m_param_number;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcGetConst
  \brief   ��������� ���������
*/
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(int number);

	int m_number;
	DECLARE_ICalc;
};

/*!
  \class   RDOCalcSetConst
  \brief   ��������� ���������
*/
CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc);

	int        m_number;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcFunctionCall
  \brief   ����� �������
*/
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

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdocalc.inl"

#endif // _LIB_RUNTIME_CALC_H_
