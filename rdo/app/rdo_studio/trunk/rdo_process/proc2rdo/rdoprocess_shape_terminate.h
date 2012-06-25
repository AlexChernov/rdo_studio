#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPShapeTerminateMJ)
	IS  INSTANCE_OF      (RPShape_MJ                       )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	tstring m_name;
	int     m_term_inc;
	ruint   m_currentTransactCountDel;
	ruint   indent;

	rdo::compiler::gui::LPRPShapeDataBlockTerminate m_pParams;
	rdo::runtime::      LPIInternalStatistics       pInternalStatistics;

	RPShapeTerminateMJ( RPObject* parent );
	virtual ~RPShapeTerminateMJ();

	virtual rp::string getClassName() const { return "RPShapeTerminateMJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	// ���������������� ����������� ������� saveToXML � loadFromXML ��� RPShapeTerminateMJ:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
