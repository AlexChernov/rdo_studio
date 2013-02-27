/*!
  \copyright (c) RDO-Team, 2012
  \file      interface_db.h
  \authors   �������� ����� (cerevra@gmail.com)
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     ��������� ������ � ��
  \indent    4T
*/

template <class T>
void pushContext           (T context)
{
	pushContxt(bany<T>(context));
}

template <class T>
T popContext            ()
{
	return any_cast<T>(popContxt());
}
