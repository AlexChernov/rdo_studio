$Pattern �������_�������_������ : irregular_event trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Create
$Time = 3
$Body
_������
	Convert_event
	trace
		��� set ���_������
$End

$Pattern �������_��������_1 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep Keep
	_������: ������ Keep Keep
	_�������: ������� Keep Keep	
$Time = �����_��������(1, ���_������, _������.�����)
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 0 and _������.��� = ���_������
	Convert_begin
		������ set ��������������
	Convert_end
		������ set ��������
		�������� set _������.�������� + 1
_������
	Choice from _������.������ = �������� and �����_��������(1, ���_������, _������.�����) > 0
	Convert_begin
		������ set �����
	Convert_end
		������ set ��������
_�������
	Choice from true
	Convert_end
		������_����_1 set _�������.������_����_1 + ������_������(1, ���_������)
		������_����_2 set _�������.������_����_2 + ������_������(2, ���_������)
		������_����_3 set _�������.������_����_3 + ������_������(3, ���_������)
$End

$Pattern �������_��������_2 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep Keep
	_������: ������ Keep Keep
	_�������: ������� Keep Keep	
$Time = �����_��������(2, ���_������, _������.�����)
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 1 and _������.��� = ���_������
	Convert_begin
		������ set ��������������
	Convert_end
		������ set ��������
		�������� set _������.�������� + 1
_������
	Choice from _������.������ = �������� and �����_��������(2, ���_������, _������.�����) > 0
	Convert_begin
		������ set �����
	Convert_end
		������ set ��������
_�������
	Choice from true
	Convert_end
		������_����_1 set _�������.������_����_1 + ������_������(1, ���_������)
		������_����_2 set _�������.������_����_2 + ������_������(2, ���_������)
		������_����_3 set _�������.������_����_3 + ������_������(3, ���_������)
$End

$Pattern �������_��������_3 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������ : ������  Keep Keep
	_������ : ������  Keep Keep
	_�������: ������� Keep Keep	
$Time = �����_��������(3, ���_������, _������.�����)
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 2 and _������.��� = ���_������
	Convert_begin
		������ set ��������������
	Convert_end
		������ set ��������
		�������� set _������.�������� + 1
_������
	Choice from _������.������ = �������� and �����_��������(3, ���_������, _������.�����) > 0
	Convert_begin
		������ set �����
	Convert_end
		������ set ��������
_�������
	Choice from true
	Convert_end
		������_����_1 set _�������.������_����_1 + ������_������(1, ���_������)
		������_����_2 set _�������.������_����_2 + ������_������(2, ���_������)
		������_����_3 set _�������.������_����_3 + ������_������(3, ���_������)
$End

$Pattern �������_����������_���������� : keyboard trace
$Parameters
	�����_����_������� : integer
$Relevant_resources
	_���������: ���������� NoChange Keep
$Time = 0
$Body
_���������
	Choice from _���������.����� = �����_����_�������
	Convert_end
		�������� set _���������.�������� + 1
$End

$Pattern �������_����������_���������� : keyboard trace
$Parameters
	�����_����_������� : integer
$Relevant_resources
	_���������: ���������� Keep NoChange
$Time = 0
$Body
_���������
	Choice from _���������.����� = �����_����_������� and _���������.�������� > 1
	Convert_begin
		�������� set _���������.�������� - 1
$End
