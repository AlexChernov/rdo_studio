/*!
  \copyright (c) RDO-Team, 2012
  \file      error.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ERROR_H_
#define _LIB_RUNTIME_ERROR_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_object.h"
#include "simulator/service/error_code.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class Error
{
public:
	typedef  std::vector<rdoSimulator::RDOSyntaxError>  ErrorList;

	void            push(CREF(rdoSimulator::RDOSyntaxError) error);
	void            push(CREF(tstring) message, CREF(RDOSrcInfo) srcInfo);
	CREF(ErrorList) list() const;

private:
	ErrorList m_errorList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_ERROR_H_
