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
#include "app/rdo_studio_console/terminate_codes.h"
#include "app/rdo_studio_console/controller_console_options.h"
#include "app/rdo_studio_console/rdo_studio_console_controller.h"
// --------------------------------------------------------------------------------

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
		new rdoSimulator::RDOThreadSimulator();
		new rdoRepository::RDOThreadRepository();

		PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
		ASSERT(pAppController);

		rdoRepository::RDOThreadRepository::OpenFile data(model_name);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD      );
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN        );

		while (!pAppController->finished())
		{
			kernel->idle();

			if (pAppController->errorOccurred())
			{
				std::cerr << _T("  run-time error  ") << std::endl;
				exit(TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR);
			}
		}

		bool simulationResult = pAppController->simulationSuccessfully();

		// Close
		RDOKernel::close();

		if (simulationResult)
		{
			std::cout << _T("  simulation finished successfully  ") << std::endl;
		}
		else
		{
			std::cout << _T("  simulation completed with errors  ") << std::endl;		
		}
	}
	else if(!options_controller.helpQuery())
	{
		std::cout << _T("  model does not exist  ") << std::endl;
		return TERMINATION_WITH_AN_ERROR_NO_MODEL;
	}
	return NORMAL_TERMINATION;
}
