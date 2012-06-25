#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_

#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPProcessShapeProcess)
	IS  INSTANCE_OF      (RPProcessShape                   )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeProcess( RPObject* parent );
	virtual ~RPProcessShapeProcess();
	virtual rp::string getClassName() const { return "RPProcessShapeProcess"; }

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

	std::list< CString> m_resourceList;

	// ��������������� ����������� ������� saveToXML � loadFromXML ��� RPProcessShapeProcess:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
