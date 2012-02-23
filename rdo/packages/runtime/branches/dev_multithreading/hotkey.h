/*!
  \copyright (c) RDO-Team, 2012
  \file      hotkey.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_HOTKEY_H_
#define _LIB_RUNTIME_HOTKEY_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� �������
class RDOHotKey
{
public:
	typedef  ruint               KeyCode;

private:
	typedef  std::list<KeyCode>  KeyList;

public:
	enum { UNDEFINED_KEY = ~0 };

	//! ��������������� �����
	class Toolkit
	{
	public:
		//! �����������, ��������� ������� ������
		Toolkit();
		virtual ~Toolkit();

		//! ��������� ��� ������� � ��� �������
		//! @param  keyName - ��� �������
		//! @result ��� ������� � ������������ �������
		KeyCode codeFromString(CREF(tstring) keyName) const;

	private:
		typedef  std::map<tstring, KeyCode>  KeySet;
		KeySet  m_keyList;
	};

	//! ������������ � ������ �������, ������ ����������� � ������ ����������
	class KeyInModelList
	{
	public:
		//! ��������� ������������ � ������ �������
		//! @param  keyCode - ��� �������
		//! @result true, ���� ��� �������� �������
		rbool insert(CREF(KeyCode) keyCode);

		//! ��������� ������� �������
		//! @param  keyCode - ��� �������
		//! @result true, ���� ��� ������ � ������
		rbool check(CREF(KeyCode) keyCode) const;

	private:
		KeyList m_keyList;
	};

	//! ������ ������� ������� ������
	class KeyDownList
	{
	public:
		KeyDownList();
		virtual ~KeyDownList();

		//! ��������� ������� �������
		//! @param  keyCode - ��� �������
		//! @result true, ���� ������� ��������� � ������ �������
		rbool down(CREF(KeyCode) keyCode);

		//! ��������� ������� �������
		//! @param  keyCode - ��� �������
		void up(CREF(KeyCode) keyCode);

		//! ���������, ���� �� ������� � ������ �������
		//! @param  keyCode - ��� �������
		//! @param  shift   - ������� �������� Shift'�
		//! @param  control - ������� �������� Ctrl'�
		//! @result true, ���� ������� ���� � ������ �������
		rbool isPressed(CREF(KeyCode) keyCode, rbool shift, rbool control);

		//! ��������� ������ �������
		//! @result true, ���� \ref down ��� \ref isPressed ������� true � ������ ������ ������
		rbool isFound() const;

		//! ������� ������ ������� ������� ������
		void clear();

	private:
		KeyList m_keyList;
		rbool   m_keyFound;
	};

	//! ������������ ����� ������ �� �������� �������, �������� ������ ������ ������� �������
	class AreaList
	{
	public:
		//! ��������� ������� �� �������
		//! @param areaName - ��� �������
		void  click(CREF(tstring) areaName);

		//! ��������� ���� �� ������� �� �������
		//! @param  areaName - ��� �������
		//! @result true, ���� ���� �������
		rbool check(CREF(tstring) areaName);

		//! ��������� ���� �� ������� ���� �� �� ���� �������
		//! @result true, ���� ���� �������
		rbool empty() const;

		//! ������� ������
		void clear();

	private:
		typedef  std::list<tstring>  NameList;
		NameList  m_activeAreasMouseClicked;
	};

	RDOHotKey();
	virtual ~RDOHotKey();

	//! ������� ������� ������� � �������� �������
	void clear();

	//! ���������� ��������������� �����
	//! @result ��������������� �����
	CREF(Toolkit) toolkit() const;

	//! ���������� ������������ � ������ �������
	//! @result ������������ � ������ �������
	REF(KeyInModelList) keyInModel();

	//! ���������� ������ ������� ������� ������
	//! @result ������ ������� ������� ������
	REF(KeyDownList) keyDown();

	//! ���������� ������ ������� ������� �������� ��������
	//! @result ������ ������� ������� �������� ��������
	REF(AreaList) areaList();

	//! ��������� ���� ������� ������� ��� ������ �������� �������
	//! @result true, ���� ���� ������ ������� ��� �������� �������
	rbool isKeyDown() const;

private:
	Toolkit        m_toolkit;
	KeyInModelList m_keyInModel;
	KeyDownList    m_keyDown;
	AreaList       m_areaList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_HOTKEY_H_
