/*!
  \copyright (c) RDO-Team, 2012
  \file      notify.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_NOTIFY_H_
#define _LIB_RUNTIME_NOTIFY_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/notify_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ��������� ����� ���������
class Notify
{
public:
	//! ���� ���������
	enum Type
	{
		RO_BEFOREDELETE = 0
	};

	void connect    (PTR(INotify) pTo, ruint message);
	void disconnect (PTR(INotify) pTo               );
	void disconnect (PTR(INotify) pTo, ruint message);
	void fireMessage(ruint message, PTR(void) pParam) const;

private:
	typedef std::multimap<ruint, PTR(INotify)> Connected;
	Connected m_connected;

	void disconnect(Connected::iterator it, PTR(INotify) pTo);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_NOTIFY_H_
