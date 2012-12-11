/*!
  \copyright (c) RDO-Team, 2012
  \file      context_param_definition.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     �������� �������� ���������� �������
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_
#define _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextParamDefinitionManager
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextParamDefinitionManager)
{
DECLARE_FACTORY(IContextParamDefinitionManager)
public:
	virtual void pushParamDefinitionContext() = 0;
	virtual void popParamDefinitionContext () = 0;

protected:
	IContextParamDefinitionManager()
	{}
	virtual ~IContextParamDefinitionManager()
	{}
};

#define DECLARE_IContextParamDefinitionManager \
	void pushParamDefinitionContext();         \
	void popParamDefinitionContext ();

// --------------------------------------------------------------------------------
// -------------------- ContextParamDefinition
// --------------------------------------------------------------------------------
class ContextParamDefinition
	: public Context
	, public IContextParamDefinitionManager
{
DECLARE_FACTORY(ContextParamDefinition)
public:
	typedef boost::optional<ruint> ParamID;

	void       pushParam  (CREF(LPRDOParam) pParam);
	LPRDOParam findParam  (CREF(tstring) paramName) const;
	ParamID    findParamID(CREF(tstring) paramName) const;

private:
	typedef std::vector<LPRDOParam> ParamList;

	ParamList m_paramList;

	ParamList::const_iterator find(CREF(tstring) paramName) const;

	DECLARE_IContextParamDefinitionManager;
};
DECLARE_POINTER(ContextParamDefinition);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_CONTEXT_FUNCTION_PARAM_DEFINITION_H_
