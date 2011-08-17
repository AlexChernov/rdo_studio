/*
 * copyright: (c) RDO-Team, 2011
 * file     : context.inl
 * author   : ������ ������
 * date     : 03.03.2011
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

template <class T>
inline rdo::intrusive_ptr<T> Context::cast()
{
	LPContext pThis = this;
	rdo::intrusive_ptr<T> pThisResult = pThis.object_dynamic_cast<T>();
	if (pThisResult)
	{
		return pThisResult;
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->cast<T>() : rdo::intrusive_ptr<T>();
}

CLOSE_RDO_PARSER_NAMESPACE
