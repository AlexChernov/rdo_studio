$Sequence ��������_�������_��������� : real
$Type = exponential 12347
$End

$Sequence ���������_��_������_1 : real
$Type = normal 12346
$End


$Sequence �����_������� : real
$Type = uniform 67891
$End


$Sequence �����_������� : real
$Type = uniform 67892
$End


$Sequence ���������_��_������_2 : real
$Type = exponential 12345
$End


$Function �����_���������� : such_as ����������.��������� = �����
$Type = list
$Parameters
	�������_��������� : such_as ����������.���������
$Body
	����� 	  = �������1 
	��������1 = �������2 
	��������2 = ������	
$End

$Function �����_��������� : such_as ������.��������� = �����
$Type = list
$Parameters
	�������_��������� : such_as ������.���������
$Body
	�����     = state1 
	��������1 = state2 
	��������2 = state3	
$End