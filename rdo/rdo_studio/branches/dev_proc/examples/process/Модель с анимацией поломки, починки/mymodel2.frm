$Frame fram_1
$Back_picture = <255 255 255> 1080 680
Show

text    [10, 5,  50, 25, <127 127 127>, <100 255 0>, '�����:' ]
text    [60, 5, 150, 25, <127 127 127>, <100 255 0>, Time_now ]
//rect	[x_st11, y_st11, gx_st11, gy_st11, <255 255 127>, <200 255 100> ]

//rect	[150,70,730,450, <255 255 255>]

//line	[0, y_mid, 1080, 400, <0  0 0>]
ellipse [x_mid - l_rob/2, y_mid - l_rob/2, l_rob, l_rob, <255 255 255>, <0 0 0>]
ellipse [x_mid - r_rob/2, y_mid - r_rob/2, r_rob, r_rob, <255 255 255>, <0 0 0>] 
/*
//----------------------���������� �������������------------------------------------
triang	[x_st11, y_st11, x_st11 + l1*cosa1, 
		 y_st11 - l1*sina1, x_st11 + l1*cosa1 + h1*sina1,
		 y_st11 - l1*sina1+h1*cosa1,
		 <127 127 127>, <127 127 127>]
triang	[x_st11 + l1*cosa1 + h1*sina1, y_st11 - l1*sina1+h1*cosa1, 
		 x_st11+h1*sina1, y_st11 + h1*cosa1,
		 x_st11, y_st11,
		 <127 127 127>, <127 127 127>]
		 
line	[x_st11, y_st11, x_st11 + l1*cosa1, y_st11 - l1*sina1, <0  0 0>]
line	[x_st11 + l1*cosa1, y_st11 - l1*sina1, x_st11 + l1*cosa1 + h1*sina1, y_st11 - l1*sina1+h1*cosa1, <0  0 0>]
line	[x_st11 + l1*cosa1 + h1*sina1, y_st11 - l1*sina1+h1*cosa1, x_st11+h1*sina1, y_st11 + h1*cosa1, <0  0 0>]
line	[x_st11+h1*sina1, y_st11 + h1*cosa1, x_st11, y_st11, <0  0 0>]
*/
/*
//----------------------���������� �������������------------------------------------
triang	[x_q1 - h1*sina1, 			  			 	y_q1 - h*cosa1, 
		 x_q1 - h1*sina1 + l1*cosa1,   				y_q1 - h*cosa1 - l1*sina1, 
		 x_q1 - h1*sina1 + l1*cosa1 + h1*sina1, 	y_q1 - h*cosa1 - l1*sina1 + h1*cosa1,
		 <127 127 127>, <127 127 127>]
triang	[x_q1 - h1*sina1 + l1*cosa1 + h1*sina1,	y_q1 - h*cosa1 - l1*sina1 + h1*cosa1, 
		 x_q1 - h1*sina1 + h1*sina1, 				y_q1 - h*cosa1 + h1*cosa1,
		 x_q1 - h1*sina1, 							y_q1 - h*cosa1,
		 <127 127 127>, <127 127 127>]
		 
line	[x_q1 - h1*sina1, 							y_q1 - h*cosa1,
		 x_q1 - h1*sina1 + l1*cosa1, 				y_q1 - h*cosa1 - l1*sina1,
		 <0  0 0>]
line	[x_q1 - h1*sina1 + l1*cosa1, 				y_q1 - h*cosa1 - l1*sina1, 
		 x_q1 - h1*sina1 + l1*cosa1 + h1*sina1, 	y_q1 - h*cosa1 - l1*sina1 + h1*cosa1, 
		 <0  0 0>]
		 
line	[x_q1 - h1*sina1 + l1*cosa1 + h1*sina1, 	y_q1 - h*cosa1 - l1*sina1 + h1*cosa1, 
		 x_q1 - h1*sina1 + h1*sina1, 			  	y_q1 - h*cosa1 + h1*cosa1, 
		 <0  0 0>]
line	[x_q1 - h1*sina1 + h1*sina1, 			  	y_q1 - h*cosa1 + h1*cosa1, 
		 x_q1 - h1*sina1, 						  	y_q1 - h*cosa1, 
		 <0  0 0>]
*/
ellipse [x_qi1, y_qi1, 3, 3, <255 255 255>, <0 0 0>]

//----------------------������� ���������� - ����------------------------------------
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cosai, 			y_qi - l1*sinai,
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <127 127 127>, <0 0 127>]

triang	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + h1*sinai, 			y_qi + h1*cosai,
		 x_qi, 						 	y_qi,
		  <127 127 127>, <127 127 127>]
		  

//----------------------������� ���������� - �� ����� �������------------------------------------
show_if QIN.�����_������� <= 8
triang	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.�����_�������*cosai, 			   y_qi - h1*QIN.�����_�������*sinai,
		 x_qi + h1*QIN.�����_�������*cosai + h1*sinai,y_qi - h1*QIN.�����_�������*sinai + h1*cosai,
		 <255 255 0>, <0 0 0>]

triang	[x_qi + h1*QIN.�����_�������*cosai + h1*sinai,y_qi - h1*QIN.�����_�������*sinai + h1*cosai, 
		 x_qi + h1*sinai, 								   y_qi + h1*cosai,
		 x_qi, 						 						   y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + h1*QIN.�����_�������*cosai + h1*sinai,y_qi - h1*QIN.�����_�������*sinai + h1*cosai,
		 <255 255 0>]
		 
show_if QIN.�����_������� > 8
triang	[x_qi, 							y_qi, 
		 x_qi + l1*cosai, 			y_qi - l1*sinai,
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <255 255 0>, <0 0 0>]

triang	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + l1*sinai, 			y_qi + h1*cosai,
		 x_qi, 						 	y_qi,
		  <255 255 0>, <0 0 0>]
line	[x_qi, 												   y_qi, 
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai,
		 <255 255 0>]

show
//----------------------������� ���������� - ���������------------------------------------

line	[x_qi, 						 	y_qi, 
		 x_qi + l1*cosai,			 	y_qi - l1*sinai, 
		 <0  0 0>]
line	[x_qi + l1*cosai,			 	y_qi - l1*sinai, 
		 x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 <0  0 0>]
line	[x_qi + l1*cosai + h1*sinai,y_qi - l1*sinai + h1*cosai, 
		 x_qi + h1*sinai, 			y_qi + h1*cosai, 
		 <0  0 0>]
line	[x_qi+ h1*sinai, 				y_qi + h1*cosai, 
		 x_qi, 					     	y_qi, 
		 <0  0 0>]


//----------------------����� ������� ���������� - ����------------------------------------------------		 
		 
line	[x_qi + 1*h1*cosai, y_qi - 1*h1*sinai, x_qi + h1*sinai + 1*h1*cosai, y_qi + h1*cosai - 1*h1*sinai, <0 0 0>]
line	[x_qi + 2*h1*cosai, y_qi - 2*h1*sinai, x_qi + h1*sinai + 2*h1*cosai, y_qi + h1*cosai - 2*h1*sinai, <0 0 0>]
line	[x_qi + 3*h1*cosai, y_qi - 3*h1*sinai, x_qi + h1*sinai + 3*h1*cosai, y_qi + h1*cosai - 3*h1*sinai, <0 0 0>]
line	[x_qi + 4*h1*cosai, y_qi - 4*h1*sinai, x_qi + h1*sinai + 4*h1*cosai, y_qi + h1*cosai - 4*h1*sinai, <0 0 0>]
line	[x_qi + 5*h1*cosai, y_qi - 5*h1*sinai, x_qi + h1*sinai + 5*h1*cosai, y_qi + h1*cosai - 5*h1*sinai, <0 0 0>]
line	[x_qi + 6*h1*cosai, y_qi - 6*h1*sinai, x_qi + h1*sinai + 6*h1*cosai, y_qi + h1*cosai - 6*h1*sinai, <0 0 0>]
line	[x_qi + 7*h1*cosai, y_qi - 7*h1*sinai, x_qi + h1*sinai + 7*h1*cosai, y_qi + h1*cosai - 7*h1*sinai, <0 0 0>]

//----------------------����� � ������� ���������� - ����------------------------------------------------		 
text    [ x_qi - 10, y_qi, 90, 25, <127 127 127>, <100 255 0>, QIN.�����_������� ]

//----------------------������� ���������� - �����------------------------------------
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cosao, 			y_qo - l1*sinao,
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <127 127 127>, <127 127 127>]

triang	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + h1*sinao, 			y_qo + h1*cosao,
		 x_qo, 						 	y_qo,
		  <127 127 127>, <127 127 127>]

//----------------------������� ���������� - �� ����� �������------------------------------------
show_if QOUT.�����_������� <= 8
triang	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.�����_�������*cosao, 			   y_qo - h1*QOUT.�����_�������*sinao,
		 x_qo + h1*QOUT.�����_�������*cosao + h1*sinao,y_qo - h1*QOUT.�����_�������*sinao + h1*cosao,
		 <255 255 0>, <0 0 0>]

triang	[x_qo + h1*QOUT.�����_�������*cosao + h1*sinao,y_qo - h1*QOUT.�����_�������*sinao + h1*cosao, 
		 x_qo + h1*sinao, 								   y_qo + h1*cosao,
		 x_qo, 						 						   y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + h1*QOUT.�����_�������*cosao + h1*sinao,y_qo - h1*QOUT.�����_�������*sinao + h1*cosao,
		 <255 255 0>]
		 
show_if QOUT.�����_������� > 8
triang	[x_qo, 							y_qo, 
		 x_qo + l1*cosao, 			y_qo - l1*sinao,
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <255 255 0>, <0 0 0>]

triang	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + l1*sinao, 			y_qo + h1*cosao,
		 x_qo, 						 	y_qo,
		  <255 255 0>, <0 0 0>]
line	[x_qo, 												   y_qo, 
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao,
		 <255 255 0>]

show
//----------------------������� ���������� - ���������------------------------------------
line	[x_qo, 						 	y_qo, 
		 x_qo + l1*cosao,			 	y_qo - l1*sinao, 
		 <0  0 0>]
line	[x_qo + l1*cosao,			 	y_qo - l1*sinao, 
		 x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 <0  0 0>]
line	[x_qo + l1*cosao + h1*sinao,y_qo - l1*sinao + h1*cosao, 
		 x_qo + h1*sinao, 			y_qo + h1*cosao, 
		 <0  0 0>]
line	[x_qo+ h1*sinao, 				y_qo + h1*cosao, 
		 x_qo, 					     	y_qo, 
		 <0  0 0>]

//----------------------����� ������� ���������� - �����------------------------------------------------		 
		 
line	[x_qo + 1*h1*cosao, y_qo - 1*h1*sinao, x_qo + h1*sinao + 1*h1*cosao, y_qo + h1*cosao - 1*h1*sinao, <0 0 0>]
line	[x_qo + 2*h1*cosao, y_qo - 2*h1*sinao, x_qo + h1*sinao + 2*h1*cosao, y_qo + h1*cosao - 2*h1*sinao, <0 0 0>]
line	[x_qo + 3*h1*cosao, y_qo - 3*h1*sinao, x_qo + h1*sinao + 3*h1*cosao, y_qo + h1*cosao - 3*h1*sinao, <0 0 0>]
line	[x_qo + 4*h1*cosao, y_qo - 4*h1*sinao, x_qo + h1*sinao + 4*h1*cosao, y_qo + h1*cosao - 4*h1*sinao, <0 0 0>]
line	[x_qo + 5*h1*cosao, y_qo - 5*h1*sinao, x_qo + h1*sinao + 5*h1*cosao, y_qo + h1*cosao - 5*h1*sinao, <0 0 0>]
line	[x_qo + 6*h1*cosao, y_qo - 6*h1*sinao, x_qo + h1*sinao + 6*h1*cosao, y_qo + h1*cosao - 6*h1*sinao, <0 0 0>]
line	[x_qo + 7*h1*cosao, y_qo - 7*h1*sinao, x_qo + h1*sinao + 7*h1*cosao, y_qo + h1*cosao - 7*h1*sinao, <0 0 0>]
//----------------------����� � ������� ���������� - �����------------------------------------------------		 
text    [ x_qo , y_qi, 90, 25, <127 127 127>, <100 255 0>, QOUT.�����_������� ]




//----------------------������� ������ 1 - ����� ------------------------------------
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cosa1, 			y_qo1 - l1*sina1,
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <127 127 127>, <127 127 127>]

triang	[x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 			y_qo1 + h1*cosa1,
		 x_qo1, 							y_qo1,
		  <127 127 127>, <127 127 127>]

//----------------------������� ������ 1 - �����------------------------------------
show_if QEXITST1.�����_������� <= 8
triang	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.�����_�������*cosa1, 			   y_qo1 - h1*QEXITST1.�����_�������*sina1,
		 x_qo1 + h1*QEXITST1.�����_�������*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.�����_�������*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + h1*QEXITST1.�����_�������*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.�����_�������*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 								   y_qo1 + h1*cosa1,
		 x_qo1, 						 						   y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + h1*QEXITST1.�����_�������*cosa1 + h1*sina1,y_qo1 - h1*QEXITST1.�����_�������*sina1 + h1*cosa1,
		 <255 255 0>]
		 
show_if QEXITST1.�����_������� > 8
triang	[x_qo1, 							y_qo1, 
		 x_qo1 + l1*cosa1, 			y_qo1 - l1*sina1,
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 			y_qo1 + h1*cosa1,
		 x_qo1, 						 	y_qo1,
		  <255 255 0>, <0 0 0>]
line	[x_qo1, 												   y_qo1, 
		 x_qo1 + l1*cosa1 + h1*sina1,y_qo1 - l1*sina1 + h1*cosa1,
		 <255 255 0>]

show
//----------------------������� ������1 ����� - ���������------------------------------------


line	[x_qo1, 								y_qo1, 
		 x_qo1 + l1*cosa1,					y_qo1 - l1*sina1, 
		 <0  0 0>]
line	[x_qo1 + l1*cosa1,					y_qo1 - l1*sina1, 
		 x_qo1 + l1*cosa1 + h1*sina1, 	y_qo1 - l1*sina1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qo1 + l1*cosa1 + h1*sina1, 	y_qo1 - l1*sina1 + h1*cosa1, 
		 x_qo1 + h1*sina1, 				y_qo1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qo1 + h1*sina1, 				y_qo1 + h1*cosa1, 
		 x_qo1, 								y_qo1, 
		 <0  0 0>]
//----------------------����� ������� - ����� ������ 1------------------------------------------------		 
		 
line	[x_qo1 + 1*h1*cosa1, y_qo1 - 1*h1*sina1, x_qo1 + h1*sina1 + 1*h1*cosa1, y_qo1 + h1*cosa1 - 1*h1*sina1, <0 0 0>]
line	[x_qo1 + 2*h1*cosa1, y_qo1 - 2*h1*sina1, x_qo1 + h1*sina1 + 2*h1*cosa1, y_qo1 + h1*cosa1 - 2*h1*sina1, <0 0 0>]
line	[x_qo1 + 3*h1*cosa1, y_qo1 - 3*h1*sina1, x_qo1 + h1*sina1 + 3*h1*cosa1, y_qo1 + h1*cosa1 - 3*h1*sina1, <0 0 0>]
line	[x_qo1 + 4*h1*cosa1, y_qo1 - 4*h1*sina1, x_qo1 + h1*sina1 + 4*h1*cosa1, y_qo1 + h1*cosa1 - 4*h1*sina1, <0 0 0>]
line	[x_qo1 + 5*h1*cosa1, y_qo1 - 5*h1*sina1, x_qo1 + h1*sina1 + 5*h1*cosa1, y_qo1 + h1*cosa1 - 5*h1*sina1, <0 0 0>]
line	[x_qo1 + 6*h1*cosa1, y_qo1 - 6*h1*sina1, x_qo1 + h1*sina1 + 6*h1*cosa1, y_qo1 + h1*cosa1 - 6*h1*sina1, <0 0 0>]
line	[x_qo1 + 7*h1*cosa1, y_qo1 - 7*h1*sina1, x_qo1 + h1*sina1 + 7*h1*cosa1, y_qo1 + h1*cosa1 - 7*h1*sina1, <0 0 0>]
//----------------------����� � ������� - ����� ������ 1------------------------------------------------		 
text    [ x_qo1 + 10, y_qo1 - 30, 90, 25, <127 127 127>, <100 255 0>, QEXITST1.�����_������� ]


//----------------------������� ������ 1 - ����------------------------------------
triang	[x_qi1, 						y_qi1, 
		 x_qi1 + l1*cosa1, 			y_qi1 - l1*sina1,
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <127 127 127>, <127 127 127>]

triang	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			y_qi1 + h1*cosa1,
		 x_qi1, 							y_qi1,
		  <127 127 127>, <127 127 127>]

//----------------------������� ������ 1 - ����------------------------------------
show_if QSTANOK1.�����_������� <= 8
triang	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.�����_�������*cosa1, 			y_qi1 - h1*QSTANOK1.�����_�������*sina1,
		 x_qi1 + h1*QSTANOK1.�����_�������*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.�����_�������*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + h1*QSTANOK1.�����_�������*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.�����_�������*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 								    y_qi1 + h1*cosa1,
		 x_qi1, 						 						y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 												y_qi1, 
		 x_qi1 + h1*QSTANOK1.�����_�������*cosa1 + h1*sina1,y_qi1 - h1*QSTANOK1.�����_�������*sina1 + h1*cosa1,
		 <255 255 0>]
		 
show_if QSTANOK1.�����_������� > 8
triang	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cosa1, 			 y_qi1 - l1*sina1,
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <255 255 0>, <0 0 0>]

triang	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			 y_qi1 + h1*cosa1,
		 x_qi1, 						 y_qi1,
		  <255 255 0>, <0 0 0>]
line	[x_qi1, 						 y_qi1, 
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1,
		 <255 255 0>]

show
//----------------------������� ������1 ���� - ���������------------------------------------

line	[x_qi1, 							y_qi1, 
		 x_qi1 + l1*cosa1,				y_qi1 - l1*sina1, 
		 <0  0 0>]
line	[x_qi1 + l1*cosa1,				y_qi1 - l1*sina1, 
		 x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qi1 + l1*cosa1 + h1*sina1,y_qi1 - l1*sina1 + h1*cosa1, 
		 x_qi1 + h1*sina1, 			y_qi1 + h1*cosa1, 
		 <0  0 0>]
line	[x_qi1+ h1*sina1, 				y_qi1 + h1*cosa1, 
		 x_qi1, 							y_qi1, 
		 <0  0 0>]

//----------------------����� ������� - ���� ������ 1------------------------------------------------		 
		 
line	[x_qi1 + 1*h1*cosa1, y_qi1 - 1*h1*sina1, x_qi1 + h1*sina1 + 1*h1*cosa1, y_qi1 + h1*cosa1 - 1*h1*sina1, <0 0 0>]
line	[x_qi1 + 2*h1*cosa1, y_qi1 - 2*h1*sina1, x_qi1 + h1*sina1 + 2*h1*cosa1, y_qi1 + h1*cosa1 - 2*h1*sina1, <0 0 0>]
line	[x_qi1 + 3*h1*cosa1, y_qi1 - 3*h1*sina1, x_qi1 + h1*sina1 + 3*h1*cosa1, y_qi1 + h1*cosa1 - 3*h1*sina1, <0 0 0>]
line	[x_qi1 + 4*h1*cosa1, y_qi1 - 4*h1*sina1, x_qi1 + h1*sina1 + 4*h1*cosa1, y_qi1 + h1*cosa1 - 4*h1*sina1, <0 0 0>]
line	[x_qi1 + 5*h1*cosa1, y_qi1 - 5*h1*sina1, x_qi1 + h1*sina1 + 5*h1*cosa1, y_qi1 + h1*cosa1 - 5*h1*sina1, <0 0 0>]
line	[x_qi1 + 6*h1*cosa1, y_qi1 - 6*h1*sina1, x_qi1 + h1*sina1 + 6*h1*cosa1, y_qi1 + h1*cosa1 - 6*h1*sina1, <0 0 0>]
line	[x_qi1 + 7*h1*cosa1, y_qi1 - 7*h1*sina1, x_qi1 + h1*sina1 + 7*h1*cosa1, y_qi1 + h1*cosa1 - 7*h1*sina1, <0 0 0>]
//----------------------����� � ������� - ���� ������ 1------------------------------------------------		 
text    [ x_qi1 - 19, y_qi1 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK1.�����_������� ]

//----------------------������� ������ 2 - ����� ------------------------------------
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2, 			 y_qo2 - l1*sina2,
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <127 127 127>, <127 127 127>]

triang	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2,
		 x_qo2, 						 y_qo2,
		  <127 127 127>, <127 127 127>]

//----------------------������� ������ 2 - �����------------------------------------
show_if QEXITST2.�����_������� <= 8
triang	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.�����_�������*cosa2, 			y_qo2 - h1*QEXITST2.�����_�������*sina2,
		 x_qo2 + h1*QEXITST2.�����_�������*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.�����_�������*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + h1*QEXITST2.�����_�������*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.�����_�������*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 								    y_qo2 + h1*cosa2,
		 x_qo2, 						 						y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 												y_qo2, 
		 x_qo2 + h1*QEXITST2.�����_�������*cosa2 + h1*sina2,y_qo2 - h1*QEXITST2.�����_�������*sina2 + h1*cosa2,
		 <255 255 0>]
		 
show_if QEXITST2.�����_������� > 8
triang	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2, 			 y_qo2 - l1*sina2,
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2,
		 x_qo2, 						 y_qo2,
		  <255 255 0>, <0 0 0>]
line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2,
		 <255 255 0>]

show
//----------------------������� ������ 2 ����� - ���������------------------------------------

line	[x_qo2, 						 y_qo2, 
		 x_qo2 + l1*cosa2,			 y_qo2 - l1*sina2, 
		 <0  0 0>]
line	[x_qo2 + l1*cosa2,			 y_qo2 - l1*sina2, 
		 x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qo2 + l1*cosa2 + h1*sina2,y_qo2 - l1*sina2 + h1*cosa2, 
		 x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qo2 + h1*sina2, 			 y_qo2 + h1*cosa2, 
		 x_qo2, 						 y_qo2, 
		 <0  0 0>]
//----------------------����� ������� - ����� ������ 2------------------------------------------------		 
		 
line	[x_qo2 + 1*h1*cosa2, y_qo2 - 1*h1*sina2, x_qo2 + h1*sina2 + 1*h1*cosa2, y_qo2 + h1*cosa2 - 1*h1*sina2, <0 0 0>]
line	[x_qo2 + 2*h1*cosa2, y_qo2 - 2*h1*sina2, x_qo2 + h1*sina2 + 2*h1*cosa2, y_qo2 + h1*cosa2 - 2*h1*sina2, <0 0 0>]
line	[x_qo2 + 3*h1*cosa2, y_qo2 - 3*h1*sina2, x_qo2 + h1*sina2 + 3*h1*cosa2, y_qo2 + h1*cosa2 - 3*h1*sina2, <0 0 0>]
line	[x_qo2 + 4*h1*cosa2, y_qo2 - 4*h1*sina2, x_qo2 + h1*sina2 + 4*h1*cosa2, y_qo2 + h1*cosa2 - 4*h1*sina2, <0 0 0>]
line	[x_qo2 + 5*h1*cosa2, y_qo2 - 5*h1*sina2, x_qo2 + h1*sina2 + 5*h1*cosa2, y_qo2 + h1*cosa2 - 5*h1*sina2, <0 0 0>]
line	[x_qo2 + 6*h1*cosa2, y_qo2 - 6*h1*sina2, x_qo2 + h1*sina2 + 6*h1*cosa2, y_qo2 + h1*cosa2 - 6*h1*sina2, <0 0 0>]
line	[x_qo2 + 7*h1*cosa2, y_qo2 - 7*h1*sina2, x_qo2 + h1*sina2 + 7*h1*cosa2, y_qo2 + h1*cosa2 - 7*h1*sina2, <0 0 0>]
//----------------------����� � ������� - ���� ������ 1------------------------------------------------		 
text    [ x_qo2 + 20, y_qo2, 90, 25, <127 127 127>, <100 255 0>, QEXITST2.�����_������� ]

//----------------------������� ������ 2 - ����------------------------------------
triang	[x_qi2, 						y_qi2, 
		 x_qi2 + l1*cosa2, 			y_qi2 - l1*sina2,
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <127 127 127>, <127 127 127>]

triang	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			y_qi2 + h1*cosa2,
		 x_qi2, 							y_qi2,
		  <127 127 127>, <127 127 127>]

//----------------------������� ������ 2 - ����------------------------------------
show_if QSTANOK2.�����_������� <= 8
triang	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.�����_�������*cosa2, 			y_qi2 - h1*QSTANOK2.�����_�������*sina2,
		 x_qi2 + h1*QSTANOK2.�����_�������*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.�����_�������*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + h1*QSTANOK2.�����_�������*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.�����_�������*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 								    y_qi2 + h1*cosa2,
		 x_qi2, 						 						y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 												y_qi2, 
		 x_qi2 + h1*QSTANOK2.�����_�������*cosa2 + h1*sina2,y_qi2 - h1*QSTANOK2.�����_�������*sina2 + h1*cosa2,
		 <255 255 0>]
		 
show_if QSTANOK2.�����_������� > 8
triang	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cosa2, 			 y_qi2 - l1*sina2,
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <255 255 0>, <0 0 0>]

triang	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			 y_qi2 + h1*cosa2,
		 x_qi2, 						 y_qi2,
		  <255 255 0>, <0 0 0>]
line	[x_qi2, 						 y_qi2, 
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2,
		 <255 255 0>]

show
//----------------------������� ������ 2 ���� - ���������------------------------------------

line	[x_qi2, 							y_qi2, 
		 x_qi2 + l1*cosa2,				y_qi2 - l1*sina2, 
		 <0  0 0>]
line	[x_qi2 + l1*cosa2,				y_qi2 - l1*sina2, 
		 x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qi2 + l1*cosa2 + h1*sina2,y_qi2 - l1*sina2 + h1*cosa2, 
		 x_qi2 + h1*sina2, 			y_qi2 + h1*cosa2, 
		 <0  0 0>]
line	[x_qi2+ h1*sina2, 				y_qi2 + h1*cosa2, 
		 x_qi2, 							y_qi2, 
		 <0  0 0>]

//----------------------����� ������� - ���� ������ 2------------------------------------------------		 
		 
line	[x_qi2 + 1*h1*cosa2, y_qi2 - 1*h1*sina2, x_qi2 + h1*sina2 + 1*h1*cosa2, y_qi2 + h1*cosa2 - 1*h1*sina2, <0 0 0>]
line	[x_qi2 + 2*h1*cosa2, y_qi2 - 2*h1*sina2, x_qi2 + h1*sina2 + 2*h1*cosa2, y_qi2 + h1*cosa2 - 2*h1*sina2, <0 0 0>]
line	[x_qi2 + 3*h1*cosa2, y_qi2 - 3*h1*sina2, x_qi2 + h1*sina2 + 3*h1*cosa2, y_qi2 + h1*cosa2 - 3*h1*sina2, <0 0 0>]
line	[x_qi2 + 4*h1*cosa2, y_qi2 - 4*h1*sina2, x_qi2 + h1*sina2 + 4*h1*cosa2, y_qi2 + h1*cosa2 - 4*h1*sina2, <0 0 0>]
line	[x_qi2 + 5*h1*cosa2, y_qi2 - 5*h1*sina2, x_qi2 + h1*sina2 + 5*h1*cosa2, y_qi2 + h1*cosa2 - 5*h1*sina2, <0 0 0>]
line	[x_qi2 + 6*h1*cosa2, y_qi2 - 6*h1*sina2, x_qi2 + h1*sina2 + 6*h1*cosa2, y_qi2 + h1*cosa2 - 6*h1*sina2, <0 0 0>]
line	[x_qi2 + 7*h1*cosa2, y_qi2 - 7*h1*sina2, x_qi2 + h1*sina2 + 7*h1*cosa2, y_qi2 + h1*cosa2 - 7*h1*sina2, <0 0 0>]
//----------------------����� � ������� - ���� ������ 2------------------------------------------------		 
text    [ x_qi2 - 10, y_qi2 - 25, 90, 25, <127 127 127>, <100 255 0>, QSTANOK2.�����_������� ]


//----------------------������ 11 - �������------------------------------------
show		 
active  ��������_�������11 [x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]
/*						  
rect					  [x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1,
						  <0 0 0>, <0 0 0>]	*/
//----------------------������ 11 - ��������------------------------------------
Show_if ST11.��������� = ��������
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------������ 11 - �����------------------------------------
Show_if ST11.��������� = �����		 
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
		 
//----------------------������ 11 - ������------------------------------------
Show_if ST11.��������� = ������		 
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 1.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 1.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 1.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]

//----------------------������ 12 - �������------------------------------------
show		 
active  ��������_�������12  [x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]
/*						  
rect					  [x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1,
						  <0 0 0>, <0 0 0>]	*/
//------------------------������ 12 - ��������------------------------------
Show_if ST12.��������� = ��������

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//------------------------������ 12 - �����------------------------------
Show_if ST12.��������� = �����

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]

//------------------------������ 12 - ������------------------------------
Show_if ST12.��������� = ������

triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 + 0.5*l_st11*sinast1- h_st11*cosast1,  
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 - 0.5*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 + 0.5*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------������ 13 - �������------------------------------------
show		 
active  ��������_�������13[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1]
/*						  
rect					  [x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
						  y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
						  l_st11*cosast1 - h_st11*sinast1,
						  h_st11*cosast1 + l_st11*sinast1,
						  <0 0 0>, <0 0 0>]	*/

//----------------------������ 13 - ��������------------------------------------
Show_if ST13.��������� = ��������
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <210 210 210>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <210 210 210>]

line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------������ 13 - �����------------------------------------
Show_if ST13.��������� = �����
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <0 255 0>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <0 255 0>]

line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
//----------------------������ 13 - ������------------------------------------
Show_if ST13.��������� = ������
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
triang	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 				
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 							
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 <255 0 0>, <127 127 127>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 			  		
		 y_st11 - 0.51*l_st11*sinast1- h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1,
		 <255 0 0>]

line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1,
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1,
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 <0  0 0>]
		 
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + l_st11*cosast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 - l_st11*sinast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1,
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 <0  0 0>]
line	[x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1 + h_st11*sinast1, 
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1 + h_st11*cosast1, 
		 x_st11 + 0.51*l_st11*cosast1 - h_st11*sinast1, 	
		 y_st11 - 0.51*l_st11*sinast1 - h_st11*cosast1, 
		 <0  0 0>]
		 
//----------------------������ 2 - �������------------------------------------
show		 
active  ��������_�������2[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
						  y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2,
						  -h_st2*cosast2 - l_st2*sinast2,
						  l_st2*cosast2 - h_st2*sinast2]
/*						  
rect					  [x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
						  y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2,
						  -h_st2*cosast2 - l_st2*sinast2,
						  l_st2*cosast2 - h_st2*sinast2,
						  <0 0 0>, <0 0 0>]	*/

//----------------------������ 2 - ��������------------------------------------
Show_if STANOK2.��������� = ��������
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <210 210 210>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <210 210 210>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <210 210 210>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]
//----------------------������ 2 - �����------------------------------------
Show_if STANOK2.��������� = �����
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <0 255 0>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <0 255 0>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <0 255 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]

//----------------------������ 2 - ������------------------------------------
Show_if STANOK2.��������� = ������
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <255 0 0>, <127 127 127>]
triang	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 				
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 							
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 <255 0 0>, <127 127 127>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 			  		
		 y_st2 + 0.51*l_st2*sinast2- h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2,
		 <255 0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2,
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2,
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 <0  0 0>]
		 
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + l_st2*cosast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 - l_st2*sinast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2,
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 <0  0 0>]
line	[x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2 + h_st2*sinast2, 
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2 + h_st2*cosast2, 
		 x_st2 - 0.51*l_st2*cosast2 - h_st2*sinast2, 	
		 y_st2 + 0.51*l_st2*sinast2 - h_st2*cosast2, 
		 <0  0 0>]
		 

/*
//-------------------������� � ������� ������---------------------------------------

rect	[x1, y1, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QSTANOK1.�����_������� <= 10
rect	[x1 + hag*10 - QSTANOK1.�����_�������*hag - 1, y1, QSTANOK1.�����_�������*hag, h, <127 0 127>, <100 255 0>]
show_if QSTANOK1.�����_������� > 10
rect	[x1, y1, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x1 + hag*10 - hag, y1 + 1, x1 + hag*10 - hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 2*hag, y1 + 1, x1 + hag*10 - 2*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 3*hag, y1 + 1, x1 + hag*10 - 3*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 4*hag, y1 + 1, x1 + hag*10 - 4*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 5*hag, y1 + 1, x1 + hag*10 - 5*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 6*hag, y1 + 1, x1 + hag*10 - 6*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 7*hag, y1 + 1, x1 + hag*10 - 7*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 8*hag, y1 + 1, x1 + hag*10 - 8*hag, y1 + h - 1, <0  0 127>]
line	[x1 + hag*10 - 9*hag, y1 + 1, x1 + hag*10 - 9*hag, y1 + h - 1, <0  0 127>]

text    [ x1 - 20, y1 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK1.�����_������� ]

*/








/*
//-------------------������� �� ������� ������---------------------------------------

rect	[x2, y2, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QSTANOK2.�����_������� <= 10
rect	[x2 + hag*10 - QSTANOK2.�����_�������*hag - 1, y2, QSTANOK2.�����_�������*hag, h, <127 0 127>, <100 255 0>]
show_if QSTANOK2.�����_������� > 10
rect	[x2, y2, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x2 + hag*10 - hag, y2 + 1, x2 + hag*10 - hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 2*hag, y2 + 1, x2 + hag*10 - 2*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 3*hag, y2 + 1, x2 + hag*10 - 3*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 4*hag, y2 + 1, x2 + hag*10 - 4*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 5*hag, y2 + 1, x2 + hag*10 - 5*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 6*hag, y2 + 1, x2 + hag*10 - 6*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 7*hag, y2 + 1, x2 + hag*10 - 7*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 8*hag, y2 + 1, x2 + hag*10 - 8*hag, y2 + h - 1, <0  0 127>]
line	[x2 + hag*10 - 9*hag, y2 + 1, x2 + hag*10 - 9*hag, y2 + h - 1, <0  0 127>]

text    [ x2- 20, y2 - 5, 90, 25, <127 127 127>, <100 255 0>, QSTANOK2.�����_������� ]

//-------------------������� �� ������ �� ������� ����������---------------------------------------

rect	[x3, y3, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QROBOT1.�����_������� <= 10
rect	[x3 + hag*10 - QROBOT1.�����_�������*hag - 1, y3, QROBOT1.�����_�������*hag, h, <127 0 127>, <100 255 0>]
show_if QROBOT1.�����_������� > 10
rect	[x3, y3, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x3 + hag*10 - hag, y3 + 1, x3 + hag*10 - hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 2*hag, y3 + 1, x3 + hag*10 - 2*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 3*hag, y3 + 1, x3 + hag*10 - 3*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 4*hag, y3 + 1, x3 + hag*10 - 4*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 5*hag, y3 + 1, x3 + hag*10 - 5*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 6*hag, y3 + 1, x3 + hag*10 - 6*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 7*hag, y3 + 1, x3 + hag*10 - 7*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 8*hag, y3 + 1, x3 + hag*10 - 8*hag, y3 + h - 1, <0  0 127>]
line	[x3 + hag*10 - 9*hag, y3 + 1, x3 + hag*10 - 9*hag, y3 + h - 1, <0  0 127>]

text    [ x3- 20, y3 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT1.�����_������� ]


//-------------------������� � ������ �� ����������� �� �������---------------------------------------

rect	[x4, y4, hag*10, h2, <127 127 127>, <100 255 0> ]
show_if QROBOT2.�����_������� <= 10
rect	[x4 + hag*10 - QROBOT2.�����_�������*hag - 1, y4, QROBOT2.�����_�������*hag, h2, <127 0 127>, <100 255 0>]
show_if QROBOT2.�����_������� > 10
rect	[x4, y4, hag*10, h2, <127 0 127>, <100 255 0>]

show
line	[x4 + hag*10 - hag, y4 + 1, x4 + hag*10 - hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 2*hag, y4 + 1, x4 + hag*10 - 2*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 3*hag, y4 + 1, x4 + hag*10 - 3*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 4*hag, y4 + 1, x4 + hag*10 - 4*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 5*hag, y4 + 1, x4 + hag*10 - 5*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 6*hag, y4 + 1, x4 + hag*10 - 6*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 7*hag, y4 + 1, x4 + hag*10 - 7*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 8*hag, y4 + 1, x4 + hag*10 - 8*hag, y4 + h2 - 1, <0  0 127>]
line	[x4 + hag*10 - 9*hag, y4 + 1, x4 + hag*10 - 9*hag, y4 + h2 - 1, <0  0 127>]

text    [ x4- 20, y4 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT2.�����_������� ]


//-------------------������� � ������ �� ��������� �������, � ����������� � ����������---------------------------------------

rect	[x5, y5, hag*10, h, <127 127 127>, <100 255 0> ]
show_if QROBOT3.�����_������� <= 10
rect	[x5 + hag*10 - QROBOT3.�����_�������*hag - 1, y5, QROBOT3.�����_�������*hag, h, <127 0 127>, <100 255 0>]
show_if QROBOT3.�����_������� > 10
rect	[x5, y5, hag*10, h, <127 0 127>, <100 255 0>]

show
line	[x5 + hag*10 - hag, y5 + 1, x5 + hag*10 - hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 2*hag, y5 + 1, x5 + hag*10 - 2*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 3*hag, y5 + 1, x5 + hag*10 - 3*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 4*hag, y5 + 1, x5 + hag*10 - 4*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 5*hag, y5 + 1, x5 + hag*10 - 5*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 6*hag, y5 + 1, x5 + hag*10 - 6*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 7*hag, y5 + 1, x5 + hag*10 - 7*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 8*hag, y5 + 1, x5 + hag*10 - 8*hag, y5 + h - 1, <0  0 127>]
line	[x5 + hag*10 - 9*hag, y5 + 1, x5 + hag*10 - 9*hag, y5 + h - 1, <0  0 127>]

text    [ x5- 20, y5 - 5, 90, 25, <127 127 127>, <100 255 0>, QROBOT3.�����_������� ]*/
/*
rect	[100, 100, 100, 20, <127 127 127>, <100 255 0> ]

Show_if QROBOT.�����_������� = 1
ellipse [ 180, 100, 20, 20,  <127 0 127>, <100 255 0> ]
Show_if QROBOT.�����_������� = 2
ellipse [ 180, 100, 20, 20,  <127 0 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 0 127>, <100 255 0> ]
Show_if QROBOT.�����_������� = 3
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
Show_if QROBOT.�����_������� = 4
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 120, 100, 20, 20,  <127 127 127>, <100 255 0> ]
Show_if QROBOT.�����_������� > 5
ellipse [ 180, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 160, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 140, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 120, 100, 20, 20,  <127 127 127>, <100 255 0> ]
ellipse [ 100, 100, 20, 20,  <127 127 127>, <100 255 0> ]
*/

//------------------������ ������ �������-----------------------------------
//active ��������_������� [x_st11, y_st11, gx_st11, gy_st11]
//active ��������_������� [x_st11, y_st11, gx_st11, gy_st11]

/*
active ��������_������� [500, 300, 80, 20]
rect	[500, 300, 80, 20, <0 127 127>, <200 255 100> ]
text    [ 510, 300, 80, 20, <0 127 127>, <100 255 0>, '�������' ]

active ��������_������� [500, 350, 80, 20]
rect	[500, 350, 80, 20, <255 255 127>, <200 255 100> ]
text    [ 510, 350, 80, 20, <255 255 127>, <100 255 0>, '��������' ]
*/

/*Show_if ST11.��������� = ��������

rect	[x_st11, y_st11, gx_st11, gy_st11, <255 255 127>, <200 255 100> ]

Show_if ST11.��������� = �����

rect	[x_st11, y_st11, gx_st11, gy_st11, <255 127 127>, <200 255 100> ]

Show_if ST11.��������� = ������

rect	[x_st11, y_st11, gx_st11, gy_st11, <0 127 127>, <0 255 100> ]
line	[x_st11 - 5, y_st11 - 5,x_st11 + gx_st11 + 5, y_st11 + gy_st11 + 5, <0  0 127>]
line	[x_st11 - 5, y_st11 + gy_st11 + 5,x_st11 + gx_st11 + 5, y_st11 - 5, <0  0 127>]
//---------------------------------------------------------------------------

Show_if ST12.��������� = ��������

rect	[x_st12, y_st12, gx_st12, gy_st12, <255 255 127>, <200 255 100> ]

Show_if ST12.��������� = �����

rect	[x_st12, y_st12, gx_st12, gy_st12, <255 127 127>, <200 255 100> ]

//---------------------------------------------------------------------------

Show_if ST13.��������� = ��������

rect	[x_st13, y_st13, gx_st13, gy_st13, <255 255 127>, <200 255 100> ]

Show_if ST13.��������� = �����

rect	[x_st13, y_st13, gx_st13, gy_st13, <255 127 127>, <200 255 100> ]
//------------------������ ������-----------------------------------

Show_if STANOK2.��������� = ��������

rect	[x_st2, y_st2, gx_st2, gy_st2, <255 255 127>, <200 255 100> ]

Show_if STANOK2.��������� = �����

rect	[x_st2, y_st2, gx_st2, gy_st2, <255 127 127>, <200 255 100> ]

Show
text    [ 600, 97, 90, 25, <127 127 127>, <100 255 0>, QEXIT.�����_������� ]
*/
/*
text   [10,70 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 1 � ���������:' ]
text   [350,70 ,350 ,25 , <127 127 127>, <0 0 0>, ������_1.���������]
text   [500,70 ,350 ,25 , <127 127 127>, <0 0 0>, ������_1.���������]
text   [10,85 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 2 � ���������:' ]
text   [350,85 ,350 ,25 , <127 127 127>, <0 0 0>, ������_2.���������]
text   [500,85 ,350 ,25 , <127 127 127>, <0 0 0>, ������_2.���������]
text   [10,100 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 3 � ���������:' ]
text   [350,100 ,350 ,25 , <127 127 127>, <0 0 0>, ������_3.���������]
text   [500,100 ,350 ,25 , <127 127 127>, <0 0 0>, ������_3.���������]

text   [10,120 ,350 ,25 , <127 127 127>, <0 0 0>, '�������_1 � ���������:' ]
text   [350,120 ,350 ,25 , <127 127 127>, <0 0 0>, �������_1.���������]
text   [500,120 ,350 ,25 , <127 127 127>, <0 0 0>, �������_1.���������]

text   [10,135 ,350 ,25 , <127 127 127>, <0 0 0>, '�������_2 � ���������:' ]
text   [350,135 ,350 ,25 , <127 127 127>, <0 0 0>, �������_2.���������]
text   [500,135 ,350 ,25 , <127 127 127>, <0 0 0>, �������_2.���������]

text   [10,150 ,350 ,25 , <127 127 127>, <0 0 0>, '�����_1 � ���������:' ]
text   [350,150 ,350 ,25 , <127 127 127>, <0 0 0>, �����_1.���������]
text   [500,150 ,350 ,25 , <127 127 127>, <0 0 0>, �����_1.���������]

text   [10,165 ,350 ,25 , <127 127 127>, <0 0 0>, '�����_2 � ���������:' ]
text   [350,165 ,350 ,25 , <127 127 127>, <0 0 0>, �����_2.���������]
text   [500,165 ,350 ,25 , <127 127 127>, <0 0 0>, �����_2.���������]

text   [10,180 ,350 ,25 , <127 127 127>, <0 0 0>, '�����_3 � ���������:' ]
text   [350,180 ,350 ,25 , <127 127 127>, <0 0 0>, �����_3.���������]
text   [500,180 ,350 ,25 , <127 127 127>, <0 0 0>, �����_3.���������]

text   [10,200 ,350 ,25 , <127 127 127>, <0 0 0>, '���-�� ������ � 1-� ����������:' ]
text   [350,200 ,350 ,25 , <127 127 127>, <0 0 0>, ����������_1.�������_����������]
text   [10,215 ,350 ,25 , <127 127 127>, <0 0 0>, '���-�� ������ � 2-� ����������:' ]
text   [350,215 ,350 ,25 , <127 127 127>, <0 0 0>, ����������_2.�������_����������]

text   [10,300 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 1:' ]
text   [250,300 ,350 ,25 , <127 127 127>, <0 0 0>, ������_1.���������]
text   [350,300 ,350 ,25 , <127 127 127>, <0 0 0>, ������_1.���������]

text   [10,315 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 2:' ]
text   [250,315 ,350 ,25 , <127 127 127>, <0 0 0>, ������_2.���������]
text   [350,315 ,350 ,25 , <127 127 127>, <0 0 0>, ������_2.���������]

text   [10,330 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 3:' ]
text   [250,330 ,350 ,25 , <127 127 127>, <0 0 0>, ������_3.���������]
text   [350,330 ,350 ,25 , <127 127 127>, <0 0 0>, ������_3.���������]

text   [10,345 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 4:' ]
text   [250,345 ,350 ,25 , <127 127 127>, <0 0 0>, ������_4.���������]
text   [350,345 ,350 ,25 , <127 127 127>, <0 0 0>, ������_4.���������]

text   [10,360 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 5:' ]
text   [250,360 ,350 ,25 , <127 127 127>, <0 0 0>, ������_5.���������]
text   [350,360 ,350 ,25 , <127 127 127>, <0 0 0>, ������_5.���������]

text   [10,375 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 6:' ]
text   [250,375 ,350 ,25 , <127 127 127>, <0 0 0>, ������_6.���������]
text   [350,375 ,350 ,25 , <127 127 127>, <0 0 0>, ������_6.���������]

text   [10,390 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 7:' ]
text   [250,390 ,350 ,25 , <127 127 127>, <0 0 0>, ������_7.���������]
text   [350,390 ,350 ,25 , <127 127 127>, <0 0 0>, ������_7.���������]

text   [10,405 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 8:' ]
text   [250,405 ,350 ,25 , <127 127 127>, <0 0 0>, ������_8.���������]
text   [350,405 ,350 ,25 , <127 127 127>, <0 0 0>, ������_8.���������]

text   [10,420 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 9:' ]
text   [250,420 ,350 ,25 , <127 127 127>, <0 0 0>, ������_9.���������]
text   [350,420 ,350 ,25 , <127 127 127>, <0 0 0>, ������_9.���������]

text   [10,435 ,350 ,25 , <127 127 127>, <0 0 0>, '������ 10:' ]
text   [250,435 ,350 ,25 , <127 127 127>, <0 0 0>, ������_10.���������]
text   [350,435 ,350 ,25 , <127 127 127>, <0 0 0>, ������_10.���������]
*/

$End
