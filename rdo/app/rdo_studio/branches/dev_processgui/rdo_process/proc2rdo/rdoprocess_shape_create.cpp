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

RPShapeCreateMJ::RPShapeCreateMJ(PTR(RPObject) _parent)
	: RPShape_MJ(_parent, _T("Create"))
	, m_currentTransactCount(0)
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
	gamount=100; // ���-�� �����������
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

}

RPShapeCreateMJ::~RPShapeCreateMJ()
{
}

rp::RPXMLNode* RPShapeCreateMJ::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

void RPShapeCreateMJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// ���������� ���� <RPShapeCreateMJ/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	// 1) �������� �������
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) �������� ����������
	node.append_attribute(_T("name"))     .set_value(getName().c_str());
	node.append_attribute(_T("amount"))   .set_value(gamount          );
	node.append_attribute(_T("base_gen")) .set_value(base_gen         );
	node.append_attribute(_T("exp"))      .set_value(gexp             );
	node.append_attribute(_T("disp"))     .set_value(gdisp            );
	node.append_attribute(_T("zakon"))    .set_value(gtype            );
}

void RPShapeCreateMJ::loadFromXML(CREF(pugi::xml_node) node)
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
			gamount = attr.as_int();
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

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeCreateMJ::generate()
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

	LPRPShapeCreateMJ pThis(this); 
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
}

void RPShapeCreateMJ::setTransCount(ruint count)
{
	m_currentTransactCount = count;
}
