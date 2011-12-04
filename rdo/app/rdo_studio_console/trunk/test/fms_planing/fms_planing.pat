/*$Pattern  qwerty : operation
$Relevant_resources
	_������         :  ������          Keep Keep 
	_������         :  ������          Keep Keep 
$Time = 10
$Body
	_������
	Choice from _������.��������� == ������
	first
	Convert_begin
			��������� = �_���������;
	Convert_end
			�����_��������++;

	_������
	Choice from _������.���������_������ == �������� and ���_������ (_������.�����_��������, _������.�����) == _������.���
	first
	Convert_begin 
		���������_������ = �����;		
	Convert_end 
		���������_������ = ��������;	
$End*/

$Pattern �������_���������_�_�����������1 : operation  // ��������� � �����������
$Relevant_resources
	_������         :  ������          Keep Keep 
	_������         :  ������          Keep Keep 
	_�������������� :  ��������������  Keep Keep  
$Time = ������������_����������� (_������.�����, _������.�����_�������_������)
$Body

    _������
	Choice from _������.��������� == �������� 
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = ���������_�����������;		 
	Convert_end
		��������� = �����������;
		
    _������
	Choice from _������.���������_������ == �������� and ���_������ (_������.�����_��������, _������.�����) == _������.��� and ��_�����_�����_�������� (time_now) >= ������������_����������� (_������.�����, _������.�����_�������_������)
	with_min (_������.����������_������������ )  //first 	
	Convert_begin 
		���������_������ = ���������_�����������;
		�����_�������_������ = _������.�����;
		�����_�������_�������� = _������.�����_��������;		
	Convert_end 
		���������_������ = �����������;	
		
_��������������
	Choice from _��������������.��������� == �������� and _��������������.�����_�������������_������ == _������.����� 
	Convert_begin
		��������� = ���������_�����������;		
	Convert_end
		��������� = �����������;		
$End

$Pattern �������_���������_�_�����������2 : operation  // ��������� � ����������� �� ����������, ��� ��� ����� �� ��� �������� > ��� �����, ���������� �� ����� �����
$Relevant_resources
	_������         :  ������          Keep Keep 
	_������         :  ������          Keep Keep 
	_�������������� :  ��������������  Keep Keep 
$Time = ��_�����_�����_�������� (time_now) - 30 
$Body

_������
	Choice from _������.��������� == �������� 
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = �������;		 
	Convert_end
		��������� = ��������;
		
_������
	Choice from _������.���������_������ == �������� and ���_������ (_������.�����_��������, _������.�����) == _������.��� and ��_�����_�����_�������� (time_now) < ������������_����������� (_������.�����, _������.�����_�������_������)
	with_min (_������.����������_������������ )  //first 	
	Convert_begin 
		���������_������ = �������;	
		�����_�������_������ = _������.�����;
		�����_�������_�������� = _������.�����_��������;		
	Convert_end 
		���������_������ = ��������;	
		
_��������������
	Choice from _��������������.��������� == �������� and _��������������.�����_�������������_������ == _������.�����
	Convert_begin
		��������� = �������;		
	Convert_end 
		��������� = ��������;		
$End

$Pattern �������1_���������_������ : operation // ���������� � ��� ������, ����� ����� ��������� ������, ��� ����� �� ����� �����
$Relevant_resources
	_������          :  ������          Keep Keep // ��������� 
	_������          :  ������          Keep Keep 
	_��������������  :  ��������������  Keep Keep // ��������� 
$Time = ������������_��������� (_������.�����_��������, _������.�����)
$Body

_������
	Choice from _������.��������� == ����������� and ��_�����_�����_�������� (time_now) >= ������������_��������� (_������.�����_��������, _������.�����)
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = �_���������;		
	Convert_end
		��������� = ������;
		�����_��������++;
		
_������
	Choice from _������.���������_������ == ����������� and _������.��� == ���_������ (_������.�����_��������, _������.�����)
	with_min (_������.����������_������������ )  //first 	
	Convert_begin
		���������_������ = �����;	
		�����_�������_������ = _������.�����;
		�����_�������_�������� = _������.�����_��������;
	Convert_end  
		���������_������ = ��������;
		����������_������������++; 
		
_��������������
	Choice from _��������������.��������� == ����������� and _��������������.�����_�������������_������  == _������.�����
	Convert_begin
		��������� = ���������;		
	Convert_end
		��������� = ��������;
$End

$Pattern �������1_�������_������ : operation // ���������� � ��� ������, ����� ����� ��������� ������, ��� ����� �� ����� �����
$Relevant_resources
	_������         :  ������          Keep Keep // �������
	_�������������� :  ��������������  Keep Keep // �������  
$Time =  ������������_������� (_������.�����) 
$Body
		 
_������
	Choice from _������.��������� == ������
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = �_�������;
	Convert_end 
		��������� = ��������; 
		
_��������������
	Choice from _��������������.��������� == �������� and _��������������.��� == ���_������ (_������.�����_��������, _������.�����)
	first
	Convert_begin
		��������� = �������;		
	Convert_end
		��������� = ��������;
		�����_�������������_������ = _������.�����; 

$End

$Pattern �������2_���������_������ : operation // ���������� � ��� ������, ���� ����� �� ��������� ������ �������, ����������� �� ����� �����, � ����� �� ����������� - ������
$Relevant_resources											 // ������� ����������, �� ������ �����������
	_������         :  ������          Keep Keep // ���������
	_������         :  ������          Keep Keep 
	_�������������� :  ��������������  Keep Keep // ���������
$Time = 0                                 
$Body

_������
	Choice from _������.��������� == ����������� and ��_�����_�����_�������� (time_now) <= ������������_��������� (_������.�����_��������, _������.�����) 
	first
	Convert_begin 
		��������� = �������;	 
	Convert_end
		��������� = �����������;    // �����������
		
_������
	Choice from _������.���������_������ == ����������� and _������.��� == ���_������ (_������.�����_��������, _������.�����)
	with_min (_������.����������_������������ )  //first 	
	Convert_begin
		���������_������ = �������;	
		�����_�������_������ = _������.�����;
		�����_�������_�������� = _������.�����_��������;		
	Convert_end  
		���������_������ = �����������;     // �����������	
		
_��������������
	Choice from _��������������.��������� == ����������� and _��������������.�����_�������������_������  == _������.�����
	Convert_begin
		��������� = �������;		
	Convert_end
		��������� = �����������;     // ����������� 		

$End

$Pattern �������2_�������_������ : operation // ���������� � ��� ������, ���� ����� �� ��������� ������ �������, ����������� �� ����� �����, � ����� �� ����������� - ������
$Relevant_resources											 // ������� ����������, �� ������ �����������
	_������         :  ������          Keep Keep // �������
	_�������������� :  ��������������  Keep Keep // �������  
$Time =  ������������_������� (_������.�����)                                     
$Body

_������
	Choice from _������.��������� == ������ and ��_�����_�����_�������� (time_now) >= ������������_������� (_������.�����)
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = �_�������;		
	Convert_end
		��������� = ��������; 
		
_��������������
	Choice from _��������������.��������� == �������� and _��������������.��� == ���_������ (_������.�����_��������, _������.�����)
	first
	Convert_begin
		��������� = �������;		
	Convert_end
		��������� = ��������;
		�����_�������������_������ = _������.�����; 		

$End

$Pattern �������3_���������_������ : operation // ���������� � ��� ������, ���� ����� �� ������� ������, ��� �������� ������� �� ����� �����
$Relevant_resources											 // �� ���������� �� �������, �� ���������
	_������         :  ������          Keep Keep // ���������
	_������         :  ������          Keep Keep 
	_�������������� :  ��������������  Keep Keep // ���������
$Time =  ��_�����_�����_�������� (time_now) - 30                                   
$Body
 
_������
	Choice from _������.��������� == ����������� 
	first
	Convert_begin 
		��������� = �������;	 
	Convert_end
		��������� = �����������;  // �����������

_������
	Choice from _������.���������_������ == ����������� and _������.��� == ���_������ (_������.�����_��������, _������.�����)
	with_min (_������.����������_������������ )  //first 	
	Convert_begin
		���������_������ = �������;
		�����_�������_������ = _������.�����;
		�����_�������_�������� = _������.�����_��������;		
	Convert_end  
		���������_������ = �����������;  // �����������
		
_��������������
	Choice from _��������������.��������� == ����������� and _��������������.�����_�������������_������  == _������.�����
	Convert_begin
		��������� = �������;		
	Convert_end
		��������� = �����������;     // �����������

$End


$Pattern �������3_�������_������ : operation // ���������� � ��� ������, ���� ����� �� ������� ������, ��� �������� ������� �� ����� �����
$Relevant_resources											 // �� ���������� �� �������, �� ���������
	_������         :  ������          Keep Keep // �������
	_�������������� :  ��������������  Keep Keep // �������  
$Time =  ��_�����_�����_�������� (time_now) - 30                                   
$Body
		
_������
	Choice from _������.��������� == ������ and ��_�����_�����_�������� (time_now) <= ������������_������� (_������.�����)
	with_min (_������.�������_�_�������)
	Convert_begin 
		��������� = �������;		
	Convert_end
		��������� = ������;    // �����������
		
_��������������
	Choice from _��������������.��������� == �������� and _��������������.��� == ���_������ (_������.�����_��������, _������.�����)
	first
	Convert_begin
		��������� = �������;		
	Convert_end 
		��������� = ��������;

$End
$Pattern  �������_�����_������: rule
$Relevant_resources
	_������  :  ������   Erase
$Body
_������  
	Choice from ���_������(_������.�����_��������, _������.�����) == ���
	Convert_rule
$End 
