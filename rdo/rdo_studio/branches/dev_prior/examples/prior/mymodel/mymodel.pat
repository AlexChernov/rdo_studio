$Pattern �������_�����������_�����_������ : irregular_event trace
$Relevant_resources
	_�������       : �������  Keep
	_������        : ������   Create
$Time = ��������_�����������( 2 )
$Body
_�������
	Convert_event
		����������_������� set _�������.����������_������� + 1

_������
	Convert_event
		�����                set _�������.����������_�������
		��������������       set ��������������_������
		���������            set ��������
$End

$Pattern �������_�������_������_������� : operation trace
$Relevant_resources
	_�������  : �������  Keep NoChange
	_������   : ������   Keep Keep
	_�����    : �����    Keep Keep
$Time = ������������_�������_������( 0.2, 0.02 )
$Body
_�������
	Choice from _�������.����������_������� > 0
	Convert_begin
		����������_�������  set _�������.����������_������� - 1

_������
	Choice from _������.��������� = ��������
	with_min( _������.����� )
	Convert_begin
		���������  set ������_�����
	Convert_end
		���������  set ������_��������

_�����
	Choice from _�����.��������� = ��������
	Convert_begin
		���������  set ������_�����
	Convert_end
		���������  set ������_��������
$End

$Pattern �������_��������������_������_��_�������_������� : operation trace
$Relevant_resources
	_������   : ������   Keep Keep
	_�����    : �����    Keep Keep
$Time = ������������_��������������_������_��_�������_�������( _������.�������������� )
$Body
_������
	Choice from _������.��������� = ������_��������
	Convert_begin
		���������  set ��������������_������
	Convert_end
		���������  set ��������������_���������

_�����
	Choice from _�����.��������� = ������_��������
	Convert_begin
		���������  set ��������������_������
	Convert_end
		���������  set ��������������_���������
$End

$Pattern �������_��������������_������_������_�������_������� : operation trace
$Relevant_resources
	_������   : ������   Keep Keep
	_�����    : �����    Keep Keep
$Time = ������������_��������������_������_������_�������_�������( _������.�������������� )
$Body
_������
	Choice from _������.��������� = ������_��������
	Convert_begin
		���������  set ��������������_������
	Convert_end
		���������  set ��������������_���������

_�����
	Choice from _�����.��������� = ������_��������
	Convert_begin
		���������  set ��������������_������
	Convert_end
		���������  set ��������������_���������
$End

$Pattern �������_���������������_������_������� : operation trace
$Relevant_resources
	_������   : ������   Keep Keep
	_�����    : �����    Keep Keep
$Time = ������������_���������������_������( 0.4, 0.04 )
$Body
_������
	Choice from _������.��������� = ��������������_���������
	Convert_begin
		���������  set ���������������_������
	Convert_end
		���������  set ���������������_���������

_�����
	Choice from _�����.��������� = ��������������_���������
	Convert_begin
		���������  set ���������������_������
	Convert_end
		���������  set ���������������_���������
$End

$Pattern �������_��������_������ : operation trace
$Relevant_resources
	_�����    : �����    Keep Keep
$Time = ������������_��������_������( 0.4, 0.04 )
$Body
_�����
	Choice from _�����.��������� = ���������������_���������
	Convert_begin
		���������  set �������
	Convert_end
		���������  set ��������
$End
