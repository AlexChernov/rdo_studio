$Pattern  �������_�������_������ : keyboard
$Parameters
	������: integer
$Relevant_resources
	������ : ������ Keep NoChange
$Time = 0.0
$Body
������
	Choice from ������.������ = ������
	Convert_begin
		��������� set �����_���������11(������.���������)
$End

$Pattern  �������_�������_������2 : keyboard
$Relevant_resources
	������ : STANOK2	Keep NoChange
$Time = 0.0
$Body
������
	Convert_begin
		��������� set �����_���������2(������.���������)
$End

$Pattern  �������_�������_������ : keyboard 
$Relevant_resources
	����� : ROBOT	Keep NoChange
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