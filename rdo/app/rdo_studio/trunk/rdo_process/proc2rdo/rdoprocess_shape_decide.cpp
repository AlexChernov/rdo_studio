#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class RPConnectorDockTrue: public RPConnectorDock
{
public:
	RPConnectorDockTrue( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockTrue() {};

	virtual rbool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
class RPConnectorDockFalse: public RPConnectorDock
{
public:
	RPConnectorDockFalse( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockFalse() {};

	virtual rbool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
RPShapeDecide::RPShapeDecide( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Decide") )
{

	pa_src.push_back(rp::point(-55, 0));
	pa_src.push_back(rp::point(0, -55));
	pa_src.push_back( rp::point(55, 0) );
	pa_src.push_back( rp::point(0, 55) );
	pa_src.push_back( rp::point(-55, 0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point(  -55, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point( 55, 0 ), 0, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  0, 55 ), 270, "transact" ) );
// ������������� ���������� ��� �������������
	 ptrue  = 0.9; 
	 pfalse = 0.1;
}

RPShapeDecide::~RPShapeDecide()
{
}

rp::RPXMLNode* RPShapeDecide::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

void RPShapeDecide::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// ���������� ���� <RPShapeDecide/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// ��x������ �������� �������:
	// 1) �������� �������
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) �������� ����������
	node.append_attribute(_T("name"))   .set_value(getName().c_str());
	node.append_attribute(_T("true"))   .set_value(ptrue            );
	node.append_attribute(_T("false"))  .set_value(pfalse           );
}

void RPShapeDecide::loadFromXML(CREF(pugi::xml_node) node)
{
	// ��������� �������� ��� �������� ������������ ����� "Decide":
	for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// ����������� ����������� � xml-����� ���������:
		// ��� ���������� (���������� ����)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("true"))
		{
			ptrue = attr.as_double();
		}
		else if (attrName == _T("false"))
		{
			pfalse = attr.as_double();
		}
	}
	// ����������� ������� �� Flowchart'�
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

RPObject* RPShapeDecide::newObject( RPObject* parent )
{
	return new RPShapeDecide( parent );
}

void RPShapeDecide::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeDecideDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeDecide::generate()
{
	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;
	//��������� ���� ���������� ��������� ��� *.opr
		CString name_value1("����_������_");
		CString name_value2(getName().c_str());
		CString name_value(name_value1 + name_value2);
		proc2rdo->list_pattern_names.push_back(name_value);
// ��������� ��������� ��� ���� *.pat

RDOfiles->pattern <<std::endl
<<std::endl<<"{-------���� ������------------"<<getName().c_str()<<"---------------------------------}"
<<std::endl
<<std::endl
<<std::endl<<"$Pattern ����_������_"<<getName().c_str()<<" : rule  {������������ ������}trace"
<<std::endl<<"	$Relevant_resources"
<<std::endl
<<std::endl<<"		_transact_X : Group_of_transacts_X keep"
<<std::endl
<<std::endl<<"$Body"
<<std::endl 
<<std::endl<<"  _transact_X"
<<std::endl<<"		 Choice from _transact_X.���������_��������������� = ������� and"
<<std::endl<<"		_transact_X.�����_����������_������� = "<<getName().c_str()
<<std::endl<<"			first"
<<std::endl	 
<<std::endl
<<std::endl<<"	Convert_rule"
<<std::endl<<"		  �����_����������            set "<<getName().c_str()
<<std::endl<<"			�����_����������_�������    set ����_"<<getName().c_str()
<<std::endl	 
<<std::endl<<"$End";

RDOfiles->function<<std::endl<<"{-------���� ������ ------" <<getName().c_str()<<"--------------}" <<std::endl
<<std::endl<<"$Sequence ����_"<<getName().c_str()<<": such_as Group_of_transacts_X.�����_����������"
	<<std::endl<<"$Type = by_hist"
	<<std::endl<<"$Body"
<<std::endl<<id_next.c_str()<<"  "<<ptrue
<<std::endl<<id_next2.c_str()<<"  "<<pfalse

	<<std::endl<<"	$End";
}