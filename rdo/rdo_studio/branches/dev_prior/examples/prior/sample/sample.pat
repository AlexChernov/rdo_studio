$Pattern �������_�������_������ : irregular_event trace
$Relevant_resources
	_������: ������ Create
$Time = 0
$Body
_������
	Convert_event
	trace
		��� set ��������_����
$End

$Pattern �������_��������_1 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep Keep
	_������: ������ Keep Keep
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
$End

$Pattern �������_��������_2 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep Keep
	_������: ������ Keep Keep
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
$End

$Pattern �������_��������_3 : operation trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep Keep
	_������: ������ Keep Keep
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
$End

$Pattern �������_��������_��������_1 : rule trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 0 and _������.��� = ���_������ and For_All(������: (�����_��������(1, ���_������, ������.�����) = 0) )
	Convert_rule
		�������� set _������.�������� + 1
$End

$Pattern �������_��������_��������_2 : rule trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 1 and _������.��� = ���_������ and For_All(������: (�����_��������(2, ���_������, ������.�����) = 0) )
	Convert_rule
		�������� set _������.�������� + 1
$End

$Pattern �������_��������_��������_3 : rule trace
$Parameters
	���_������: integer
$Relevant_resources
	_������: ������ Keep
$Body
_������
	Choice from _������.������ = �������� and _������.�������� = 2 and _������.��� = ���_������ and For_All(������: (�����_��������(3, ���_������, ������.�����) = 0) )
	Convert_rule
		�������� set _������.�������� + 1
$End
