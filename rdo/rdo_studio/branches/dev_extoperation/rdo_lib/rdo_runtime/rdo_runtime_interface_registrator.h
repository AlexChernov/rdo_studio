/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_runtime_interface_registrator.h
 * author   : ������ ������
 * date     : 29.07.2009
 * bref     : ����������� ����������� ���������� ������
 * indent   : 4T
 */

#ifndef _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_
#define _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

INTERFACE_REGISTRATOR(IPokaz,                     1);
INTERFACE_REGISTRATOR(IPokazTrace,                2);
INTERFACE_REGISTRATOR(IPokazTraceValue,           3);
INTERFACE_REGISTRATOR(IPokazWatchQuant,           4);
INTERFACE_REGISTRATOR(IPokazWatchValue,           5);
INTERFACE_REGISTRATOR(INotify,                    6);
INTERFACE_REGISTRATOR(IModelStructure,            7);
INTERFACE_REGISTRATOR(ITrace,                     8);
INTERFACE_REGISTRATOR(IName,                      9);
INTERFACE_REGISTRATOR(IBaseOperation,            10);
INTERFACE_REGISTRATOR(IIrregEvent,               11);
INTERFACE_REGISTRATOR(IRule,                     12);
INTERFACE_REGISTRATOR(IOperation,                13);
INTERFACE_REGISTRATOR(IAction,                   14);
INTERFACE_REGISTRATOR(IKeyboard,                 15);
INTERFACE_REGISTRATOR(ILogic,                    16);
INTERFACE_REGISTRATOR(IDPTSearchTraceStatistics, 17);
INTERFACE_REGISTRATOR(IBaseOperationContainer,   18);
INTERFACE_REGISTRATOR(IPROCBlock,                19);
INTERFACE_REGISTRATOR(IPriority,                 20);
INTERFACE_REGISTRATOR(IActivity,                 21);
INTERFACE_REGISTRATOR(IActivityTrace,            22);
INTERFACE_REGISTRATOR(IActivityPatternTrace,     23);
INTERFACE_REGISTRATOR(IOperationTrace,           24);
INTERFACE_REGISTRATOR(IActionTrace,              25);
INTERFACE_REGISTRATOR(IDPTSearchActivity,        26);
INTERFACE_REGISTRATOR(IDPTSearchLogic,           27);
INTERFACE_REGISTRATOR(IPROCProcess,              28);

#endif //! _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_
