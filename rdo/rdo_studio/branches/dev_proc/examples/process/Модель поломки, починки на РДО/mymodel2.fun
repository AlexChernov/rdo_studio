$Constant
	x_mid	: integer = 540
	y_mid	: integer = 400
	r_rob	: integer = 18
	l_rob	: integer = 240
	h_rob	: integer = 55
	
	l1		: integer = 120
	h1		: integer = 15
	n1		: integer = 6
	
	x_qi	: integer = 410
	y_qi	: integer = 407
	ai		: real	  = 3.14
	sinai	: real	  = 0
	cosai	: real	  = -1
	
	
	x_qo1	: integer = 450
	y_qo1	: integer = 310
	x_qi1	: integer = 439
	y_qi1	: integer = 321
	a1		: real = 2.355
	sina1	: real	  = 0.71
	cosa1	: real	  = -0.71
	
	x_qo2	: integer = 630
	y_qo2	: integer = 310
	
	x_qi2	: integer = 619
	y_qi2	: integer = 299
	a2		: real = 0.785
	sina2	: real	  = 0.71
	cosa2	: real	  = 0.71
	
	x_qo	: integer = 670
	y_qo	: integer = 393
	ao		: real	  = 0
	sinao	: real	  = 0
	cosao	: real	  = 1
	
	
	x_st11 	: integer = 335
	y_st11 	: integer = 195
	l_st11	: integer = 100
 	h_st11	: integer = 50
	a_st11	: real	  = 0.785
	sinast1 : real	  = 0.71
	cosast1 : real	  = 0.71

	x_st2 	: integer = 745
	y_st2 	: integer = 195
	l_st2	: integer = 200
 	h_st2	: integer = 30
	a_st2	: real = 5.495
	sinast2	: real = -0.71
	cosast2	: real = 0.71
	
	xs1		: integer = 250
	ys1		: integer = 400
	ls1		: integer = 20
	bs1		: integer = 15
	cs1		: integer = 8
	sas1	: real = 0
	cas1	: real = 1
	

	xs2		: integer = 357
	ys2		: integer = 227
	sas2	: real = 0.71
	cas2	: real = -0.71
	
	
	xs3		: integer = 342
	ys3		: integer = 192
	sas3	: real = -0.71
	cas3	: real = 0.71
	
	xs4		: integer = 710
	ys4		: integer = 215
	sas4	: real = 0.71
	cas4	: real = 0.71
	
	xs5		: integer = 745
	ys5		: integer = 203
	sas5	: real = -0.71
	cas5	: real = -0.71
	
	xs6		: integer = 430
	ys6		: integer = 400
	ls6		: integer = 60
	sas6	: real = 0.97
	cas6	: real = 0.26
	
	xs7		: integer = 475
	ys7		: integer = 305
	ls7		: integer = 115
	sas7	: real = 0
	cas7	: real = 1
	
	xs8		: integer = 627
	ys8		: integer = 325
	ls8		: integer = 60
	sas8	: real = -0.97
	cas8	: real = 0.26
	
	xr1		: integer = 535
	yr1		: integer = 400
	lr1		: integer = 58
	hr1		: integer = 5
	ar1		: real = 2.704
	sinr1	: real = 0.424
	cosr1	: real = -0.906
	k		: integer = 30
	
	xr2		: integer = 535
	yr2		: integer = 399
	lr2		: integer = 63
	hr2		: integer = 5
	ar2		: real = 0.436
	sinr2	: real = 0.424
	cosr2	: real = 0.906
	
	
$End

$Function �����_���������� : such_as ����������.��������� = �����
$Type = list
$Parameters
	�������_��������� : such_as ����������.���������
$Body
	����� 	  = �������1 
	��������1 = �������2 
	��������2 = ������	
$End

$Function �����_��������� : such_as ������.��������� = �����
$Type = list
$Parameters
	�������_��������� : such_as ������.���������
$Body
	�����     = state1 
	��������1 = state2 
	��������2 = state3	
$End



$Function �����_���������11 : such_as ������.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ������.���������
$Body
	if �������_��������� = �������� or �������_��������� = ����� result = ������
	
	result = ��������
$End

$Function �����_���������2 : such_as ������.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ������.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������2 = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������2 = ��������
$End

$Function �����_���������_������ : such_as ������.��������� = ��������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ������.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������_������ = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������_������ = ��������
$End

$Sequence ��������_�������_��������� : real
$Type = exponential 12347
$End

$Sequence ���������_��_������_1 : real
$Type = normal 12346
$End


$Sequence �����_������� : real
$Type = uniform 67891
$End


$Sequence �����_������� : real
$Type = uniform 67892
$End


$Sequence ���������_��_������_2 : real
$Type = exponential 12345
$End