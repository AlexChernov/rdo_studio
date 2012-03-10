/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     �������� ������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_FUZZY_H_
#define _LIB_RUNTIME_FUZZY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFuzzyType);
PREDECLARE_POINTER(RDOFuzzyValue);

//! �������� ��������
OBJECT(RDOFuzzyValue)
{
DECLARE_FACTORY(RDOFuzzyValue);
public:
	typedef  std::pair<RDOValue, double>                              FuzzyItem;
	typedef  std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySet;

	REF(RDOFuzzyValue)         append     (CREF(RDOValue) rdovalue, double appertain);
	REF(RDOFuzzyValue)         operator() (CREF(RDOValue) rdovalue, double appertain);
	REF(double)                operator[] (CREF(RDOValue) rdovalue);
	FuzzySet::const_iterator   find       (CREF(RDOValue) rdovalue) const;
	FuzzyItem                  findValue  (CREF(RDOValue) rdovalue) const;
	FuzzySet::const_iterator   begin      () const;
	FuzzySet::const_iterator   end        () const;
	rbool                      empty      () const;
	CREF(LPRDOFuzzyType)       type       () const;
	LPRDOFuzzyValue            clone      () const;

	/* 3.37 */  LPRDOFuzzyValue operator&& (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.40 */  LPRDOFuzzyValue operator|| (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.102*/  LPRDOFuzzyValue operator+  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.104*/  LPRDOFuzzyValue operator-  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.106*/  LPRDOFuzzyValue operator*  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.108*/  LPRDOFuzzyValue operator/  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;

	/* 3.116*/  LPRDOFuzzyValue u_minus() const;
	/* 3.117*/  LPRDOFuzzyValue u_obr  () const;
	/* 3.118*/  LPRDOFuzzyValue u_scale(double scale) const;
	/* 3.119*/  LPRDOFuzzyValue u_log  () const;

	/* 3.39 */  LPRDOFuzzyValue a_mult    (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.48 */  LPRDOFuzzyValue alpha     (double appertain) const;
	/* 3.62 */  LPRDOFuzzyValue supplement() const;
	/* 3.78 */  LPRDOFuzzyValue a_con     () const;
	/* 3.79 */  LPRDOFuzzyValue a_dil     () const;

	/* 3.272*/ RDOValue defuzzyfication();

	tstring getAsString() const;

private:
	RDOFuzzyValue(CREF(LPRDOFuzzyType)  pType );
	RDOFuzzyValue(CREF(LPRDOFuzzyValue) pValue);
	virtual ~RDOFuzzyValue();

	FuzzySet        m_fuzzySet;
	LPRDOFuzzyType  m_pType;

	FuzzySet::iterator  begin();
	FuzzySet::iterator  end  ();

	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) rdovalue);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) rdovalue, PTR(void) pParam);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) rdovalue1, CREF(RDOValue) rdovalue2);

	            LPRDOFuzzyValue a_pow     (double power) const;
	/* 3.114*/  LPRDOFuzzyValue ext_unary (ExtUnaryFun  fun) const;
	/* 3.114*/  LPRDOFuzzyValue ext_unary (ExtUnaryFunP fun, PTR(void) pParam) const;
	/* 3.83 */  LPRDOFuzzyValue ext_binary(ExtBinaryFun fun, CREF(LPRDOFuzzyValue) pFuzzyValue) const;
};

PREDECLARE_POINTER(RDOFuzzySetDefinition);
PREDECLARE_POINTER(RDOActivatedValue);
PREDECLARE_POINTER(RDOValue);

//! ��� �������� ����������
class RDOFuzzyType: public RDOType
{
DECLARE_FACTORY(RDOFuzzyType);
public:
	typedef std::map<tstring, LPRDOFuzzyValue> TermSet;

	TermSet::const_iterator begin     ();
	TermSet::const_iterator end       ();
	CREF(TermSet)         getTermSet  () const;
	virtual tstring       name      () const;
	virtual RDOValue      value_cast(CREF(RDOValue) from) const;

	rbool operator== (CREF(RDOFuzzyType) type) const;
	rbool operator!= (CREF(RDOFuzzyType) type) const;

	LPRDOActivatedValue fuzzyfication(CREF(RDOValue) rdovalue);

	rbool           inRange      (CREF(RDOValue)        rdovalue) const;
	LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const;

protected:
	RDOFuzzyType(CREF(LPRDOFuzzySetDefinition) pFuzzySetDefinition);
	virtual ~RDOFuzzyType();

private:
	TermSet                  m_termSet;
	LPRDOFuzzySetDefinition  m_fuzzySetDefinition;
};
DECLARE_POINTER(RDOFuzzyType);

OBJECT (RDOActivatedValue)
{
DECLARE_FACTORY(RDOActivatedValue)
public:
	typedef std::map<tstring,double>ActivatedValue;
	
	ActivatedValue::const_iterator begin ();
	ActivatedValue::const_iterator end   ();

	REF(RDOActivatedValue)         append(tstring term, double appertain);

private:
	RDOActivatedValue();

	ActivatedValue m_actValue;
	LPRDOFuzzyType m_pType;
};

//! �������� ���������
OBJECT(RDOFuzzySetDefinition)
{
DECLARE_FACTORY(RDOFuzzySetDefinition)
public:
	virtual rbool           inRange      (CREF(RDOValue)        rdovalue) const = 0;
	virtual LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const = 0;

protected:
	RDOFuzzySetDefinition();
	virtual ~RDOFuzzySetDefinition();
};

//! �������� ������������� ���������
class RDOFuzzySetDefinitionFixed: public RDOFuzzySetDefinition
{
DECLARE_FACTORY(RDOFuzzySetDefinitionFixed)
public:
	REF(RDOFuzzySetDefinitionFixed) append     (CREF(RDOValue) rdovalue);
	REF(RDOFuzzySetDefinitionFixed) operator() (CREF(RDOValue) rdovalue);

	virtual rbool           inRange      (CREF(RDOValue)        rdovalue) const;
	virtual LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const;

private:
	RDOFuzzySetDefinitionFixed();
	virtual ~RDOFuzzySetDefinitionFixed();

	RDOFuzzyValue::FuzzySet  m_items;
};
DECLARE_POINTER(RDOFuzzySetDefinitionFixed);

//! ���������� ��������� ����������
class RDOFuzzySetDefinitionRangeDiscret: public RDOFuzzySetDefinition
{
DECLARE_FACTORY(RDOFuzzySetDefinitionRangeDiscret)
public:
	virtual rbool           inRange      (CREF(RDOValue)        rdovalue) const;
	virtual LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const;

private:
	RDOFuzzySetDefinitionRangeDiscret(CREF(RDOValue) from, CREF(RDOValue) till, CREF(RDOValue) step = 1);
	virtual ~RDOFuzzySetDefinitionRangeDiscret();

	RDOValue  m_from;
	RDOValue  m_till;
	RDOValue  m_step;
};
DECLARE_POINTER(RDOFuzzySetDefinitionRangeDiscret);

//! ������ �������� ���
class RDOFuzzyEmptyType: public RDOFuzzyType
{
public:
	static LPRDOFuzzyType getInstance();

private:
	RDOFuzzyEmptyType();
	virtual ~RDOFuzzyEmptyType();

	static PTR(RDOFuzzyEmptyType) s_emptyType;

	//! ������ ���������
	class RDOFuzzySetDefinitionEmpty: public RDOFuzzySetDefinition
	{
	public:
		RDOFuzzySetDefinitionEmpty();
		virtual ~RDOFuzzySetDefinitionEmpty();

		virtual rbool           inRange      (CREF(RDOValue)        rdovalue) const;
		virtual LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const;
	};

	virtual tstring asString() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_fuzzy.inl"

#endif // _LIB_RUNTIME_FUZZY_H_
