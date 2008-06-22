#ifndef RDOPARSER_VALUE_H
#define RDOPARSER_VALUE_H

#include <rdo_value.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue: public RDOParserSrcInfo
{
public:
	// ��� ���������� (����������� �� ����) ��������
	RDOValue( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& src_info = RDOParserSrcInfo() ):
		RDOParserSrcInfo( src_info ),
		m_value( value )
	{
	}
	// ��� t_identificator �������� ������ ���, �� �� ���
	RDOValue( const RDOParserSrcInfo& src_info ):
		RDOParserSrcInfo( src_info ),
		m_value( rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator) )
	{
	}
	// �������������� ���
	RDOValue():
		RDOParserSrcInfo(),
		m_value( rdoRuntime::RDOValue(rdoRuntime::g_unknow) )
	{
	}

	const rdoRuntime::RDOValue& value() const { return m_value; }

	bool defined() const { return m_value.typeID() != rdoRuntime::RDOType::t_unknow; }

private:
	rdoRuntime::RDOValue m_value;
};

} // namespace rdoParse

#endif // RDOPARSER_VALUE_H
