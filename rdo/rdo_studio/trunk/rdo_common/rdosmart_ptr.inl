/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdosmart_ptr.inl
 * author   : ������ ������
 * date     : 12.07.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
inline smart_ptr<T>::smart_ptr()
	: m_object (NULL)
	, m_counter(NULL)
{}

template<class T>
inline smart_ptr<T>::smart_ptr(PTR(T) obj)
	: m_object(obj)
{
	if (m_object)
	{
		allocateCounter();
		counter() = 1;
	}
	else
	{
		m_counter = NULL;
	}
}

template<class T>
inline smart_ptr<T>::smart_ptr(CREF(this_type) sptr)
	: m_counter(sptr.m_counter)
	, m_object (sptr.m_object )
{
	addref();
}

template<class T>
inline REF(typename smart_ptr<T>::this_type) smart_ptr<T>::operator= (CREF(this_type) sptr)
{
	if (get() != NULL)
		clear();

	m_counter = sptr.m_counter;
	m_object  = sptr.m_object;
	addref();
	return *this;
}

template<class T>
inline smart_ptr<T>::~smart_ptr()
{
	if (inited())
		clear();
}

template<class T>
inline PTR(T) smart_ptr<T>::get()
{
	return m_object;
}

template<class T>
inline CPTR(T) smart_ptr<T>::get() const
{
	return m_object;
}

template<class T>
inline smart_ptr<T>::operator rbool () const
{
	return get() != NULL;
}

template<class T>
inline PTR(T) smart_ptr<T>::operator-> ()
{
	return get();
}

template<class T>
inline rbool smart_ptr<T>::owner() const
{
	return inited() ? (counter() == 1) : false;
}

template<class T>
inline void smart_ptr<T>::addref()
{
	if (inited())
		counter()++;
}

template<class T>
inline void smart_ptr<T>::release()
{
	if (!inited())
		return;

	counter()--;
	if (counter() == 0)
	{
		if (m_object)
		{
			Factory<T>::destroy(m_object);
			m_object = NULL;
		}
	}
}

template<class T>
inline rbool smart_ptr<T>::inited() const
{
	return m_counter != NULL;
}

template<class T>
inline CREF(ruint) smart_ptr<T>::counter() const
{
	return *m_counter;
}

template<class T>
inline REF(ruint) smart_ptr<T>::counter()
{
	return *m_counter;
}

template<class T>
inline void smart_ptr<T>::allocateCounter()
{
	m_counter = new ruint;
}

template<class T>
inline void smart_ptr<T>::deallocateCounter()
{
	delete m_counter;
}

template<class T>
inline void smart_ptr<T>::clear()
{
	release();
	if (counter() == 0)
		deallocateCounter();
}

CLOSE_RDO_NAMESPACE
