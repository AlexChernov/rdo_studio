$Pattern �������_1 : operation trace
$Parameters
	��������_�������_1 : integer
	��������_�������_2 : array<integer>
$Relevant_resources
	���_������_1 : ���_�������_1 Keep Keep
$Time = ���������_1 	
$Body
	���_������_1
		Choice from ���_������_1.��������_1 == ��������_�������_1 and ���_������_1.��������_3 == ��������
		first
		Convert_begin
			��������_3 set �����;
		Convert_end
			��������_1 set ��������_�������_1 + 1;
			��������_2 set ��������_�������_2;
			��������_3 set ��������;
$End