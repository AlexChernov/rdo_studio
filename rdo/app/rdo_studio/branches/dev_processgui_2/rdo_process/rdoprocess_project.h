#ifndef RDO_PROCESS_PROJECT_MFC_H
#define RDO_PROCESS_PROJECT_MFC_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_project.h"
#include "utils/rdodebug.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// --------------------------------------------------------------------------------
// ---------- RPProjectMFC
// --------------------------------------------------------------------------------
class RPCtrlToolbarMFC;

class RPProjectMFC: public RPProject
{
private:
	std::list< RPCtrlToolbarMFC* > toolbars;

public:
	RPProjectMFC();
	virtual ~RPProjectMFC();
	virtual rp::string getClassName() const { return "RPProjectMFC"; }
	virtual std::ofstream& log() const;
	virtual RPPixmap*      createBitmap( char* xpm[] );
	virtual void makeFlowChartWnd( RPObjectFlowChart* flowobj );

	void open();
	void save();
	virtual void load( rp::RPXMLNode* node );
	// ���������������� ����������� ������� saveToXML ��� RPProjectMFC:
	void saveToXML(REF(pugi::xml_node) parentNode);
};

#endif // RDO_PROCESS_PROJECT_MFC_H
