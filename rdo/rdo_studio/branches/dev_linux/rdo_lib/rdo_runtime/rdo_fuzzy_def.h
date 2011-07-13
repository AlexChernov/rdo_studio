#ifndef RDO_FUZZY_DEF_H
#define RDO_FUZZY_DEF_H

#ifndef _RDO_VALUE_H_
#error include "rdo_value.h" first (������ "rdo_fuzzy_def.h" ���� ��������� "rdo_fuzzy.h")
#endif

//
// �������� ���� ���� ��������� �� ����, ������
// #include "rdo_fuzzy.h"
//

#include <map>
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFuzzyType);

class RDOFuzzyValue
{
public:
	typedef std::pair<RDOValue, double>                              FuzzyItem;
	typedef std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySet;

	RDOFuzzyValue(CREF(LPRDOFuzzyType) pType);
	RDOFuzzyValue(CREF(RDOFuzzyValue)  value);
	~RDOFuzzyValue();

	REF(RDOFuzzyValue)         append     (CREF(RDOValue) rdovalue, double appertain);
	REF(RDOFuzzyValue)         operator() (CREF(RDOValue) rdovalue, double appertain);
	REF(double)                operator[] (CREF(RDOValue) rdovalue);
	FuzzySet::const_iterator   find       (CREF(RDOValue) rdovalue) const;
	FuzzyItem                  findValue  (CREF(RDOValue) rdovalue) const;
	FuzzySet::const_iterator   begin      () const;
	FuzzySet::const_iterator   end        () const;
	rbool                      empty      () const;
	CREF(LPRDOFuzzyType)       type       () const;

	/* 3.37  */  RDOFuzzyValue operator&& (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.40  */  RDOFuzzyValue operator|| (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.102 */  RDOFuzzyValue operator+  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.104 */  RDOFuzzyValue operator-  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.106 */  RDOFuzzyValue operator*  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.108 */  RDOFuzzyValue operator/  (CREF(RDOFuzzyValue) fuzzy_value) const;

	/* 3.116 */  RDOFuzzyValue u_minus() const;
	/* 3.117 */  RDOFuzzyValue u_obr  () const;
	/* 3.118 */  RDOFuzzyValue u_scale(double scale) const;
	/* 3.119 */  RDOFuzzyValue u_log  () const;

	/* 3.39  */  RDOFuzzyValue a_mult    (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.48  */  RDOFuzzyValue alpha     (double appertain) const;
	/* 3.62  */  RDOFuzzyValue supplement() const;
	/* 3.78  */  RDOFuzzyValue a_con     () const;
	/* 3.79  */  RDOFuzzyValue a_dil     () const;

	/* 3.272 */ RDOValue defuzzyfication();

	tstring getAsString() const;

private:
	FuzzySet        m_fuzzySet;
	LPRDOFuzzyType  m_pType;

	FuzzySet::iterator  begin();
	FuzzySet::iterator  end  ();

	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) value);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) value, PTR(void) param);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) value1, CREF(RDOValue) value2);

	             RDOFuzzyValue a_pow     (double power) const;
	/* 3.114 */  RDOFuzzyValue ext_unary (ExtUnaryFun  fun) const;
	/* 3.114 */  RDOFuzzyValue ext_unary (ExtUnaryFunP fun, PTR(void) param) const;
	/* 3.83  */  RDOFuzzyValue ext_binary(ExtBinaryFun fun, CREF(RDOFuzzyValue) fuzzy_value) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFuzzySetDefinition);

class RDOFuzzyType: public RDOType
{
DECLARE_FACTORY(RDOFuzzyType);
public:
	virtual tstring  name      ()                    const;
	virtual RDOValue value_cast(CREF(RDOValue) from) const;

	rbool operator== (CREF(RDOFuzzyType) type) const;
	rbool operator!= (CREF(RDOFuzzyType) type) const;

	rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
	RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

protected:
	RDOFuzzyType(CREF(LPRDOFuzzySetDefinition) pFuzzySetDefinition);
	virtual ~RDOFuzzyType();

private:
	typedef std::map< tstring, RDOFuzzyValue > Terms;

	Terms                    m_terms;
	LPRDOFuzzySetDefinition  m_fuzzySetDefinition;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
OBJECT(RDOFuzzySetDefinition)
{
DECLARE_FACTORY(RDOFuzzySetDefinition)
public:
	virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const = 0;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const = 0;

protected:
	RDOFuzzySetDefinition();
	virtual ~RDOFuzzySetDefinition();
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionFixed: public RDOFuzzySetDefinition
{
public:
	REF(RDOFuzzySetDefinitionFixed) append     (CREF(RDOValue) value);
	REF(RDOFuzzySetDefinitionFixed) operator() (CREF(RDOValue) value);

	virtual  rbool          inRange      (CREF(RDOValue)      value) const;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

private:
	RDOFuzzySetDefinitionFixed();
	virtual ~RDOFuzzySetDefinitionFixed();

	RDOFuzzyValue::FuzzySet  m_items;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionRangeDiscret: public RDOFuzzySetDefinition
{
public:
	RDOFuzzySetDefinitionRangeDiscret(CREF(RDOValue) from, CREF(RDOValue) till, CREF(RDOValue) step = 1);
	virtual ~RDOFuzzySetDefinitionRangeDiscret();

	virtual  rbool          inRange      (CREF(RDOValue)      value) const;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

private:
	RDOValue  m_from;
	RDOValue  m_till;
	RDOValue  m_step;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
class RDOFuzzyEmptyType: public RDOFuzzyType
{
public:
	static LPRDOFuzzyType getInstance();

private:
	static PTR(RDOFuzzyEmptyType) s_emptyType;

	// ----------------------------------------------------------------------------
	// ---------- RDOFuzzySetDefinitionEmpty
	// ----------------------------------------------------------------------------
	class RDOFuzzySetDefinitionEmpty: public RDOFuzzySetDefinition
	{
	public:
		RDOFuzzySetDefinitionEmpty();
		virtual ~RDOFuzzySetDefinitionEmpty();

		virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
		virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;
	};

	RDOFuzzyEmptyType();

	virtual ~RDOFuzzyEmptyType();
	virtual tstring asString() const;
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_DEF_H
