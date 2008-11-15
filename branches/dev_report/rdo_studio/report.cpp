#include "stdafx.h"
#include "report.h"

using namespace std ;

// �������� �����
const std::string KeyWords[] = 
{
	"//#groop", 
    "//#endgroop",
    "watch_state",      // ����� ����������� - ����� ����������
    "watch_par",        // ����� ����������� - ����� ����������
    "get_value",		// ���� ���������� - ����� ����������
	"watch_quant",      // ����� ����������� - ����� ����������
    "watch_value"		// ����� ����������� - ����� ����������
} ;

int StringToList(std::string Str, vector <std::string> *List)
{
    int i = 0 ;
	std::string ListBuffer = "" ;
	while(Str[i] != '\0')
	{
		// ���������� �������, �������� ������ � ���������
		while(Str[i] == ' ' || Str[i] == '\n' || Str[i] == 9 || Str[i] == 13)
		  i++ ;
		// �������� �����
		while(Str[i] != ' ' && Str[i] != '\n' && Str[i] != '\0' && Str[i] != ':' && Str[i] != 9 && Str[i] != 13)
	      ListBuffer += Str[i++] ;
		// ��������� �������� ����� � ������
		if (ListBuffer != "")
			List -> push_back(ListBuffer) ;
		// ������� �����
		ListBuffer = "" ;
		// ���� ��������� ��������� - �������� ��� � ��������� �����
		if (Str[i] == ':')
		{
			List -> push_back(":") ;
			i++ ;
		}
	}
	return 0 ;
}


int DeleteComments(std::string Source, std::string *Result)
{
    int ParseIndex = 0 ;	
	int ParseFlag = NOCOMMENT ;
	while (ParseIndex < Source.length())
	{
       switch(Source[ParseIndex])
	   {
	  	  case '/' : {
			  if (Source[ParseIndex + 1] == '/' && Source[ParseIndex+2] != '#' && ParseFlag == NOCOMMENT)
	  				   ParseFlag = LINECOMMENT ;
	                 if (Source[ParseIndex+1] == '*' && ParseFlag == NOCOMMENT)
					   ParseFlag = LONGCOMMENT ;
	                 break ;
	                 } 
		  case '*' : if (Source[ParseIndex+1] == '/' && ParseFlag == LONGCOMMENT)
					 {
					   ParseFlag = ENDLONGCOMMENT ;
					   ParseIndex++;
					 }
			break ;

		  case '\n': if (ParseFlag == LINECOMMENT)
					   ParseFlag = NOCOMMENT ; 
			break ;
	    }
	  
	   // ���������� ����� �����
      if (ParseFlag == NOCOMMENT)
        *Result  += Source[ParseIndex] ; 
	  
	  if (ParseFlag == ENDLONGCOMMENT)
        ParseFlag = NOCOMMENT ;
	 
	  ParseIndex++ ;   
	}
	
	*Result += '\0' ;
	return 0 ;
}


int AddGroop(vector <Groop> *Groops, std::string GroopName_, bool ActivityFlag)
{
  //vector <Groop>::iterator Groop_Iter ;
  Groop Buffer ;
  
  int i = 0;
  bool flag = 1 ;
  for (i = 0; i < Groops->size() ; i++)
  {
    
	Buffer = Groops -> at(i) ;
    // ���� ������ ������� - ������������� ActivityFlag
	if (Buffer.GroopName  == GroopName_ ) 
	{
	    Groops -> at(i).IsActive = ActivityFlag ;
		flag = 0 ;
	}	
  }

  // ���� ������ �� ������� - ��������� � �����
  if (flag)
  {
    Buffer.GroopName = GroopName_ ;
    Buffer.Variables.clear() ;
	
	Buffer.IsActive = ActivityFlag ;
    Groops->push_back(Buffer) ;
  }
 
  return 0 ;
}

int InsertVar(vector <Groop> *Groops, std::string VarName, int VarType_)
{
  int i = 0 ;
  ReportVar Buffer ;
  Buffer.Name = VarName ;
  Buffer.VarType = VarType_ ;
  for (i = 0; i < Groops->size(); i++)
    if (Groops -> at(i).IsActive)
	  Groops -> at(i).Variables.push_back(Buffer) ;

  return 0 ;
}


int FindKeyWord(vector <std::string> List, int StartIndex ,int *WhatWord) 
{
  int i, j ;
  for (i = StartIndex; i < List.size(); i++)
	for (j = 0; j < KEYWORDS_QUANTITY; j++)
		if (List.at(i) == KeyWords[j])
		{
			*WhatWord = j ;
			return i + 1 ;
		}
	*WhatWord = -1 ;
	return -1 ;
}

int GetParam(vector <Groop> *Groops, vector <std::string> List)
{
  int i, j, k ;
  ReportVar VarBuf ;
  for (i = 0; i < Groops->size(); i++)
  {
	  for (j = 0; j < Groops->at(i).Variables.size(); j++)
	  {
		  VarBuf = Groops->at(i).Variables.at(j) ;
		  for (k = 0; k < List.size(); k++)
		  {
				if(List.at(k) == VarBuf.Name)
				{
				  switch (VarBuf.VarType)
				  {
				  case 4: Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+1)) ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
						  Groops->at(i).Variables.at(j).Parameters.push_back("") ;
					  break ;
				  default:  Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+1)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+2)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+3)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+4)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+5)) ;
							Groops->at(i).Variables.at(j).Parameters.push_back(List.at(k+6)) ;
					  break ;
				  }
				}
		  }
	  }
  }
  return 0 ;
}



