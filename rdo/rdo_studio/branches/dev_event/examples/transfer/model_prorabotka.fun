$Constant
	������ : real = 60.0
	������_�_�������: integer = 60
$End

$Sequence �_������_������: integer [0..3000]
$Type = normal 3487478
$End

$Function �_����������_�_����������_��������_������: integer = 0
$Type = algorithmic
$Parameters
	��������: real
$Body
	if (Round(��������) <  ��������) result = Round(��������) + 1
	if (Round(��������) >= ��������) result = Round(��������)
$End

$Sequence �_���_����� : real 
$Type = uniform 34556
$End

$Sequence �_���_��������: such_as ������.�������
$Type = by_hist 7833
$Body
    1 2 5.0
	2 3 2.0
	3 3 3.0
$End

$Sequence �_��������_������� : real[0..100000]
$Type = normal 235
$End

$Sequence �_�������� : real[0..100000]
$Type = normal 768434
$End

$Sequence �_�����_�_���� : real[0..100000]
$Type = normal 0876543
$End

$Sequence �_�����������_������� : real[0..100000]
$Type = normal 545478
$End

$Sequence �_�����_����� : real[0..100000]
$Type = normal 54747848
$End

$Sequence �_��������� : real[0..100000]
$Type = normal 321258
$End

$Sequence �_����� : real[0..100000]
$Type = normal 45789211
$End

$Sequence �_��������_�������_�_���� : real[0..100000]
$Type = normal 80504030890
$End