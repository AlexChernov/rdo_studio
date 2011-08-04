/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_keyboard_i.h
 * @authors   ������ ������
 * @date      01.08.2009
 * @brief     ��������� ������������ ��������
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_KEYBOARD_I_H_
#define _LIB_RUNTIME_KEYBOARD_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
// ********************************************************************************

/******************************************************************************//**
 * @interface IKeyboard
 * @brief     ��������� ������������ ��������
 *********************************************************************************/
class IKeyboard
{
public:
	/**
	 * @enum  AddHotKeyResult
	 * @brief �����c ������� �������
	 * @var   addhk_ok        ������� ������
	 * @var   addhk_already   ������� ��� ������
	 * @var   addhk_notfound  ������� �� �������
	 * @var   addhk_dont      ������� �� ������
	 */
	enum AddHotKeyResult
	{
		addhk_ok,
		addhk_already,
		addhk_notfound,
		addhk_dont
	};
	virtual rbool           hasHotKey() const = 0;
	virtual AddHotKeyResult addHotKey(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(tstring) hotKey) = 0;
};

/******************************************************************************//**
 * @def       DECLARE_IKeyboard
 * @brief     ���������� ���������� ������������ ��������
 *********************************************************************************/
#define DECLARE_IKeyboard \
	virtual rbool           hasHotKey() const; \
	virtual AddHotKeyResult addHotKey(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(tstring) hotKey);

#endif // _LIB_RUNTIME_KEYBOARD_I_H_
