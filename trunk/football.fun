$Constant
	������: real = 40
	������: real = 40
	pi    : real = 3.14
$End

$Function ����������_����: real
$Type = algorithmic
$Parameters
	�������_����: real
$Body
	Calculate_if �������_���� < 0   ����������_���� = �������_���� + 360
	Calculate_if �������_���� > 360 ����������_���� = �������_���� - 360
	Calculate_if 1 = 1              ����������_���� = �������_����
$End

$Function �_�����_�������_�������: integer
$Type = algorithmic
$Parameters
	�������_�������  : real
	���������_�������: real
$Body
	Calculate_if ���������_������� = �������_������� �_�����_�������_������� =  0
	Calculate_if ���������_������� > �������_������� and ���������_������� - �������_������� >= 180 �_�����_�������_������� = -1
	Calculate_if ���������_������� > �������_������� and ���������_������� - �������_������� <  180 �_�����_�������_������� =  1
	Calculate_if ���������_������� < �������_������� and ���������_������� - �������_������� >= 180 �_�����_�������_������� =  1
	Calculate_if ���������_������� < �������_������� and ���������_������� - �������_������� <  180 �_�����_�������_������� = -1
$End

$Function ���_������������: such_as ������.��������
$Type = algorithmic
$Parameters
	�������_�������  : real
	���������_�������: real
$Body
	Calculate_if �������_������� = ���������_������� ���_������������ = ���
	Calculate_if 1 = 1                               ���_������������ = �������
$End

$Function ���_���������: such_as ������.��������
$Type = algorithmic
$Parameters
	�������_x  : real
	�������_y  : real
	���������_x: real
	���������_y: real
$Body
	Calculate_if abs(�������_x - ���������_x) < 2 and abs(�������_y - ���������_y) < 2 ���_��������� = ���
	Calculate_if 1 = 1                                                                 ���_��������� = ��������
$End

$Function ���������_����������: real
$Type = algorithmic
$Parameters
	�������: real
	����   : real
	������ : real
$Body
	Calculate_if abs(���� + ������ - �������) < 1 ���������_���������� = 0
	Calculate_if 1 = 1                            ���������_���������� = ������
$End

$Function �����������_��������: real
$Type = algorithmic
$Parameters
	�������_x  : real
	���������_x: real
$Body
	Calculate_if �������_x >= ���������_x �����������_�������� = 1
	Calculate_if 1 = 1                    �����������_�������� = 1
$End

$Function ����_�����: real
$Type = algorithmic
$Parameters
	�������_x  : real
	�������_y  : real
	���������_x: real
	���������_y: real
$Body
	Calculate_if abs(�������_y - ���������_y) < 0.0001 and �������_x <= ���������_x ����_����� = pi / 2
	Calculate_if abs(�������_y - ���������_y) < 0.0001 and �������_x  > ���������_x ����_����� = pi * 3 / 2
	Calculate_if 1 = 1                                                              ����_����� = pi / 2 - (���������_x - �������_x)/(���������_y - �������_y)
$End
