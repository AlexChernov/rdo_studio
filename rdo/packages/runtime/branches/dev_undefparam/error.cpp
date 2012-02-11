/*!
  \copyright (c) RDO-Team, 2012
  \file      error.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
void Error::push(CREF(rdoSimulator::RDOSyntaxError) error)
{
	m_errorList.push_back(error);
	throw RDORuntimeException(_T(""));
}

void Error::push(CREF(tstring) message, CREF(LPRDOCalc) pCalc)
{
	ASSERT(!message.empty());
	ASSERT(pCalc);

	rdoSimulator::RDOSyntaxError error(
		rdoSimulator::RDOSyntaxError::UNKNOWN,
		message,
		pCalc->srcInfo().src_pos().m_last_line,
		pCalc->srcInfo().src_pos().m_last_pos,
		pCalc->srcInfo().src_filetype()
	);
	push(error);
}

CREF(Error::ErrorList) Error::list() const
{
	return m_errorList;
}

CLOSE_RDO_RUNTIME_NAMESPACE
