/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     ������� � runtime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESOURCE_H_
#define _LIB_RUNTIME_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
PREDECLARE_OBJECT_INTERFACE(IResourceType);

/*!
  \class   RDOResource
  \brief   "�������" ������� �������
  \details �������, ������� ����� ���� ���������� ����������� �
 * ��������, �� �� ����� �������������� � ���������
*/
OBJECT(RDOResource) IS INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
public:
	/*!
	  \enum  ConvertStatus
	  \brief ������� ����������� ����������� ��������
	*/
	enum ConvertStatus
	{
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	typedef  std::vector<RDOValue>      ParamList;
	typedef  ParamList::const_iterator  ParamCIt;

	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(ParamList) paramList, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary);
	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy);
	virtual ~RDOResource();

	rbool operator!= (REF(RDOResource) other);

	ConvertStatus          getState    (                           ) const;
	CREF(RDOValue)         getParam    (ruint index                ) const;
	rbool                  checkType   (ruint type                 ) const;
	rbool                  canFree     (                           ) const;
	CREF(LPIResourceType)  getResType  (                           ) const;
	ruint                  getType     (                           ) const;
	virtual ruint          paramsCount (                           ) const;
	LPRDOResource          clone       (CREF(LPRDORuntime) pRuntime) const;
	CREF(ParamList)        getParamList(                           ) const;

	virtual void    appendParams        (CREF(ParamCIt) from_begin, CREF(ParamCIt) from_end);
	void            setRuntime          (CREF(LPRDORuntime) pRuntime             );
	void            makeTemporary       (rbool value                             );
	void            setState            (ConvertStatus value                     );
	tstring         traceResourceState  (char prefix, CREF(LPRDORuntime) pRuntime);
	REF(RDOValue)   getParamRaw         (ruint index                             );
	void            setParam            (ruint index, CREF(RDOValue) value       );
	tstring         getTypeId           ();
	tstring         traceParametersValue();
	virtual tstring whoAreYou           ();
	void            incRef              ();
	void            decRef              ();

protected:
	ParamList      m_paramList;
	rbool          m_temporary;
	ConvertStatus  m_state;

private:
	ruint            m_type;
	ruint            m_referenceCount;
	LPIResourceType  m_resType;
	tstring          m_typeId;

	tstring traceTypeId();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_resource.inl"

#endif // _LIB_RUNTIME_RESOURCE_H_
