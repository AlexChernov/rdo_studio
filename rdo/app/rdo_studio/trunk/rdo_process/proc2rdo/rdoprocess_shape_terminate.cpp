#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/src/application.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RPShapeTerminateMJ::RPShapeTerminateMJ( RPObject* _parent )
	: RPShape_MJ(_parent, _T("Terminate"))
	, m_currentTransactCountDel(0)
{
	m_term_inc = 1;
	m_name     = _T("Terminate");

	// ������������� ���� ����� 
	type = "block";
		
	pa_src.push_back( rp::point(-50,   0) );
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point( 50, -25) );
	pa_src.push_back( rp::point( 50,  25) );
	pa_src.push_back( rp::point(-25,  25) );
	pa_src.push_back( rp::point(-50,   0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180, "transact" ) );

	indent = 5;
}

RPShapeTerminateMJ::~RPShapeTerminateMJ()
{
}

RPObject* RPShapeTerminateMJ::newObject( RPObject* parent )
{
	return new RPShapeTerminateMJ( parent );
}

void RPShapeTerminateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeTerminateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeTerminateMJ::generate()
{
	LPRPShapeTerminateMJ pThis(this);
	ASSERT(pThis);

	pInternalStatistics = pThis.interface_cast<rdo::runtime::IInternalStatistics>();
	ASSERT(pInternalStatistics);

	m_pParams = rdo::Factory<rdo::compiler::gui::RPShapeDataBlockTerminate>::create(m_name);
	m_pParams->setTermInc   (m_term_inc         );
	m_pParams->setStatistics(pInternalStatistics);

	studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_TERMINATE, m_pParams.get());

	m_pParams = NULL;
}

void RPShapeTerminateMJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// ���������� ���� <RShapeTerminateMJ/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	// 1) �������� �������
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) �������� ����������
	node.append_attribute(_T("name"))                .set_value(getName().c_str());
	node.append_attribute(_T("terminateCounter"))    .set_value(m_term_inc       );
}

void RPShapeTerminateMJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// ��������� �������� ��� �������� ������������ ����� "Terminate":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// ����������� ����������� � xml-����� ���������:
		// ��� ���������� (���������� ����)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("terminateCounter"))
		{
			m_term_inc = attr.as_int();
		}
	}
	// ����������� ������� �� Flowchart'�
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

void RPShapeTerminateMJ::setTransCount(ruint count)
{
	m_currentTransactCountDel = count;
	update();
}

void RPShapeTerminateMJ::drawCustom(REF(CDC) dc)
{
	dc.SetTextColor(RGB(0x00, 0x64, 0x00));
	dc.TextOut(this->pa_global.getMaxX() - 2*indent, this->pa_global.getMaxY() + indent, rp::string::fromint(m_currentTransactCountDel).c_str());
}
