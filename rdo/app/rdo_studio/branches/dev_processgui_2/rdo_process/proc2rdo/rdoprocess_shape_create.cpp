// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeCreateMJ::RPShapeCreateMJ(PTR(RPObject) _parent)
	: RPShape_MJ(_parent, _T("Create"))
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
	gexp=10;
	gdisp=0;

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

void RPShapeCreateMJ::saveToXML(REF(pugi::xml_node) parentNode)
{
	// ���������� ���� <RShapeCreateMJ/>:
	pugi::xml_node      node        = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	pugi::xml_attribute nameAttr    = node.append_attribute(_T("gname"));
	nameAttr.set_value(getName().c_str());
	pugi::xml_attribute amountAttr  = node.append_attribute(_T("gamount"));
	amountAttr.set_value(gamount);
	pugi::xml_attribute basegenAttr = node.append_attribute(_T("base_gen"));
	basegenAttr.set_value(base_gen);
	pugi::xml_attribute gexpAttr    = node.append_attribute(_T("gexp"));
	gexpAttr.set_value(gexp);
	pugi::xml_attribute gdispAttr   = node.append_attribute(_T("gdisp"));
	gdispAttr.set_value(gdisp);
	pugi::xml_attribute zakonAttr   = node.append_attribute(_T("zakon"));
	zakonAttr.set_value(gtype);
}

void RPShapeCreateMJ::loadFromXML(REF(pugi::xml_node) Node)
{
	// ���� � ��������������� FlowChart'�, ��������� �� Node'�, ����(�) "Create":
	for(pugi::xml_node node = Node.first_child(); node; node = node.next_sibling())
	{
		if(strcmp(node.name(), getClassName().c_str()) == 0)
		{
			// ��������� �������� ��� �������� ������������ ����� "Create":
			for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
			{
				// ����������� ����������� � xml-����� ���������:
				gname    = node.attribute(_T("gname")).value();
				gamount  = node.attribute(_T("gamount")).as_int();
				base_gen = node.attribute(_T("base_gen")).as_int();
				gexp     = node.attribute(_T("gexp")).as_double();
				gdisp    = node.attribute(_T("gdisp")).as_double();
				gtype    = node.attribute(_T("zakon")).as_int();
			}
		}
	}

	// ������� ���� RPShapeCreateMJ:
	RPShapeCreateMJ* p = static_cast<RPShapeCreateMJ*>(rpMethod::factory->getNewObject(getClassName().c_str(), rpMethod::project));
	p->generate();
}

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeCreateMJ::generate()
{
	RPShapeDataBlock::zakonRaspr zakon;
	switch(gtype)
	{
		case 0: // ���������
			zakon = RPShapeDataBlock::Const;
			break;	
		case 1: // ����������
			zakon = RPShapeDataBlock::Normal;
			break;
		case 2: // ����������� �����
			zakon = RPShapeDataBlock::Uniform;
			break;
		case 3: // ����������������
			zakon = RPShapeDataBlock::Exp;
			break;
	}

	m_pParams = rdo::Factory<RPShapeDataBlockCreate>::create(zakon, gname);
	m_pParams->setBase(base_gen);
	m_pParams->setAmount(gamount);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);

	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_CREATE, m_pParams.get());

	m_pParams = NULL;
}
