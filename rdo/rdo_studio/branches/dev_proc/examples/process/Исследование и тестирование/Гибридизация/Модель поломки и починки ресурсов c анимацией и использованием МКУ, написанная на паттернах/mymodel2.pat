$Pattern �������_��������_��������� : irregular_event trace
$Relevant_resources
	�����_���������		:������ 	Create
	�������_����������	:�����	Keep
$Time = ��������_�������_���������(50)
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
	�����_		: �������	 keep 		keep
	������		: ������	 keep	keep
	����������1	: ���������� keep 		NoChange
	����������2	: ���������� NoChange 	keep
$Time = �����_�������(8,1) + �����_��_�������(����������1.���������) + �����_�������(8,1)
$Body
�����_	
	Choice from �����_.������ = 5 and �����_.��������� = ��������
	Convert_begin
		��������� set �����_���������(������.���������)
		��������� set �����
	Convert_end
		��������� set �����_���������_�������(�����_.���������)
������
	Choice from ������.��������� = ����� or ������.��������� = ��������1 or ������.��������� = ��������2
	with_min	������.�����
	Convert_begin
		��������� set �����_.���������
	Convert_end 
		��������� set ����������_����������(������.���������, �����_.���������)
		�����	  set Time_now
����������1
	Choice from ����������1.��������� = ������.��������� and ����������1.�����_������� > 0
	Convert_begin
		�����_������� set ���������_����������(����������1.�����_�������, �����_.���������)

����������2
	Choice from ����������2.��������� = �����_����������(����������1.���������)
	Convert_end
		�����_������� set ��������_����������(����������2.�����_�������, �����_.���������)
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
		��������� set �����_���������_�������(������.���������)

����������1
	Choice from ����������1.��������� = �������1 and ����������1.�����_������� > 0
	Convert_begin
		�����_������� set ���������_����������(����������1.�����_�������, ������.���������)
	
����������2
	Choice from ����������2.��������� = ��������1 
	Convert_end
		�����_������� set ��������_����������(����������2.�����_�������, ������.���������)
		
������
	Choice from ������.��������� = �������1 
	with_min	������.�����
	Convert_begin
		��������� set ������.���������
	Convert_end 
		��������� set ����������_����������(������.���������, ������.���������)
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
		��������� set �����_���������_�������(������.���������)

����������1
	Choice from ����������1.��������� = �������2
	Convert_begin
		�����_������� set ���������_����������(����������1.�����_�������, ������.���������)
����������2
	Choice from ����������2.��������� = ��������2
	Convert_end
		�����_������� set ��������_����������(����������2.�����_�������, ������.���������)
		
������
	Choice from ������.��������� = �������2 
	with_min	������.�����
	Convert_begin
		��������� set ������.���������
	Convert_end 
		��������� set ����������_����������(������.���������, ������.���������)
		�����	  set Time_now
$End

$Pattern  �������_�������_������� : keyboard
$Parameters
	������: integer
$Relevant_resources
	������ : ������� Keep NoChange
$Time = 0.0
$Body
������
	Choice from ������.������ = ������
	Convert_begin
		��������� set �����_���������_�������(������.���������)
$End

