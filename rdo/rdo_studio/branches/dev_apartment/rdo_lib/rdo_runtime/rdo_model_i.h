/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_model_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     ���������� IModelStructure � IName
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MODEL_I_H_
#define _LIB_RUNTIME_MODEL_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface IModelStructure
  \brief     ��������� ��������� ������, ������������ ��� ������ �������� � ���� �����������
*/
class IModelStructure
{
public:
	/*!
	  \brief      �������� ������� ������ � �����
	  \param[out] stream �������� �����
	*/
	virtual void writeModelStructure(REF(std::ostream) stream) const = 0;
};

#define DECLARE_IModelStructure \
	virtual void writeModelStructure(REF(std::ostream) stream) const;

/*!
  \interface IName
  \brief     ��������� ����� �������
*/
class IName
{
public:
	/*!
	  \brief  �������� ��� �������
	  \return ��� �������
	*/
	virtual CREF(tstring) name() const = 0;
};

#define DECLARE_IName \
	virtual CREF(tstring) name() const;

#endif // _LIB_RUNTIME_MODEL_I_H_
