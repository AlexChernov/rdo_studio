$Constant
	x_mid	: integer = 540
	y_mid	: integer = 400
	r_rob	: integer = 18
	l_rob	: integer = 240
	
	
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
	
	
	x_st11 	: integer = 350
	y_st11 	: integer = 210
	l_st11	: integer = 100
 	h_st11	: integer = 50
	a_st11	: real	  = 0.785
	sinast1 : real	  = 0.71
	cosast1 : real	  = 0.71
	
	x_st12 	: integer = 150
	y_st12 	: integer = 160
	gx_st12	: integer = 100
 	gy_st12	: integer = 50
	
	x_st13 	: integer = 150
	y_st13 	: integer = 220
	gx_st13	: integer = 100
 	gy_st13	: integer = 50
	
	x_st2 	: integer = 730
	y_st2 	: integer = 210
	l_st2	: integer = 200
 	h_st2	: integer = 30
	a_st2	: real = 5.495
	sinast2	: real = -0.71
	cosast2	: real = 0.71
	
	x1		: integer = 50 
	y1 		: integer = 160
	//l  : integer = 100
	h  		: integer = 10
	hag		: integer = 7
	
	h2		: integer = 15
	
	x2		: integer = 400 
	y2 		: integer = 160
	
	x3		: integer = 20 
	y3 		: integer = 450
	
	x4		: integer = 300 
	y4 		: integer = 450
	
	x5		: integer = 500 
	y5 		: integer = 450
	
	//l  : integer = 100
	
$End


$Function �����_���������11 : such_as ST11s.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ST11s.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������11 = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������11 = ��������
$End

$Function �����_���������12 : such_as ST12s.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ST12s.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������12 = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������12 = ��������
$End

$Function �����_���������13 : such_as ST13s.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as ST13s.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������13 = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������13 = ��������
$End

$Function �����_���������2 : such_as STANOK2s.���������
$Type = algorithmic
$Parameters
	�������_��������� : such_as STANOK2s.���������
$Body
	Calculate_if �������_��������� = �������� or �������_��������� = �����
		�����_���������2 = ������
	Calculate_if �������_��������� = ������ 		
		�����_���������2 = ��������
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