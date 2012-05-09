#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_

#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPShapeProcessMJ)
	IS  INSTANCE_OF      (RPShape_MJ                       )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPShapeProcessMJ( RPObject* parent );
	virtual ~RPShapeProcessMJ();
	virtual rp::string getClassName() const { return "RPShapeProcessMJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	//��������� ��� ����������
	rdo::compiler::gui::LPRPShapeDataBlockProcess m_pParams;
	rdo::runtime::      LPIInternalStatistics     pInternalStatistics;
	//���������� ��� ���������
	tstring gname; // ���
	int gtype; // ����� ��������
	int base_gen;
	ruint m_currentTransactCountProc;
	
	//�������� �������
	double gexp;
	double gdisp;
	double gmax;
	double gmin;

	int action; // ��� �������� �� ��������� � �������
	int prior;
	int queue;
	int parameter;

	ruint indent;

	std::list< CString> list_resource_procMJ;

	// ��������������� ����������� ������� saveToXML � loadFromXML ��� RPShapeProcessMJ:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
