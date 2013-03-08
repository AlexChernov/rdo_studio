/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      07.06.2011
  \brief     ���� �������� � rdo::runtime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_H_
#define _LIB_RUNTIME_RES_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ��������� ���-��� ������� (RTP), ������� ���� ������� ��� ���-��������
//! tparam T - ������, ������� ����� ����������� ������ ��������
template <class T>
CLASS_PARENT_OF(RDOResourceTypeBase, RDOType)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeBase<T>);
public:
	typedef  T  value_type;

private:
	//! �����������
	//! \param number - ������������� �������������
	RDOResourceTypeBase(ruint number);
	virtual ~RDOResourceTypeBase();

	DECLARE_IResourceType;
};

//! ��� �������� ��� �������� ������� �������� ���
//! \details ������� �������, ������� ����� ���� ���������� ����������� �
//!          ��������, �� �� ����� �������������� � ���������
typedef  RDOResourceTypeBase<RDOResource>     RDOResourceType;
typedef  rdo::intrusive_ptr<RDOResourceType>  LPRDOResourceType;

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_res_type.inl"

#endif // _LIB_RUNTIME_RES_TYPE_H_
