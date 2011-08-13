/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_fuzzy.inl
 * @authors   ������ ������
 * @date      18.04.2009
 * @brief     �������� ������
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOFuzzyValue
// ********************************************************************************
inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyType) pType)
	: m_pType(pType)
{}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(RDOFuzzyValue) value)
	: m_pType(value.m_pType)
{
	m_fuzzySet = value.m_fuzzySet;
}

inline RDOFuzzyValue::~RDOFuzzyValue()
{}

inline REF(RDOFuzzyValue) RDOFuzzyValue::append(CREF(RDOValue) rdovalue, double appertain)
{
	operator[](rdovalue) = appertain;
	return *this;
}

inline REF(RDOFuzzyValue) RDOFuzzyValue::operator() (CREF(RDOValue) rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

inline REF(double) RDOFuzzyValue::operator[] (CREF(RDOValue) rdovalue)
{
	if (!type()->inRange(rdovalue))
		throw RDOValueException();

	return m_fuzzySet[rdovalue];
}

inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::find(CREF(RDOValue) rdovalue) const
{
	return m_fuzzySet.find(rdovalue);
}

inline RDOFuzzyValue::FuzzyItem RDOFuzzyValue::findValue(CREF(RDOValue) rdovalue) const
{
	FuzzySet::const_iterator found = find(rdovalue);
	if (found != end()) return FuzzyItem(found->first, found->second);
	else                return FuzzyItem(rdovalue,     0.0          );
}

inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::begin() const { return m_fuzzySet.begin(); }
inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::end  () const { return m_fuzzySet.end();   }
inline RDOFuzzyValue::FuzzySet::iterator       RDOFuzzyValue::begin()       { return m_fuzzySet.begin(); }
inline RDOFuzzyValue::FuzzySet::iterator       RDOFuzzyValue::end  ()       { return m_fuzzySet.end();   }
inline rbool                                   RDOFuzzyValue::empty() const { return m_fuzzySet.empty(); }
inline CREF(LPRDOFuzzyType)                    RDOFuzzyValue::type () const { return m_pType;            }

inline RDOFuzzyValue RDOFuzzyValue::supplement() const { return type()->getSupplement(*this); }
inline RDOFuzzyValue RDOFuzzyValue::a_con     () const { return a_pow(2.0);                   }
inline RDOFuzzyValue RDOFuzzyValue::a_dil     () const { return a_pow(0.5);                   }

// ********************************************************************************
// ******************** RDOFuzzyType
// ********************************************************************************
inline RDOFuzzyType::RDOFuzzyType(CREF(LPRDOFuzzySetDefinition) pFuzzySetDefinition)
	: RDOType             (t_fuzzy            )
	, m_fuzzySetDefinition(pFuzzySetDefinition)
{
	//! ����
	//! m_fuzzySetDefinition->reparent(this);
	/// @todo ��� ������� ��������� �� ����� ���������
}

inline RDOFuzzyType::~RDOFuzzyType()
{}

inline tstring RDOFuzzyType::name() const
{
	return _T("RDOFuzzyType");
}

inline RDOValue RDOFuzzyType::value_cast(CREF(RDOValue) from) const
{
	throw RDOTypeException();
}

inline rbool RDOFuzzyType::operator== (CREF(RDOFuzzyType) type) const
{
	return this == &type;
}

inline rbool RDOFuzzyType::operator!= (CREF(RDOFuzzyType) type) const
{
	return !operator==(type);
}

inline rbool RDOFuzzyType::inRange(CREF(RDOValue) rdovalue) const
{
	return m_fuzzySetDefinition->inRange(rdovalue);
}

inline RDOFuzzyValue RDOFuzzyType::getSupplement(CREF(RDOFuzzyValue) value) const
{
	return m_fuzzySetDefinition->getSupplement(value);
}

// ********************************************************************************
// ******************** RDOFuzzySetDefinition
// ********************************************************************************
inline RDOFuzzySetDefinition::RDOFuzzySetDefinition()
{}

inline RDOFuzzySetDefinition::~RDOFuzzySetDefinition()
{}

// ********************************************************************************
// ******************** RDOFuzzySetDefinitionFixed
// ********************************************************************************
inline RDOFuzzySetDefinitionFixed::RDOFuzzySetDefinitionFixed()
	: RDOFuzzySetDefinition()
{}

inline RDOFuzzySetDefinitionFixed::~RDOFuzzySetDefinitionFixed()
{}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::append(CREF(RDOValue) rdovalue)
{
	m_items[rdovalue] = 1.0;
	return *this;
}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::operator() (CREF(RDOValue) rdovalue)
{
	return append(rdovalue);
}

inline rbool RDOFuzzySetDefinitionFixed::inRange(CREF(RDOValue) rdovalue) const
{
	return m_items.find(rdovalue) != m_items.end();
}

// ********************************************************************************
// ******************** RDOFuzzySetDefinitionRangeDiscret
// ********************************************************************************
inline RDOFuzzySetDefinitionRangeDiscret::RDOFuzzySetDefinitionRangeDiscret(CREF(RDOValue) from, CREF(RDOValue) till, CREF(RDOValue) step)
	: RDOFuzzySetDefinition(    )
	, m_from               (from)
	, m_till               (till)
	, m_step               (step)
{}

inline RDOFuzzySetDefinitionRangeDiscret::~RDOFuzzySetDefinitionRangeDiscret()
{}

// ********************************************************************************
// ******************** RDOFuzzyEmptyType
// ********************************************************************************
inline RDOFuzzyEmptyType::RDOFuzzyEmptyType()
	: RDOFuzzyType(rdo::Factory<RDOFuzzySetDefinitionEmpty>::create())
{}

inline RDOFuzzyEmptyType::~RDOFuzzyEmptyType()
{
	RDOFuzzyEmptyType::s_emptyType = NULL;
}

inline tstring RDOFuzzyEmptyType::asString() const
{
	return _T("[empty set]");
}

inline LPRDOFuzzyType RDOFuzzyEmptyType::getInstance()
{
	if (!RDOFuzzyEmptyType::s_emptyType)
	{
		RDOFuzzyEmptyType::s_emptyType = new RDOFuzzyEmptyType();
	}
	return LPRDOFuzzyType(RDOFuzzyEmptyType::s_emptyType);
}

// ********************************************************************************
// ******************** RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty
// ********************************************************************************
inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::RDOFuzzySetDefinitionEmpty()
	: RDOFuzzySetDefinition()
{}

inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::~RDOFuzzySetDefinitionEmpty()
{}

inline rbool RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::inRange(CREF(RDOValue) rdovalue) const
{
	return false;
}

inline RDOFuzzyValue RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::getSupplement(CREF(RDOFuzzyValue) value) const
{
	return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance());
}

CLOSE_RDO_RUNTIME_NAMESPACE
