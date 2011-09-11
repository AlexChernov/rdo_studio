/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmart_ptr_wrapper.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      26.02.2010
  \brief     
  \indent    4T
*/

#ifndef _LIB_COMMON_RDOSMART_PTR_WRAPPER_H_
#define _LIB_COMMON_RDOSMART_PTR_WRAPPER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class ISmartPtrWrapper
{
public:
	virtual void      destroy    () = 0;
	virtual PTR(void) getSmartPtr() = 0;
};
typedef PTR(ISmartPtrWrapper) LPISmartPtrWrapper;

#define DECLARE_ISmartPtrWrapper     \
public:                              \
	virtual void      destroy    (); \
	virtual PTR(void) getSmartPtr();

template<class T>
class smart_ptr_wrapper: public ISmartPtrWrapper
{
public:
	smart_ptr_wrapper(CREF(intrusive_ptr<T>) intrusive_ptr)
		: m_intrusive_ptr(intrusive_ptr)
	{}
	~smart_ptr_wrapper()
	{}
	void destroy()
	{
		delete this;
	}
	PTR(void) getSmartPtr()
	{
		return &m_intrusive_ptr;
	}
	CREF(intrusive_ptr<T>) get() const
	{
		return m_intrusive_ptr;
	}

private:
	intrusive_ptr<T> m_intrusive_ptr;
};

class smart_ptr_wrapper_caster
{
public:
	smart_ptr_wrapper_caster(CREF(LPISmartPtrWrapper) pISmartPtrWrapper)
		: m_pISmartPtrWrapper(pISmartPtrWrapper)
	{
		ASSERT(m_pISmartPtrWrapper);
	}
	~smart_ptr_wrapper_caster()
	{
		m_pISmartPtrWrapper->destroy();
	}

	template <class T>
	intrusive_ptr<T> cast() const
	{
		PTR(rdo::smart_ptr_wrapper<T>) pSmartPtrWrapper = dynamic_cast<PTR(rdo::smart_ptr_wrapper<T>)>(m_pISmartPtrWrapper);
		return pSmartPtrWrapper ? pSmartPtrWrapper->get() : intrusive_ptr<T>();
	}

private:
	LPISmartPtrWrapper m_pISmartPtrWrapper;
};

CLOSE_RDO_NAMESPACE

#endif // _LIB_COMMON_RDOSMART_PTR_WRAPPER_H_
