/*!
  \copyright (c) RDO-Team, 2011
  \file      controller_console_options.cpp
  \author    ��������� ������� (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     ���������� ������ RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "app/rdo_studio_console/controller_console_options.h"
#include "app/rdo_studio_console/controller_console_options_config.h"
// --------------------------------------------------------------------------------

RDOControllerConsoleOptions::RDOControllerConsoleOptions(int argc, char *argv[])
{
	po::options_description options_header(RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMAND_SHORT +
		RDO_STUDIO_CONSOLE_VERSION + NOP_STRING + SYSTEM_OS + RDO_SITE);

	po::options_description options_general(_T("General options"));
	createGeneralOptions(options_general);

	po::options_description options_additional(_T("Compatibility options (skipped in console version)"));
	createAdditionalOptions(options_additional);

	m_options.add(options_header);
	m_options.add(options_general);
	m_options.add(options_additional);

	try
	{
		po::store(po::parse_command_line(argc, argv, m_options), m_variables);
		po::notify(m_variables);
	}
	catch (CREF(std::exception) e)
	{
		std::cout << _T("command line error: ") << e.what() << std::endl;
	}
}

RDOControllerConsoleOptions::~RDOControllerConsoleOptions()
{

}

void RDOControllerConsoleOptions::parseQuery()
{
	if (m_variables.empty() || m_variables.count(CHELP_COMMAND) && 
		!m_variables.count(LANGUAGE_COMMAND) && !m_variables.count(VERSION_COMMAND))
	{
		std::cout << m_options << std::endl;
	}
	else if (m_variables.count(LANGUAGE_COMMAND))
	{
		std::cout << _T("rdo language v") + RDO_LANGUAGE_VERSION << _T(" ( supported rdox )") << std::endl;
	}
	else if (m_variables.count(VERSION_COMMAND))
	{
		std::cout << RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMAND_SHORT + RDO_STUDIO_CONSOLE_VERSION << std::endl;
	}
}

void RDOControllerConsoleOptions::getModelName(REF(tstring) modelName)
{
	if(m_variables.count(INPUT_MODEL_COMMAND))
	{
		///! @todo added control input data
		modelName = m_variables[INPUT_MODEL_COMMAND].as<tstring>();
		return;
	}
	modelName = _T("");
}

void RDOControllerConsoleOptions::createGeneralOptions(REF(po::options_description) options)
{
	options.add_options()
		((INPUT_MODEL_COMMAND + COMMA_STRING + INPUT_MODEL_COMMAND_SHORT).c_str(), po::value<tstring>(), MODEL_COMMENT.c_str())
		((VERSION_COMMAND + COMMA_STRING + VERSION_COMMAND_SHORT).c_str(), VERSION_COMMENT.c_str())
		((CHELP_COMMAND + COMMA_STRING + HELP_COMMAND_SHORT).c_str(), HELP_COMMENT.c_str())
		((LANGUAGE_COMMAND + COMMA_STRING + LANGUAGE_COMMAND_SHORT).c_str(), LANGUAGE_COMMENT.c_str());
}

void RDOControllerConsoleOptions::createAdditionalOptions(REF(po::options_description) options)
{
	options.add_options()
		(AUTO_RUN_COMMAND.c_str(), AUTO_RUN_COMMENT.c_str())
		(AUTO_EXIT_COMMAND.c_str(), AUTO_EXIT_COMMENT.c_str())
		(DONT_CLOSE_IF_ERROR_COMMAND.c_str(), DONT_CLOSE_IF_ERROR_COMMENT.c_str())
		(PLUGIN_START_COMMAND.c_str(), PLUGIN_START_COMMENT.c_str())
		(PLUGIN_AUTO_EXIT_COMMAND.c_str(), PLUGIN_AUTO_EXIT_COMMENT.c_str());	
}
