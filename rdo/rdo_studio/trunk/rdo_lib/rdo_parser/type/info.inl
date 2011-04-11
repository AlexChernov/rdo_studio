/*
 * copyright: (c) RDO-Team, 2011
 * filename : info.inl
 * author   : ������ ������
 * date     : 09.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------
inline TypeInfo::~TypeInfo()
{}

inline CREF(LPRDOType) TypeInfo::type() const
{
	return m_pType;
}

inline RDOParserSrcInfo TypeInfo::src_info() const
{
	return m_srcInfo ? m_srcInfo.get() : RDOParserSrcInfo();
}

inline CREF(RDOParserSrcInfo) TypeInfo::src_info(CREF(RDOParserSrcInfo) srcInfo) const
{
	return m_srcInfo ? m_srcInfo.get() : srcInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
