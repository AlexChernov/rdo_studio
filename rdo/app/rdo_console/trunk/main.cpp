/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    ��������� ������� (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     ���������� ������ RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/rdo_console_controller.h"
// --------------------------------------------------------------------------------

static ruint g_exitCode;

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);

	options_controller.parseOptions();

	tstring model_name;
	options_controller.getModelName(model_name);
	
	rbool model_exist = fs::exists(model_name);

	if (model_exist)
	{
		// Init
		RDOKernel::init();
		new rdo::service::simulation::RDOThreadSimulator();
		new rdo::repository::RDOThreadRepository();

		PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
		ASSERT(pAppController);

		rdo::repository::RDOThreadRepository::OpenFile data(model_name);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD      );
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN        );

		while (!pAppController->finished())
		{
			kernel->idle();

			if (pAppController->runtimeError())
			{
				std::cout << _T("Run-time error") << std::endl;
				g_exitCode = TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR;
			}
		}
		bool simulationSuccessfully = pAppController->simulationSuccessfully();

		RDOKernel::close();

		if (simulationSuccessfully)
		{
			std::cout << _T("Simulation finished successfully") << std::endl;
			g_exitCode = TERMINATION_NORMAL;
		}
		else
		{
			std::cout << _T("Simulation completed with errors") << std::endl;
		}
	}
	else if(!options_controller.helpQuery())
	{
		std::cout << _T("Model does not exist") << std::endl;
		g_exitCode = TERMINATION_WITH_AN_ERROR_NO_MODEL;
	}
	return g_exitCode;
}
