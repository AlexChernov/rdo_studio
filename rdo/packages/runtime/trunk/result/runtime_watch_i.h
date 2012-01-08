/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopokaz_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     ���������� ���������� �����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_POKAZ_I_H_
#define _LIB_RUNTIME_POKAZ_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

/*!
  \interface IPokaz
  \brief     ������� ��������� ����������� ����������
*/
class IPokaz
{
public:
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream) = 0;
};

#define DECLARE_IPokaz                                               \
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream);

/*!
  \interface IPokazWatchQuant
  \brief     ��������� ����������� ���������� WatchQuant
*/
class IPokazWatchQuant
{
public:
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc) = 0;
};

#define DECLARE_IPokazWatchQuant \
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc);

/*!
  \interface IPokazWatchValue
  \brief     ��������� ����������� ���������� WatchValue
*/
class IPokazWatchValue
{
public:
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ) = 0;
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ) = 0;
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc) = 0;
};

#define DECLARE_IPokazWatchValue                                                   \
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ); \
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ); \
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc);

/*!
  \interface IPokazGetValue
  \brief     ��������� ����������� ���������� GetValue
*/
class IPokazGetValue
{
public:
	virtual CREF(rdoRuntime::RDOValue) getValue() const = 0;
};

#define DECLARE_IPokazGetValue \
	CREF(rdoRuntime::RDOValue) getValue() const;

#endif // _LIB_RUNTIME_POKAZ_I_H_
