$Pattern �������_������_���������_������_��_������: rule trace
$Parameters
	�����_������    : integer
$Relevant_resources
	_������: ������ Keep
	_������: ������ Keep
$Body
_������
	Choice from _������.��������� = �������� and _������.����� = �����_������
	Convert_rule
		��������� set ������
_������
	Choice from _������.����� = _������.���������� + 1 and _������.��������� = ��������
	Convert_rule
		��������� set �����
		�����_������_��������� set �������.time
		�����_�����_���������  set �������.time + �����(_������.�����, _������.�����)
$End

$Pattern �������_���������_���������_������_��_������: rule trace
$Parameters
	�����_������    : integer
$Relevant_resources
	_������ : ������  Keep
	_������ : ������  Keep
$Body
_������
	Choice from _������.��������� = ������ and _������.����� = �����_������
	Convert_rule
		��������� set ��������
		���������� set _������.���������� + 1
_������
	Choice from _������.����� = _������.���������� + 1 and _������.��������� = ����� and _������.�����_�����_��������� <= �������.time
	Convert_rule
		��������� set ��������
		�����_������_��������� set 0
		�����_�����_���������  set 1000
$End

$Pattern �������_��������_�������: rule trace
$Relevant_resources
	_�������: ������� Keep
$Body
_�������
	Choice from ( ������_1.��������� = ����� and ������_2.��������� = ����� ) 
			or ( ������_1.��������� = ����� and for_all(������: ������.���������� = 0) )
			or ( ������_2.��������� = ����� and for_all(������: ������.���������� > 0) )
			or ( ������_1.��������� = ����� and ������_2.��������� = �������� and for_all(������: ������.���������� <> 1) )
//			or ( ������_1.��������� = �������� and ������_2.��������� = ����� and for_all(������: ������.���������� > 0) )
	Convert_rule
		time set min(������_1.�����_�����_���������, ������_2.�����_�����_���������)
$End