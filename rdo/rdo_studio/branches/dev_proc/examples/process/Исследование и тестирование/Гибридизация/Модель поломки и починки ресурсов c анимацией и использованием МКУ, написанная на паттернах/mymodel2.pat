$Pattern �������_��������_��������� : irregular_event trace
$Relevant_resources
	�����_���������		:������ 	Create
	�������_����������	:�����	Keep
$Time = ��������_�������_���������(150)
$Body
�����_���������
	Convert_event trace
		��������� 	 set �����
		�����		 set Time_now
		
�������_����������	
	Convert_event 
		�����_������� set �������_����������.�����_������� + 1
$End

$Pattern �������_������_������ : operation
$Relevant_resources
	������		: ������	 NoChange	keep
	����������1	: ���������� keep 		NoChange
	����������2	: ���������� NoChange 	keep
	�����		: �������	 keep 		keep
$Time = �����_�������(8,1) + �����_��_�������(����������1.���������) + �����_�������(8,1)
$Body
������
	Choice from ������.��������� = ����� or ������.��������� = ��������1 or ������.��������� = ��������2
	with_min	������.�����
	Convert_end 
		��������� set �����_����������(������.���������)
		�����	  set Time_now

����������1
	Choice from ����������1.��������� = ������.��������� and ����������1.�����_������� > 0
	Convert_begin
		�����_������� set ����������1.�����_������� - 1

����������2
	Choice from ����������2.��������� = �����_����������(����������1.���������)
	Convert_end
		�����_������� set ����������2.�����_������� + 1

�����	
	Choice from �����.������ = 5 and �����.��������� = ��������
	Convert_begin
		��������� set �����_���������(������.���������)
		��������� set �����
	Convert_end
		��������� set ��������
$End

$Pattern �������_������_��1 : operation
$Relevant_resources
	������		: �������	 keep 		keep
	����������1	: ���������� keep 		NoChange
	����������2	: ���������� NoChange 	keep
	������		: ������	 keep 		keep
$Time = ���������_��_������_1(60,10)
$Body
������	
	Choice from ������.��������� = �������� and (������.������ = 1 or ������.������ = 2 or ������.������ = 3)
	Convert_begin
		��������� set �����
	Convert_end
		��������� set ��������

����������1
	Choice from ����������1.��������� = �������1 and ����������1.�����_������� > 0
	Convert_begin
		�����_������� set ����������1.�����_������� - 1
	
����������2
	Choice from ����������2.��������� = ��������1 
	Convert_end
		�����_������� set ����������2.�����_������� + 1
		
������
	Choice from ������.��������� = �������1 
	with_min	������.�����
	Convert_begin
		��������� set ������.���������
	Convert_end 
		��������� set ��������1
		�����	  set Time_now
$End

$Pattern �������_������_��2 : operation
$Relevant_resources
	������		: �������	 keep 		keep
	����������1	: ���������� keep 		NoChange
	����������2	: ���������� NoChange 	keep
	������		: ������	 keep 		keep
$Time = ���������_��_������_2(100)
$Body
������	
	Choice from ������.��������� = �������� and ������.������ = 4
	Convert_begin
		��������� set �����
	Convert_end
		��������� set ��������

����������1
	Choice from ����������1.��������� = �������2
	Convert_begin
		�����_������� set ����������1.�����_������� - 1
	
����������2
	Choice from ����������2.��������� = ��������2
	Convert_end
		�����_������� set ����������2.�����_������� + 1
		
������
	Choice from ������.��������� = �������2 
	with_min	������.�����
	Convert_begin
		��������� set ������.���������
	Convert_end 
		��������� set ��������2
		�����	  set Time_now
$End




$Pattern  �������_�������_������ : keyboard
$Parameters
	������: integer
$Relevant_resources
	������ : ������� Keep NoChange
$Time = 0.0
$Body
������
	Choice from ������.������ = ������
	Convert_begin
		��������� set �����_���������11(������.���������)
$End

$Pattern  �������_�������_������2 : keyboard
$Relevant_resources
	������ : ��2	Keep NoChange
$Time = 0.0
$Body
������
	Convert_begin
		��������� set �����_���������2(������.���������)
$End

$Pattern  �������_�������_������ : keyboard 
$Relevant_resources
	����� : �����	Keep NoChange
$Time = 0.0
$Body
�����
	Convert_begin
		��������� set �����_���������_������(�����.���������)
$End

/*
$Pattern  �������_������� : keyboard
$Relevant_resources
	rel_ST11 : ST11	Keep NoChange
$Time = 0.0
$Body
	rel_ST11
	Choice from rel_ST11.��������� = ������
	first
	Convert_begin
		��������� set ��������
$End*/