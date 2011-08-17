/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_range.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDO_TYPE_RANGE_H_
#define _CONVERTOR_RDO_TYPE_RANGE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

class Converter;

// ----------------------------------------------------------------------------
// ---------- RDOTypeRangeRange
// ----------------------------------------------------------------------------
OBJECT(RDOTypeRangeRange) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
	void           checkRange()                     const throw(...);
	void           checkValue(CREF(RDOValue) value) const throw(...);
	CREF(RDOValue) getMin    () const;
	CREF(RDOValue) getMax    () const;

private:
	RDOTypeRangeRange(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeRangeRange();

	RDOValue m_min_value;
	RDOValue m_max_value;
};
DECLARE_POINTER(RDOTypeRangeRange);

// ----------------------------------------------------------------------------
// ---------- RDOTypeRange
// ----------------------------------------------------------------------------
template<class T>
class RDOTypeRange: public T
{
DECLARE_FACTORY(RDOTypeRange<T>);
public:
	CREF(LPRDOTypeRangeRange) range() const;

private:
	typedef T parent_type;

	RDOTypeRange(CREF(LPRDOTypeRangeRange) range);
	virtual ~RDOTypeRange();

	LPRDOTypeRangeRange m_range;

	DECLARE_ITypeConverter;
};
typedef RDOTypeRange<RDOType__int>  RDOTypeIntRange;
typedef RDOTypeRange<RDOType__real> RDOTypeRealRange;
DECLARE_POINTER(RDOTypeIntRange);
DECLARE_POINTER(RDOTypeRealRange);

CLOSE_RDO_CONVERTER_NAMESPACE

#include "rdo_lib/rdo_converter/rdo_type_range.inl"

#endif //! _CONVERTOR_RDO_TYPE_RANGE_H_
