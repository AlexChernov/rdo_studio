/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     ������ � �������
  \indent    4T
*/

#ifndef _LIB_COMMON_RDOFILE_H_
#define _LIB_COMMON_RDOFILE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

//! ������ ������������ ��� ��� ����������� �������, ��� �����
class File
{
public:
	//! ��������� ������������� �����
	//! \param name - ��� �����
	//! \result \b true, ���� ����������
	static rbool   exist          (CREF(tstring) name);

	//! ��������� ������� ������ ������
	//! \param name - ��� �����
	//! \result \b true, ���� � ����� ���������� ������� ������ ������
	static rbool   read_only      (CREF(tstring) name);

	//! ������� ����
	//! \param name - ��� �����
	//! \result \b true, ���� ���� ����� ������
	static rbool   unlink         (CREF(tstring) name);

	//! ��������� ������ ��� ����� �� ��� ������������
	//! \param[in]  name     - ������ ��� �����
	//! \param[out] fileDir  - ����
	//! \param[out] fileName - ���
	//! \param[out] fileExt  - ����������
	//! \result \b true, ���� ���������� ������� ������
	static rbool   splitpath      (CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt);

	//! ���������� ������ ��� ���������� �����
	//! \result ������ ��� ���������� �����
	static tstring getTempFileName();

	//! ������� ������� (�������, ���������, �������� �����) � ����� �����
	//! \param name - ��� �����
	//! \result \b true, ���� �� ������ �������
	static rbool   trimLeft       (CREF(tstring) name);
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/rdofile.inl"

#endif //! _LIB_COMMON_RDOFILE_H_
