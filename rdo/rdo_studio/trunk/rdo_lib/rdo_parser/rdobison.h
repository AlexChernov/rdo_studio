/*
  \copyright (c) RDO-Team, 2011
  \file      rdobison.h
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      03.08.2010
  \brief     ��������� ��������� ������ ������, ����������� �������� ��������
  \indent    4T
 */

#ifndef _RDOBISON_H_
#define _RDOBISON_H_

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
// ===============================================================================

#ifndef YYLTYPE
struct yyltype
{
	ruint m_first_line;
	ruint m_first_pos;

	ruint m_last_line;
	ruint m_last_pos;

	ruint m_first_seek;
	ruint m_last_seek;
};

#define YYLTYPE yyltype
#endif //! YYLTYPE

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)       \
	Current.m_last_line = Rhs[N].m_last_line; \
	Current.m_last_pos  = Rhs[N].m_last_pos;
#endif //! YYLLOC_DEFAULT

#endif //! _RDOBISON_H_
