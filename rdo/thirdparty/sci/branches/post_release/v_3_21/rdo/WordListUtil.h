/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      WordListUtil.h
  \author    ������� ������� (robot.xet@gmail.com)
  \date      29.09.2012
  \brief     
  \indent    4T
*/

class WordListUtil
{
public:
	WordListUtil(const WordList& wordlist);
	//~WordListUtil();
	WordListUtil& operator=( const WordListUtil&) {}
	//tstring GetNearestWord(const char *wordStart, int searchLen) const;
	std::vector<tstring> GetNearestWords(const char *wordStart) const;

private:
	const WordList& wl;
};