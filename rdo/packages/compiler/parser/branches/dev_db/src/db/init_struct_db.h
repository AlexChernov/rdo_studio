/*!
  \copyright (c) RDO-Team, 2013
  \file      init_struct_db.h
  \authors   �������� ����� (cerevra@gmail.com)
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      25.01.2013
  \brief     �������� ��������� ��
  \indent    4T
*/

#ifndef _SIMULATOR_PARSER_DB_INIT_STRUCT_H_
#define _SIMULATOR_PARSER_DB_INIT_STRUCT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\src\db\general_db.h"
// --------------------------------------------------------------------------------

class InitSructDB : public GeneralDB
{
public:
	InitSructDB();

private:
	GeneralDB::QueryList generateCreateDBQuery();
};

#endif // _SIMULATOR_PARSER_DB_INIT_STRUCT_H_
