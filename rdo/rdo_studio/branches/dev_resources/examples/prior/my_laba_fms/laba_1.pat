$Pattern ��������_������ :  operation trace
$Relevant_resources
	����������_	: ���������� Keep NoChange
	������_		: ������ Keep Keep
	�����_		: ������ Keep Keep
	�������_  	: ������� Keep Keep
$Time = ����������������( �����_�������� )
$Body
	����������_
		Choice from ����������_.����� = 1 and ����������_.�������_���������� > 0
		first
		Convert_begin
			�������_���������� set ����������_.�������_���������� - 1
	
	������_
		Choice from ������_.��������� = ����������_1
		first
		Convert_begin
			��������� set �����
			��������� set ����������������
		Convert_end
			��������� set �������_1_�

	�����_
		Choice from �����_.����� = 1 and �����_.��������� = ����������_1 and �����_.��������� = ��������
		first
		Convert_begin
			��������� set �����
			��������� set �����
		Convert_end
			��������� set �������_1_�		
			��������� set ��������
	�������_
		Choice from �������_.����� = 1 and �������_.��������� = �������_1_� and �������_.��������� = ��������
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
$End

$Pattern ��������_������_��_������_������� :  operation trace
$Parameters
	�����_����������   : such_as ����������.���������
	�����_�������      : such_as ����������.���������
	�����_������       : such_as ����������.���������
$Relevant_resources
	����������_	: ���������� Keep NoChange
	������_		: ������ Keep Keep
	�����_		: ������ Keep Keep
	�������_  	: ������� Keep Keep
$Time = ����������������( �����_�������� )
$Body
	����������_
		Choice from ����������_.��������� = �����_���������� and ����������_.�������_���������� > 0
		first
		Convert_begin
			�������_���������� set ����������_.�������_���������� - 1
	
	������_
		Choice from ������_.��������� = �����_����������
		first
		Convert_begin
			��������� set �����
			��������� set ����������������
		Convert_end
			��������� set �����_�������

	�����_
		Choice from �����_.��������� = �����_������ and �����_.��������� = ��������
		first
		Convert_begin
			��������� set �����
			��������� set �����
		Convert_end
			��������� set �����_�������
			��������� set ��������
	�������_
		Choice from �������_.��������� = �����_������� and �������_.��������� = ��������
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
$End

$Pattern ��������_������ :  operation trace
$Parameters
	�����_������� : integer
	���������_���������_���������� : such_as ����������.���������
	��������_���������_����������  : such_as ����������.���������
$Relevant_resources
	�������_  : ������� Keep Keep	
	������_	  : ������ NoChange Keep	
$Time = ����������������( �����_�������� )
$Body
	�������_	  
		Choice from �������_.��������� = ���������_���������_���������� 
				and �������_.��������� = ��������
		first
		Convert_begin
			��������� set ������������
		Convert_end
			��������� set ��������_���������_���������� 
			��������� set ������

	������_
		Choice from ������_.��������� = ���������_���������_����������
		first
		Convert_end
			��������� set ��������_���������_���������� 
$End

$Pattern �������_������ :  operation trace
$Relevant_resources
	�����_	  : ������ Keep Keep	
$Time = �����_��������
$Body
	�����_	  
		Choice from �����_.��������� <> �����_��������(�����_.���������)
				and �����_.��������� = ��������
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
			��������� set �����_��������(�����_.���������)
$End

$Pattern �������_������� :  operation trace
$Parameters
	�����_������� : integer
	���������_���������_���������� : such_as ����������.���������
	��������_���������_����������  : such_as ����������.���������
$Relevant_resources
	�������_  : ������� Keep Keep		
$Time = ����������������( �����_�������� )
$Body
	�������_	  
		Choice from �������_.��������� = ���������_���������_���������� 
				and �������_.��������� = ��������
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
			��������� set ��������_���������_���������� 
$End

$Pattern ���������_��_������ :  operation trace
$Parameters
	�����_������ : such_as ����������.���������
	���������_���������� : such_as ����������.���������
$Relevant_resources	
	������_	  :	������ Keep Keep
	�������_  : ������� Keep Keep		
	�����_	  : ������ Keep Keep	
	������_	  : ������ NoChange Keep
$Time = ����������������( �����_���������_��_������ )
$Body
	������_	  
		Choice from ������_.��������� = �����_������ and ������_.��������� = ��������
		first
		Convert_begin
			��������� set �����������
		Convert_end
			��������� set �����_�_���������

	�������_	  
		Choice from �������_.��������� = ���������_���������� 
				and �������_.��������� = ������
		first
		Convert_begin
			��������� set �������
		Convert_end
			��������� set ��������

	�����_	  
		Choice from �����_.��������� = ���������_���������� 
				and �����_.��������� = ��������
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
			��������� set ������_.���������	   

	������_
		Choice from ������_.��������� = ���������_���������� 
		first
		Convert_end
			��������� set �����_������ 
$End

$Pattern ���������_��_������ :  operation trace
$Parameters
	�����_������ : such_as ����������.���������
$Relevant_resources
	������_	  :	������ Keep Keep
	������_	  : ������ Keep Keep
$Time = ������_.�����_������
$Body
	������_	  
		Choice from ������_.��������� = �����_������ and ������_.��������� = �����_�_���������
		first
		Convert_begin
			��������� set ��������
		Convert_end
			��������� set ��������_���������
	
	������_
		Choice from ������_.��������� = �����_������
		first	
		Convert_begin	
			��������� set ��������������
		Convert_end
			��������� set ���������_���������
$End

$Pattern ���������_������� :  operation trace
$Parameters
	�����_������       : such_as ����������.���������
	�����_����������   : such_as ����������.���������
$Relevant_resources
	������_	     : ������     Keep Keep
	�����_	     : ������     Keep Keep
	����������_  : ���������� NoChange Keep	
	������_	     : ������     Keep Keep
$Time = ����������������( �����_���������_������ )
$Body
	������_	  
		Choice from ������_.��������� = �����_������ and ������_.��������� = ��������_���������
		first
		Convert_begin
			��������� set ������������
		Convert_end
			��������� set ��������
	
	�����_	  
		Choice from �����_.��������� = �������� and �����_������(�����_.�����, ������_.�����) = 1
		first
		Convert_begin
			��������� set �����
		Convert_end
			��������� set ��������
			��������� set ����������_.���������	   
	
	����������_	  
		Choice from ����������_.��������� = �����_���������� and ����������_.�������_���������� < ����������_.������������_����������
		first
		Convert_end
			�������_���������� set ����������_.�������_���������� + 1
	
	������_
		Choice from ������_.��������� = �����_������
		first	
		Convert_begin	
			��������� set ����������������
		Convert_end
			��������� set ����������_.���������
$End

$Pattern ���������_��������� :  operation trace
$Parameters
	�����_������ : such_as ����������.���������
$Relevant_resources
	������_	  	: ������ Keep Keep
	������_	  	: ������ Keep Keep
	����������_ : ����������_2 NoChange Keep
$Time = ����������������( �����_���������_������ )
$Body
	������_	  
		Choice from ������_.��������� = �����_������ and ������_.��������� = ��������_���������
		first
		Convert_begin
			��������� set ������������
		Convert_end
			��������� set ��������

	������_
		Choice from ������_.��������� = �����_������
		first	
		Convert_begin	
			��������� set ����������������
		Convert_end
			��������� set ����������_2
			��������� set ��������


	����������_
		Choice NoCheck
		first	
		Convert_end
			�������_���������� set ����������_.�������_���������� + 1
$End

$Pattern ������_������� : irregular_event trace
$Relevant_resources
	����������	  :	����������_1 Keep
$Time = 0.5
$Body
	����������
		Convert_event
			��������� set ����������.���������
$End
