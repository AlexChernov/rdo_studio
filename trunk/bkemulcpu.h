#ifndef BKEMULCPU_H
#define BKEMULCPU_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulCPU
// --------------------------------------------------------------
#define bits_Mask ((WORD)0x0007)  // ����� ��� ��������� ���� ������� ���,

class BKEmulCPU;
typedef void (BKEmulCPU::* CPUcommand)();

class BKEmulCPU
{
private:
	// �������� R0..R7
	std::vector< WORD > regs;

	// ����� ��������� ����������, PS = 0177776
	bool FC;  // ���� �����
	bool FV;  // ���� ����
	bool FZ;  // ���� ������������
	bool FN;  // ���� ��������
	bool FT;  // ���� T - �������
	bool FP1; // ���� ����������
	bool FP2; // ���� ����������
	bool FP3; // ���� ���������� (��������� ���������� � ������� ���������, ��������, ����������)

	WORD MFPS();
	void MTPS( WORD data );

	std::vector< CPUcommand > doCommandGroup_0;
	void BK_doGroup_1();   // 00xxxx
	void BK_doMOV();       // 01ssdd
	void BK_doCMP();       // 02ssdd
	void BK_doBIT();       // 03ssdd
	void BK_doBIC();       // 04ssdd
	void BK_doBIS();       // 05ssdd
	void BK_doADD();       // 06ssdd
	void BK_doGroup_2();   // 07xxxx
	void BK_doGroup_3();   // 10xxxx
	void BK_doMOVB();      // 11ssdd
	void BK_doCMPB();      // 12ssdd
	void BK_doBITB();      // 13ssdd
	void BK_doBICB();      // 14ssdd
	void BK_doBISB();      // 15ssdd
	void BK_doSUB();       // 16ssdd
	void BK_doUnknown();   // 17xxxx

public:
	BKEmulCPU();
	virtual ~BKEmulCPU();

	void reset();
};

} // namespace bkemul

#endif // BKEMULCPU_H
