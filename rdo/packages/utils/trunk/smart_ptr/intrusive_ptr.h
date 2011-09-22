/*!
  \copyright (c) RDO-Team, 2011
  \file      intrusive_ptr.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.06.2010
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_INTRUSIVE_PTR_H_
#define _UTILS_SMART_PTR_INTRUSIVE_PTR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "utils/smart_ptr/counter_reference.h"
#include "utils/smart_ptr/interface_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
class intrusive_ptr
{
template <typename P> friend class intrusive_ptr;
public:
	typedef T                object_type;
	typedef intrusive_ptr<T> this_type;

	 intrusive_ptr();
	 intrusive_ptr(PTR (T)         object);
	 intrusive_ptr(CREF(this_type) sptr  );
	~intrusive_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);

	//! ���������� �� ����������
	template <class P>
	rbool operator== (CREF(intrusive_ptr<P>) sptr) const;
	template <class P>
	rbool operator!= (CREF(intrusive_ptr<P>) sptr) const;

	//! ���������� �� ���������
	template<class P>
	rbool compare(CREF(intrusive_ptr<P>) sptr) const;

	operator rbool     () const;
	 PTR(T) operator-> () const;
	 PTR(T) operator-> ();

	template <class P>
	operator intrusive_ptr<P>() const;

	template <class P>
	intrusive_ptr<P> object_parent_cast() const;

	template <class P>
	intrusive_ptr<P> object_static_cast() const;

	template <class P>
	intrusive_ptr<P> object_dynamic_cast() const;

	template <class P>
	interface_ptr<P> interface_cast();

	template <class P>
	interface_ptr<P> interface_dynamic_cast();

	/// @todo ����������� � protected
	PTR(T)   get();
	CPTR(T)  get() const;

	rbool owner () const;

protected:
	void  addref ();
	void  release();

private:
	PTR(T) m_object;

	REF(ruint) counter();
};

#define DECLARE_POINTER(TYPE)    typedef rdo::intrusive_ptr<TYPE> LP##TYPE;
#define PREDECLARE_POINTER(TYPE) class TYPE; DECLARE_POINTER(TYPE);
#define OBJECT(TYPE)             PREDECLARE_POINTER(TYPE) class TYPE: public rdo::counter_reference

CLOSE_RDO_NAMESPACE

#include "utils/smart_ptr/intrusive_ptr.inl"

#endif // _UTILS_SMART_PTR_INTRUSIVE_PTR_H_
