/*!
  \copyright (c) RDO-Team, 2011
  \file      procgui.h
  \author    ������� ������
  \date      22.04.2011
  \brief     
  \indent    4T
*/

#ifndef _PROCGUI_H_
#define _PROCGUI_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/service/namespace.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/service/procgui_datablock.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ProcGUICalc
// --------------------------------------------------------------------------------
class ProcGUICalc
{
public:
	rdoRuntime::LPRDOCalcConst          getConstCalc     (double arg                                       );
	rdoRuntime::LPRDOCalcFunctionCaller getNormalCalc    (ruint base, double arg1, double arg2             );
	rdoRuntime::LPRDOCalcFunctionCaller getUniformCalc   (ruint base, double arg1, double arg2             );
	rdoRuntime::LPRDOCalcFunctionCaller getExpCalc       (ruint base, double arg                           );
	rdoRuntime::LPRDOCalcFunctionCaller getTriangularCalc(ruint base, double arg1, double arg2, double arg3);
	rdoRuntime::LPRDOCalc               getCalc();

protected:
	ProcGUICalc(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(LPRPShapeDataBlock) pParams);
	virtual ~ProcGUICalc();

private:
	rdoRuntime::LPRDORuntime  m_pRuntime;
	LPRPShapeDataBlock        m_pParams;
	rdoRuntime::LPRDOCalc     m_pCalc;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlock);

OBJECT(ProcGUIProcess)
{
DECLARE_FACTORY(ProcGUIProcess);
public:
	typedef  std::list<LPProcGUIBlock>  BlockList;
	typedef  std::list<tstring>         ResNameList;

	static tstring s_namePrefix;
	static tstring s_nameSufix;

	void           clear             ();
	void           insertBlock       (CREF(LPProcGUIBlock)        pBlock );
	void           initResources     (CREF(rdoParser::LPRDOParser) pParser);
	void           addResNameToBlock (CREF(tstring)               name   );
	CREF(LPILogic) getProcess        () const     { return m_pProcess; }

private:
	ProcGUIProcess(CREF(rdoRuntime::LPRDORuntime) pRuntime);
	virtual ~ProcGUIProcess();

	rdoRuntime::LPRDORuntime  m_pRuntime;
	LPILogic                  m_pProcess;
	BlockList                 m_blockList;
	ResNameList               m_resNameList;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlock
// --------------------------------------------------------------------------------
OBJECT(ProcGUIBlock)
{
DECLARE_FACTORY(ProcGUIBlock);
protected:
	ProcGUIBlock(CREF(LPProcGUIProcess) pProcess, CREF(tstring) name);
	virtual ~ProcGUIBlock();

private:
	tstring m_name;
};

#define PROCGUI_BLOCK(A)                  \
PREDECLARE_POINTER(A);                    \
CLASS(A): IMPLEMENTATION_OF(ProcGUIBlock)

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockGenerate
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIBlockGenerate)	IS IMPLEMENTATION_OF(ProcGUICalc)
{
DECLARE_FACTORY(ProcGUIBlockGenerate);
private:
	ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(rdoParser::LPRDOParser) pParser, CREF(LPRPShapeDataBlockCreate) pParams);
	virtual ~ProcGUIBlockGenerate();

	LPIPROCBlock             m_pBlock;
	LPRPShapeDataBlockCreate m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockTerminate
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIBlockTerminate)
{
DECLARE_FACTORY(ProcGUIBlockTerminate);
private:
	ProcGUIBlockTerminate(CREF(LPProcGUIProcess) pProcess, CREF(LPRPShapeDataBlockTerminate) pParams);
	virtual ~ProcGUIBlockTerminate();

	LPIPROCBlock                m_pBlock;
	LPRPShapeDataBlockTerminate m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockProcess
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIBlockProcess)
{
DECLARE_FACTORY(ProcGUIBlockProcess);
private:
	ProcGUIBlockProcess(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(rdoParser::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);
	virtual ~ProcGUIBlockProcess();

	LPProcGUIBlock            m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIAdvance
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIAdvance) IS IMPLEMENTATION_OF(ProcGUICalc)
{
DECLARE_FACTORY(ProcGUIAdvance);
private:
	ProcGUIAdvance(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(LPRPShapeDataBlockProcess) pParams);
	virtual ~ProcGUIAdvance();

	LPIPROCBlock              m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUISeize
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUISeize)
{
DECLARE_FACTORY(ProcGUISeize);
public:
	void createRuntime  (CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser);
	void addResourceName(CREF(tstring)          name);

private:
	ProcGUISeize(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);
	virtual ~ProcGUISeize();

	typedef  std::list<tstring>                         ResNameList;
	typedef  std::vector<rdoRuntime::parser_for_Seize>  TODO_parser_for_Seize;

	LPIPROCBlock              m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
	ResNameList               m_resNameList;
	TODO_parser_for_Seize     m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIRelease
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIRelease)
{
DECLARE_FACTORY(ProcGUIRelease);
public:
	void createRuntime  (CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser);
	void addResourceName(CREF(tstring)          name);

private:
	ProcGUIRelease(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);
	virtual ~ProcGUIRelease();

	typedef  std::list<tstring>                         ResNameList;
	typedef  std::vector<rdoRuntime::parser_for_Seize>  TODO_parser_for_Seize;

	LPIPROCBlock              m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
	ResNameList               m_resNameList;
	TODO_parser_for_Seize     m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIQueue
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIQueue)
{
DECLARE_FACTORY(ProcGUIQueue);
public:
	void createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser);
	void setResource  (CREF(tstring)          name);

private:
	ProcGUIQueue(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser, CREF(tstring) name);
	virtual ~ProcGUIQueue();

	tstring                      m_resourceName;
	LPIPROCBlock                 m_pBlock;
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIDepart
// --------------------------------------------------------------------------------
PROCGUI_BLOCK(ProcGUIDepart)
{
DECLARE_FACTORY(ProcGUIDepart);
public:
	void createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser);
	void setResource  (CREF(tstring)          name);

private:
	ProcGUIDepart(CREF(LPProcGUIProcess) pProcess, CREF(rdoParser::LPRDOParser) pParser, CREF(tstring) name);
	virtual ~ProcGUIDepart();

	tstring                      m_resourceName;
	LPIPROCBlock                 m_pBlock;
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};

CLOSE_RDO_SIMULATOR_NAMESPACE

#endif // _PROCGUI_H_
