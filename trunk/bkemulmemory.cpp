#include "stdafx.h"
#include "bkemulmemory.h"
#include "bkemul.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKMemoryAccessError
// --------------------------------------------------------------
IMPLEMENT_DYNAMIC( BKMemoryAccessError, CException )

BKMemoryAccessError::BKMemoryAccessError( const WORD _address, const WORD _data, const bool _isByte ):
	CException(),
	address( _address ),
	data( _data ),
	isByte( _isByte )
{
};

int BKMemoryAccessError::ReportError( UINT nType, UINT nMessageID ) {
	std::string s;
	if ( isByte ) {
		BYTE byte = data;
		s = format( IDS_MEMORYACCESSERROR_BYTE, byte, address );
	} else {
		s = format( IDS_MEMORYACCESSERROR_WORD, data, address );
	}
	return AfxMessageBox( s.c_str(), nType | MB_ICONERROR );
}

// --------------------------------------------------------------
// ---------- BKEmulMemory
// --------------------------------------------------------------
BKEmulMemory::BKEmulMemory()
{
	memory.resize( 64 * 1024 );
	std::vector< unsigned char >::iterator it = memory.begin();
	while ( it != memory.end() ) {
		(*it++) = 0xFF;
	}
}

BKEmulMemory::~BKEmulMemory()
{
}

BYTE BKEmulMemory::getMemoryByte( WORD address )
{
	switch ( address ) {
		// � �������� ��������� ���������� �� ������ �������� 6 � 7 ����, ��������� �������� ������
		case 0177660: return get_byte( address ) & static_cast<BYTE>(0300);
		// � �������� ������ ���������� �������� ���� 0-6, ��������� (7-15) �������� ������
		case 0177662: {
			BYTE res = get_byte( address ) & static_cast<BYTE>(0177);
			// �� ������� ������ ���������� �������� ��� ������� (������� 177660, ��� 0200 = 0 - ��������, 1 - ��������)
			set_word( 0177660, get_word( 0177660 ) & 0177577 );
			return res;
		}
		// � �������� �������� �������� 0-7 � 9 ����, ��������� �� ������������ (�������� ������ ????)
		case 0177664: return get_byte( address ) & static_cast<BYTE>(0377);
		case 0177665: return get_byte( address ) & static_cast<BYTE>(0002);
		// � �������� ���������� �������� ���� 8-15 �������� ���������
		case 0177713: return get_byte( address ) & static_cast<BYTE>(0377);
		// ������� �������� ���������� �������� �� ������
		case 0177716: return static_cast<BYTE>(R_177716_read);
		case 0177717: return 0;
		// ������� ������������� ���������������� ���������� �������� ������� ��������
		case 0177714: return 0;
		case 0177715: return 0;
	}
	return get_byte( address );
}

WORD BKEmulMemory::getMemoryWord( WORD address )
{
	address &= oddWordMask;
	WORD data = get_word( address );
	switch ( address ) {
		// � �������� ��������� ���������� �������� 6 � 7 ����, ��������� �������� ������
		case 0177660: return data & static_cast<WORD>(0000300);
		// � �������� ������ ���������� �������� ���� 0-6, ��������� (7-15) �������� ������
		case 0177662: {
			data &= static_cast<WORD>(0000177);
			// �� ������� ������ ���������� �������� ��� ������� (������� 177660, ��� 0200 = 0 - ��������, 1 - ��������)
			set_word( 0177660, get_word( 0177660 ) & 0177577 );
			return data;
		}
		// � �������� �������� �������� 0-7 � 9 ����, ��������� �� ������������ (�������� ������ ????)
		case 0177664: return data & static_cast<WORD>(0001377);
		// � �������� ���������� �������� ���� 8-15 �������� ���������
		case 0177712: return data | static_cast<WORD>(0xFF00);
		// ������� �������� ���������� �������� �� ������
		case 0177716: return R_177716_read;
		// ������� ������������� ���������������� ���������� �������� ������� ��������
		case 0177714: return 0;
	}
	return data;
}

void BKEmulMemory::setMemoryByte( WORD address, BYTE data )
{
	// ������ � ���
	if ( ( address >= 0100000 && address < 0176560 ) ||
		 ( address >= 0176570 && address < 0177660 ) ||
		 ( address >  0177665 && address < 0177706 ) ||
		 ( address >  0177717) ) {
		throw new BKMemoryAccessError( address, data );
	}

	switch ( address ) {
		// � �������� ��������� ���������� �� ������ �������� ������ 6 ���
		case 0177660: {
			data = (get_byte( address ) & static_cast<BYTE>(0200)) | static_cast<BYTE>(data & 0100);
			set_byte( address, data );
			break;
		}
		case 0177661: set_byte( address, 0 ); break;
		// � ��������� �������� �� ������ �������� ������ 4-7 ������� ��������� �������� 177716
		case 0177716: {
			R_177716_write &= 0xFF0F;
			R_177716_write |= static_cast<BYTE>(data & 0xF0);
			emul.doSpeaker();
			break;
		}
		case 0177717: R_177716_write &= 0x00F0; break;
		// ������� ��������
		case 0177664:
		case 0177665: {
			// �������� ������ 0-7 � 9 ���� ����� �������� ��������
			if ( address == 0177665 ) {
				data &= 0002;
			}
			WORD R_177664_prev_value = get_word( 0177664 ) & 0001377;
			set_byte( address, data );
			// �������� ����������, ���������� ����������� ������
			WORD value = get_word( 0177664 ) & 0001377;
			if ( value != R_177664_prev_value ) {
				if ( (value & 01000) != (R_177664_prev_value & 01000) ) {
					// ������������ ���� �����, �.�. �������� ����� ������ (������ <=> 1/4)
					emul.video.updateMonitor();
				} else {
					// ������������ ������ �������� �����
					emul.video.updateScrolling( static_cast<BYTE>(value - R_177664_prev_value) );
				}
			}
			break;
		}
		// ������� ���������� ��������
		case 0177712:
		case 0177713: {
			// ������ � 8-15 ���� ����� ������������
			if ( address == 0177713 ) {
				data = 0xFF;
			}
			set_word( 0177710, get_word( 0177706 ) );
			// ������/���������� ������
			emul.BK_SYS_Timer_work = data & 020 ? true : false;
			set_byte( address, data );
			break;
		}
		default: set_byte( address, data );
	}

	// ������ � �������
	if ( address >= 0040000 && address < 0100000 ) {
		emul.video.updateVideoMemoryByte( address );
	}
}

void BKEmulMemory::setMemoryWord( WORD address, WORD data )
{
	address &= oddWordMask;

	// ������ � ���
	if ( ( address >= 0100000 && address < 0176560 ) ||
	     ( address >= 0176570 && address < 0177660 ) ||
	     ( address >  0177665 && address < 0177706 ) ||
	     ( address >  0177717 ) ) {
		throw new BKMemoryAccessError( address, data, false );
	}

	WORD oldData = get_word( address );
	switch ( address ) {
		// � �������� ��������� ���������� �� ������ �������� ������ 6 ���
		case 0177660: {
			data = ( oldData & 0000200 ) | ( data & 0000100 );
			set_word( address, data );
			break;
		}
		// � ��������� �������� �� ������ �������� ������ 4-7 ������� ��������� �������� 177716
		case 0177716: {
			R_177716_write &= 0xFF0F;
			R_177716_write |= data & 0x00F0;
			emul.doSpeaker();
			break;
		}
		// ������� ��������
		case 0177664: {
			// �������� ������ 0-7 � 9 ����
			data &= 0001377;
			WORD R_177664_prev_value = oldData & 0001377;
			set_word( address, data );
			// �������� ����������, ���������� ����������� ������
			if ( data != R_177664_prev_value ) {
				if ( (data & 01000) != (R_177664_prev_value & 01000) ) {
					// ������������ ���� �����, �.�. �������� ����� ������ (������ <=> 1/4)
					emul.video.updateMonitor();
				} else {
					// ������������ ������ �������� �����
					emul.video.updateScrolling( static_cast<BYTE>(data - R_177664_prev_value) );
				}
			}
			break;
		}
		// � ������� �������� ������� ������ ������������
		case 0177710: break;
		// ������� ���������� ��������
		case 0177712: {
			// ������ � 8-15 ���� ������������
			data |= 0xFF00;
			set_word( 0177710, get_word( 0177706 ) );
			// ������/���������� ������
			emul.BK_SYS_Timer_work = data & 0000020 ? true : false;
			set_word( address, data );
			break;
		}
		default: set_word( address, data );
	}

	// ������ � �������
	if ( address >= 0040000 && address < 0100000 ) {
		emul.video.updateVideoMemoryWord( address );
	}
}
