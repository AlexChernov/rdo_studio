/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_lexer.inl
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
inline RDOLexer::RDOLexer(PTR(RDOParser) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout)
	: yyFlexLexer(yyin, yyout)
	, m_pParser  (pParser    )
	, m_yyin     (yyin       )
	, m_yyout    (yyout      )
	, m_lpval    (NULL       )
	, m_lploc    (NULL       )
	, m_enumEmpty(true       )
	, m_array_param_cnt(0    )
{}

inline void RDOLexer::loc_init()
{
	if (m_lploc)
	{
		m_lploc->m_first_line     = 0;
		m_lploc->m_first_pos      = 0;
		m_lploc->m_last_line      = 0;
		m_lploc->m_last_pos       = 0;
		m_lploc->m_first_seek     = 0;
		m_lploc->m_last_seek      = 0;
		m_lploc->m_last_seek_real = 0;
	}
}

inline void RDOLexer::loc_action()
{
	if (m_lploc)
	{
		m_lploc->m_first_line = m_lploc->m_last_line;
		m_lploc->m_first_pos  = m_lploc->m_last_pos;
		m_lploc->m_first_seek = m_lploc->m_last_seek_real;
		m_lploc->m_last_seek  = m_lploc->m_last_seek_real;
		for (int i = 0; i < YYLeng(); i++)
		{
			switch (YYText()[i])
			{
			case '\n': 
				m_lploc->m_last_line++;
				m_lploc->m_last_pos = 0;
				break;
			case '\r':
				m_lploc->m_last_pos = 0;
				break;
			default:
				m_lploc->m_last_pos++;
				m_lploc->m_last_seek++;
				break;
			}

			m_lploc->m_last_seek_real++;
		}
	}
}

inline void RDOLexer::loc_delta_pos(int value)
{
	if (m_lploc)
	{
		m_lploc->m_first_pos += value;
		m_lploc->m_last_pos  += value;
	}
}

inline void RDOLexer::setvalue(int value)
{
	*m_lpval = value;
}

inline PTR(RDOParser) RDOLexer::parser()
{
	return m_pParser;
}

inline void RDOLexer::enumBegin()
{
	m_enumEmpty = false;
}

inline void RDOLexer::enumReset()
{
	m_enumEmpty = true;
}

inline rbool RDOLexer::enumEmpty()
{
	return m_enumEmpty;
}
inline void RDOLexer::array_cnt_pls()
{
	m_array_param_cnt++;
}
inline void RDOLexer::array_cnt_rst()
{
	m_array_param_cnt = 0;
}
inline rsint RDOLexer::array_cnt_shw()
{
	return m_array_param_cnt;
}
CLOSE_RDO_PARSER_NAMESPACE
