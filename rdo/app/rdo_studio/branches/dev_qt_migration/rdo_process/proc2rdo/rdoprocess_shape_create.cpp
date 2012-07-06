#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/src/application.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RPProcessShapeCreate::RPProcessShapeCreate(PTR(RPObject) _parent)
	: RPProcessShape(_parent, _T("Create"))
	, m_createdTransactCount(0)
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// ������������� ���������� ��� �������������
	gname=_T("Create"); // ���
	gfirst=0; // ����� �������
	gtype=0; // ����� ��������
	base_gen=1234567890;
	//�������� �������
	gexp=0;
	gdisp=0;
	gmax=0;

	//������ ����
	inf=1000000; // ������������
	gID=1; // ID ������
	gpar1=0;
	gpar2=0;
	gpar3=0;

	indent = 5;
}

RPProcessShapeCreate::~RPProcessShapeCreate()
{
}

rp::RPXMLNode* RPProcessShapeCreate::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPProcessShape::save( parent_node );
	return obj_node;
}

void RPProcessShapeCreate::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// ���������� ���� <RPProcessShapeCreate/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	// 1) �������� �������
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) �������� ����������
	node.append_attribute(_T("name")).set_value(getName().c_str());
	if (gamount)
	{
		node.append_attribute(_T("amount")).set_value(gamount.get());
	}
	node.append_attribute(_T("base_gen")).set_value(base_gen);
	node.append_attribute(_T("exp"))     .set_value(gexp    );
	node.append_attribute(_T("disp"))    .set_value(gdisp   );
	node.append_attribute(_T("zakon"))   .set_value(gtype   );
}

void RPProcessShapeCreate::loadFromXML(CREF(pugi::xml_node) node)
{
	// ��������� �������� ��� �������� ������������ ����� "Create":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// ����������� ����������� � xml-����� ���������
		// ��� ���������� (���������� ����)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("amount"))
		{
			ruint amount = attr.as_uint();
			gamount = amount > 0
				? amount
				: boost::optional<ruint>();
		}
		else if (attrName == _T("base_gen"))
		{
			base_gen = attr.as_int();
		}
		else if (attrName == _T("exp"))
		{
			gexp = attr.as_double();
		}
		else if (attrName == _T("disp"))
		{
			gdisp = attr.as_double();
		}
		else if (attrName == _T("zakon"))
		{
			gtype = attr.as_int();
		}
	}
	// ����������� ������� �� Flowchart'�
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

RPObject* RPProcessShapeCreate::newObject( RPObject* parent )
{
	return new RPProcessShapeCreate( parent );
}

void RPProcessShapeCreate::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPProcessShapeCreateDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPProcessShapeCreate::generate()
{
	rdo::compiler::gui::RPShapeDataBlock::zakonRaspr zakon;
	switch(gtype)
	{
		case 0: // ���������
			zakon = rdo::compiler::gui::RPShapeDataBlock::Const;
			break;	
		case 1: // ����������
			zakon = rdo::compiler::gui::RPShapeDataBlock::Normal;
			break;
		case 2: // ����������� �����
			zakon = rdo::compiler::gui::RPShapeDataBlock::Uniform;
			break;
		case 3: // �����������
			zakon = rdo::compiler::gui::RPShapeDataBlock::Triangular;
			break;
		case 4: // ����������������
			zakon = rdo::compiler::gui::RPShapeDataBlock::Exp;
			break;
	}

	LPRPProcessShapeCreate pThis(this); 
	ASSERT(pThis);

	pInternalStatistics = pThis.interface_cast<rdo::runtime::IInternalStatistics>();
	ASSERT(pInternalStatistics);

	m_pParams = rdo::Factory<rdo::compiler::gui::RPShapeDataBlockCreate>::create(zakon, gname);
	m_pParams->setBase(base_gen);
	m_pParams->setAmount(gamount);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);
	m_pParams->setMax(gmax);
	m_pParams->setStatistics(pInternalStatistics);

	studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_CREATE, m_pParams.get());

	m_pParams = NULL;

	m_createdTransactCount = 0;
	update();
}

void RPProcessShapeCreate::setTransCount(ruint count)
{
	m_createdTransactCount = count;
	update();
}

void RPProcessShapeCreate::drawCustom(REF(CDC) dc)
{
	dc.SetTextColor(RGB(0x00, 0x64, 0x00));
	dc.TextOut((3*(this->pa_global.getMaxX()) + (this->pa_global.getMinX()))/4 - indent, this->pa_global.getMaxY() + indent, rp::string::fromint(m_createdTransactCount).c_str());
}
