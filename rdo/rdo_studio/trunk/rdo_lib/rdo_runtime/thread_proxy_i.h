/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      thread_proxy_i.h
 * @author    ������ ������
 * @date      05.03.2011
 * @brief     ��������� IThreadProxy
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_THREAD_PROXY_I_H_
#define _LIB_RUNTIME_THREAD_PROXY_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ********************************************************************************

/******************************************************************************//**
 * @interface IThreadProxy
 * @brief     ��������� IThreadProxy
 *********************************************************************************/
OBJECT_INTERFACE(IThreadProxy)
{
DECLARE_FACTORY(IThreadProxy)
public:
	enum ThreadID
	{
		TID_REPOSITORY
	};
	virtual void sendMessage(ThreadID threadID, ruint messageID, PTR(void) pParam) = 0;

protected:
	IThreadProxy()
	{}
	virtual ~IThreadProxy()
	{}
};

#endif // _LIB_RUNTIME_THREAD_PROXY_I_H_
