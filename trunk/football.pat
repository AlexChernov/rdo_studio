$Pattern �������_������_��������: rule
$Relevant_resources
	_�����: ������ Keep
$Body
_�����
	Choice from _�����.�������� = ��� and _�����.������� <> _�����.����_�������
	Convert_rule
		�������� set �������
$End

$Pattern �������_��������_������: operation
$Relevant_resources
	_�����: ������ Keep Keep
$Time = 1
$Body
_�����
	Choice from _�����.�������� = �������
	Convert_begin
		�������� set ������_�������
	Convert_end
		�������  set ����������_����( _�����.������� + �_�����_�������_�������( _�����.�������, _�����.����_������� ) )
		�������� set ���_������������( _�����.�������, _�����.����_������� )
$End

$Pattern �������_������_��������: rule
$Relevant_resources
	_�����: ������ Keep
$Body
_�����
	Choice from _�����.�������� = ��� and [abs(_�����.x - _�����.����_x) > 2 or abs(_�����.y - _�����.����_y) > 2]
	Convert_rule
		��������    set ��������
		����������� set ����_�����( _�����.x, _�����.y, _�����.����_x, _�����.����_y )
$End

$Pattern �������_�����������_������: operation
$Relevant_resources
	_�����: ������ Keep Keep
$Time = 1
$Body
_�����
	Choice from _�����.�������� = ��������
	Convert_begin
		�������� set ������_��������
	Convert_end
		x           set _�����.x + ���������_����������(_�����.x, _�����.����_x, �����������_��������(_�����.x, _�����.����_x) * cos(_�����.�����������))
		y           set _�����.y - ���������_����������(_�����.y, _�����.����_y, �����������_��������(_�����.y, _�����.����_y) * sin(_�����.�����������))
		��������    set ���_���������( _�����.x, _�����.y, _�����.����_x, _�����.����_y )
$End

$Pattern �������_��������_�������_������: keyboard
$Parameters
	�����������_��������: integer
$Relevant_resources
	_�����: �����_1 Keep NoChange
$Time = 1
$Body
_�����
	Convert_begin
		������� set _�����.������� + �����������_�������� * 5
$End

$Pattern �������_�����������_�������_������: keyboard
$Parameters
	�����������_��������: integer
$Relevant_resources
	_�����: �����_1 Keep NoChange
$Time = 1
$Body
_�����
	Convert_begin
		x set _�����.x + �����������_�������� * 5 * cos((_�����.�������) * pi / 180)
		y set _�����.y - �����������_�������� * 5 * sin((_�����.�������) * pi / 180)
$End

$Pattern �������_�����_������ : irregular_event
$Relevant_resources
	_�����: �����_1 Keep
$Time = 10
$Body
_�����
	Convert_event
		������� set _�����.�������
$End
