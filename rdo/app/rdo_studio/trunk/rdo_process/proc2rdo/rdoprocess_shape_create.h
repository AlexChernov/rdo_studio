#ifndef _PROC2RDO_RDOPROCESS_SHAPE_CREATE_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_CREATE_H_

#include <boost/optional.hpp>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPProcessShapeCreate)
	IS  INSTANCE_OF      (RPProcessShape                   )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeCreate( RPObject* parent );
	virtual ~RPProcessShapeCreate();

	virtual rp::string getClassName() const { return "RPProcessShapeCreate"; }
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// ���������������� ����������� ������� saveToXML ��� RPProcessShapeCreate:
	void saveToXML(REF(pugi::xml_node) parentNode) const;
	// ���������������� ����������� ������� loadFromXML ��� RPProcessShapeCreate:
	void loadFromXML(CREF(pugi::xml_node) node);

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	//virtual void list_name();
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	//��������� ��� ����������
	rdo::compiler::gui::LPRPShapeDataBlockCreate m_pParams;
	rdo::runtime      ::LPIInternalStatistics    pInternalStatistics;
	//���������� ��� ���������
	tstring gname; // ���
	double gfirst; // ����� �������
	boost::optional<ruint> gamount; // ���-�� �����������
	int gtype; // ����� ��������
	int base_gen;
	ruint m_createdTransactCount;
	
	//�������� �������
	double gexp;
	double gdisp;
	double gmax;

	//������ ����
	int inf; // ������������
	int gID; // ID ������
	double gpar1, gpar2, gpar3;

	ruint indent;
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_CREATE_H_
