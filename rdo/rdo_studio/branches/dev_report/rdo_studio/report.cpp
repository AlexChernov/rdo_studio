#include "stdafx.h"
#include "report.h"

using namespace std ;

// ����������� ������ ReportParser
ReportParser::ReportParser(std::string UserString)
{
	StringToParse = UserString;
    ParserStatus = RPS_COMMENTSNOTDELETED;
	CurrentWordIndex = 0;
}

// ������� �������� ������������
int ReportParser:: DeleteComments() 
{
    int i = 0 ;	
	DeleteCommentsFlagType ParseFlag = DC_NOCOMMENT ;
	while ( i < StringToParse.length() )
	{
       switch( StringToParse[i] )
	   {
	  	  case '/':  {
			         if ( StringToParse[i + 1] == '/' && StringToParse[i + 2] != '#' && ParseFlag == DC_NOCOMMENT )
	  				   ParseFlag = DC_LINECOMMENT;
	                 if ( StringToParse[i+1] == '*' && ParseFlag == DC_NOCOMMENT )
					   ParseFlag = DC_LONGCOMMENT;
	                 } 
		             break;
		  case '*':  if ( StringToParse[i+1] == '/' && ParseFlag == DC_LONGCOMMENT )
					 {
					   ParseFlag = DC_ENDLONGCOMMENT ;
					   i++;
					 }
	                 break;

		  case '\n': if ( ParseFlag == DC_LINECOMMENT )
					 ParseFlag = DC_NOCOMMENT ; 
			         break ;
	    }
	  // ���������� ����� �����
      if ( ParseFlag == DC_NOCOMMENT )
        StringToParse_wo_Comments  += StringToParse[i]; 
	  
	  if ( ParseFlag == DC_ENDLONGCOMMENT )
        ParseFlag = DC_NOCOMMENT;
	 
	  i++ ;   
	}
	StringToParse_wo_Comments += '\0';
	ParserStatus = RPS_COMMENTSDELETED;
	return 0 ;
}

int ReportParser::StringToList()
{
    int ParseIndex = 0;
	std::string ListBuffer = "";
	// ���� ���������� �� ������� - �� �������� �� ������
	if (ParserStatus == RPS_COMMENTSNOTDELETED)
		return 1;

	while( StringToParse_wo_Comments[ParseIndex] != '\0' )
	{
		// ���������� �������, �������� ������ � ���������
		while( StringToParse_wo_Comments[ParseIndex] == ' ' || StringToParse_wo_Comments[ParseIndex] == '\n' || StringToParse_wo_Comments[ParseIndex] == 9 || StringToParse_wo_Comments[ParseIndex] == 13 )
			ParseIndex++;
		// �������� �����
		while( StringToParse_wo_Comments[ParseIndex] != ' ' && StringToParse_wo_Comments[ParseIndex] != '\n' && StringToParse_wo_Comments[ParseIndex] != '\0' && StringToParse_wo_Comments[ParseIndex] != ':' && StringToParse_wo_Comments[ParseIndex] != 9 && StringToParse_wo_Comments[ParseIndex] != 13 )
			ListBuffer += StringToParse_wo_Comments[ParseIndex++];
		// ��������� �������� ����� � ������
		if ( ListBuffer != "" )
			ListToParse.push_back( ListBuffer );
		// ������� �����
		ListBuffer = "" ;
		// ���� ��������� ��������� - �������� ��� � ��������� �����
		if ( StringToParse_wo_Comments[ParseIndex] == ':' )
		{
			ListToParse.push_back( ":" );
			ParseIndex++ ;
		}
	}
	ParserStatus = RPS_LISTCREATED;
	return 0;
}


int ReportParser::FindNextKeyWord()
{
  int i, j;
  for ( i = CurrentWordIndex; i < ListToParse.size(); i++ )
	for ( j = 0; j < ReportParserKeyWordsQuantity; j++ )
		if ( ListToParse.at(i) == ReportParserKeyWords[j] )
		{
			KeyWordType = (ReportParser::ReportParserKeyWordType)j;
			CurrentWordIndex = i + 1;
			return 0; 
		}
  KeyWordType = RPKWT_END ;
  CurrentWordIndex = 0;
  return 1 ;
}

int GroopManager::AddGroop(std::string GroopName_, bool Activity)
{
Groop Buffer ;  
int i = 0;
bool flag = 1 ;
for ( i = 0; i < GroopsVector.size() ; i++ )
{ 
	Buffer = GroopsVector.at(i) ;
    // ���� ������ ������� - ������������� ActivityFlag
	if ( Buffer.GroopName  == GroopName_ ) 
	{
		ActivityPattern.at(i) = Activity;
		flag = 0 ;
	}	
}
// ���� ������ �� ������� - ��������� � �����
if (flag)
{
	Buffer.GroopName = GroopName_;
    Buffer.Variables.clear();
	ActivityPattern.push_back(Activity);
	// ���� ��� ������ Chart - ������� �� ��������
	if (GroopName_ == "Chart") 
		Buffer.Type = Buffer.GR_CHART ;
    GroopsVector.push_back(Buffer) ;
}
return 0 ;
}

int GroopManager::InsertVar( std::string VarName, ReportVar::VarTypeValues VarType_ )
{
int i = 0;
ReportVar Buffer;
Buffer.Name = VarName;
Buffer.VarType = VarType_;
for ( i = 0; i < GroopsVector.size(); i++ )
	if ( ActivityPattern.at(i) )
		GroopsVector.at( i ).Variables.push_back( Buffer );
return 0 ;
}

int GroopManager::GetParam()
{
int i, j, k;
ReportVar VarBuf;
// ������� ���� - �������� ������
for ( i = 0; i < GroopsVector.size(); i++ )
{
	// ������������� ���� - �������� ����������
	for ( j = 0; j < GroopsVector.at( i ).Variables.size(); j++ )
	{
		// ��������� � ����� ������� ����������  
		VarBuf = GroopsVector.at( i ).Variables.at( j );
		for ( k = 0; k < List.size(); k++ )
		{
			if ( List.at( k ) == VarBuf.Name )
			{
				switch ( VarBuf.VarType )
				{
					// ���� ���������� ����� ��� Get_value �� ��������� ���� ��������� ������� ��������
				case (ReportVar::VarTypeValues) 4 : GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
						            GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
						            GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
									GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( "" );
					break ;
					default: GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 1 ) );
							 GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 2 ) );
						     GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 3 ) );
							 GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 4 ) );
							 GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 5 ) );
							 GroopsVector.at( i ).Variables.at( j ).Parameters.push_back( List.at( k + 6 ) );
							 
					break ;
			    }
			}
		}
	}
}
return 0;
}




