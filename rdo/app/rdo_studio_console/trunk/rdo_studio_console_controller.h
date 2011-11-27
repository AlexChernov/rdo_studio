/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_studio_console_controller.h
  \author    ��������� ������� (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     ���������� ������ RDO
  \indent    4T
*/

#ifndef _RDO_STUDIO_CONSOLE_CONTROLLER_H_
#define _RDO_STUDIO_CONSOLE_CONTROLLER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/thread/mutex.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
// --------------------------------------------------------------------------------

class RDOStudioConsoleController: public RDOThread
{
public:
	RDOStudioConsoleController();
	virtual ~RDOStudioConsoleController();
	
	bool simulationFinished();
	
private:
	void proc(REF(RDOMessageInfo) msg);
	
private:
	boost::mutex m_simulation_mutex;
	rbool        m_simulation;
};

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
