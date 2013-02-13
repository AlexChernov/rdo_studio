/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \authors   �������� ����� (cerevra@gmail.com)
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      05.11.2012
  \brief     ���� �������� �������� ���� ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <conio.h>
#include <QtSql\QtSql>
#include <QtCore\QCoreApplication>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\runtime\src\db\init_struct_db.h"
// --------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	InitSructDB::dropDB();
	InitSructDB::createDB();
	InitSructDB db;

	std::cout << "to be continued...\n";
	getch();
	return 0;
}
