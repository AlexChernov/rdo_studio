/*
 * copyright: (c) RDO-Team, 2010
 * filename : stack.h
 * author   : ������ ������
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_COMMON_STACK_H_
#define _RDO_COMMON_STACK_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
class stack
{
public:
	void    push  (CREF(T) item);
	void    pop   ();

	rbool   empty () const;
	ruint   size  () const;

	CREF(T) top   () const;
	 REF(T) top   ();

private:
	typedef std::list<T> Container;
	Container m_container;
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/stack.inl"

#endif //! _RDO_COMMON_STACK_H_
