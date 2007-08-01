unit Var_Unit;

interface

uses IniFiles, TopBox_Unit;

type

  ChipNumType = record
    Mesto: integer;
    Look : boolean;
  end;

var
  MouseX, MouseY: integer;   // ���������� � �������� ScrollBox'�
                             //   (��� ������� ������ box'�, arrow)
  ScrollX, ScrollY: integer; // ���������� TopScrollBox.Horz|Vert ScrollBar
  IniFile: TIniFile;    // ini file (*.ini)
// TopBox
  TopBox, TopBoxFirst, TopBoxLast: PTopBox;
  // ��� ��������������/���������� Box
  TopBoxMode: (Normal, MyBoxDrag, SizeUp, SizeDown, SizeLeft, SizeRight,
               SizeUpLeft, SizeUpRight, SizeDownLeft, SizeDownRight);
  MyTopBoxChange: boolean; // �����������/����������� Box'�
  // ��� �������������� ��������� � '�������� ��������'
  OldChip: array [1..6] of ChipNumType;

implementation

end.
