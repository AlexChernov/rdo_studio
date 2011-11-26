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
	pugi::xml_node      node        = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	// 1) �������� �������
	pugi::xml_attribute nameAttr    = node.append_attribute("gname");
						nameAttr.set_value(getName().c_str());
	pugi::xml_attribute position_X  = node.append_attribute("pos_X");
						position_X.set_value(getCenter().x);
	pugi::xml_attribute position_Y  = node.append_attribute("pos_Y");
						position_Y.set_value(getCenter().y);
	pugi::xml_attribute scale_X     = node.append_attribute("scale_X");
						scale_X.set_value(getScaleX());
	pugi::xml_attribute scale_Y     = node.append_attribute("scale_Y");
						scale_Y.set_value(getScaleY());
	// 2) �������� ����������
	pugi::xml_attribute amountAttr  = node.append_attribute("gamount");
						amountAttr.set_value(gamount);
	pugi::xml_attribute basegenAttr = node.append_attribute("base_gen");
						basegenAttr.set_value(base_gen);
	pugi::xml_attribute gexpAttr    = node.append_attribute("gexp");
						gexpAttr.set_value(gexp);
	pugi::xml_attribute gdispAttr   = node.append_attribute("gdisp");
						gdispAttr.set_value(gdisp);
	pugi::xml_attribute zakonAttr   = node.append_attribute("zakon");
						zakonAttr.set_value(gtype);
}

void RPShapeCreateMJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// ��������� �������� ��� �������� ������������ ����� "Create":
	for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// ����������� ����������� � xml-����� ���������:
		// 1) ��� ����������� ������� �� Flowchart'�
		if ( strcmp(attr.name(), "gname")    == 0 )				setName   (attr.value());
		if ( strcmp(attr.name(), "pos_X")    == 0 )				setX      (attr.as_double());
		if ( strcmp(attr.name(), "pos_Y")    == 0 )				setY      (attr.as_double());
		if ( strcmp(attr.name(), "scale_X")  == 0 )				setScaleX (attr.as_double());
		if ( strcmp(attr.name(), "scale_Y")  == 0 )				setScaleY (attr.as_double());
		// 2) ��� ���������� (���������� ����)
		if ( strcmp(attr.name(), "gamount")  == 0 )				gamount   = attr.as_int();
		if ( strcmp(attr.name(), "base_gen") == 0 )				base_gen  = attr.as_int();
		if ( strcmp(attr.name(), "gexp")     == 0 )				gexp      = attr.as_double();
		if ( strcmp(attr.name(), "gdisp")    == 0 )				gdisp     = attr.as_double();
		if ( strcmp(attr.name(), "zakon")    == 0 )				gtype     = attr.as_int();
	}
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
		case 3: // �����������
			zakon = RPShapeDataBlock::Triangular;
			break;
		case 4: // ����������������
			zakon = RPShapeDataBlock::Exp;
			break;
	}

	m_pParams = rdo::Factory<RPShapeDataBlockCreate>::create(zakon, gname);
	m_pParams->setBase(base_gen);
	m_pParams->setAmount(gamount);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);
	m_pParams->setMax(gmax);

	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_CREATE, m_pParams.get());

	m_pParams = NULL;
}
