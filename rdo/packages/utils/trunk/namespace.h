/*!
  \copyright (c) RDO-Team, 2011
  \file      utils/namespace.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     ���������� namespace'��
  \indent    4T
*/

#ifndef _UTILS_NAMESPACE_H_
#define _UTILS_NAMESPACE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! \namespace rdo
//! \brief     ������� ������������ ���
#define OPEN_RDO_NAMESPACE					namespace rdo {
#define CLOSE_RDO_NAMESPACE					};

//! \namespace rdoRuntime
//! \brief     ������������ ��� ��������
#define OPEN_RDO_RUNTIME_NAMESPACE			namespace rdoRuntime {
#define CLOSE_RDO_RUNTIME_NAMESPACE			};

//! \namespace rdoSimulator
//! \brief     ������������ ��� ����������
#define OPEN_RDO_SIMULATOR_NAMESPACE		namespace rdoSimulator {
#define CLOSE_RDO_SIMULATOR_NAMESPACE		};

//! \namespace rdoAnimation
//! \brief     ������������ ��� ��������
#define OPEN_RDO_ANIMATION_NAMESPACE		namespace rdoAnimation {
#define CLOSE_RDO_ANIMATION_NAMESPACE		};

#endif // _UTILS_NAMESPACE_H_
