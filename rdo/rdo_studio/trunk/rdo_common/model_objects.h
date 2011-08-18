/*!
  \copyright (c) RDO-Team, 2011
  \file      model_objects.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     
  \indent    4T
*/

#ifndef _LIB_COMMON_MODEL_OBJECTS_H_
#define _LIB_COMMON_MODEL_OBJECTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! \namespace rdoModelObjects
//! \brief     ������������ ��� �������� ������
#define OPEN_RDO_MODEL_OBJECTS_NAMESPACE	namespace rdoModelObjects {
#define CLOSE_RDO_MODEL_OBJECTS_NAMESPACE	};

OPEN_RDO_MODEL_OBJECTS_NAMESPACE

	//! \details ���� ������
	enum RDOFileType
	{
		RTP = 0, //!< ���� ����� ��������
		RSS,     //!< ���� ��������
		EVN,     //!< ���� �������
		PAT,     //!< ���� ���������
		DPT,     //!< ���� ����� �������� �������
		PRC,     //!< ���� ���������
		FRM,     //!< ���� ��������
		FUN,     //!< ���� �������
		SMR,     //!< ���� �������
		PMD,     //!< ���� �������� �����������
		PMV,     //!< ���� �� ���������� �����������
		TRC,     //!< ���� �����������
		RDOX     //!< ���� �������
	};

	//! \details ���� �������� �������, ������������ ����� ������� �����
	enum RDOParseType
	{
		obNONE = 0x0000, //!< ��� ����������� (����� ���)
		obPRE  = 0x0001, //!< ��������������� ����������
		obRTP  = 0x0002, //!< ���������� ����� ��������
		obRSS  = 0x0004, //!< ���������� ��������
		obFUN  = 0x0008, //!< ���������� �������
		obEVN  = 0x0010, //!< ���������� �������
		obPAT  = 0x0020, //!< ���������� ���������
		obDPT  = 0x0040, //!< ���������� ����� �������� �������
		obPRC  = 0x0080, //!< ���������� ���������
		obPMD  = 0x0100, //!< ���������� �����������
		obFRM  = 0x0200, //!< ���������� ��������
		obSMR  = 0x0400, //!< ���������� �������
		obPOST = 0x0800, //!< ����-����������
		obALL  = 0x0FFF  //!< ��� ���� (����� ��������� ����������)
	};
CLOSE_RDO_MODEL_OBJECTS_NAMESPACE

#endif //! _LIB_COMMON_MODEL_OBJECTS_H_
