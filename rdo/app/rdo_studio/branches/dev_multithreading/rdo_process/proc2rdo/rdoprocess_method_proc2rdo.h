#ifndef RDO_PROCESS_METHOD_PROC2RDO_H
#define RDO_PROCESS_METHOD_PROC2RDO_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_creation_RDO_files.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "utils/rdotypes.h"

class RPMethodProc2RDO_MJ: public rpMethod::RPMethod, public RPObject_MJ
{
private:
	virtual ~RPMethodProc2RDO_MJ();

protected:
	void registerObject();

	CToolBar toolbar;
	int btn_generate;
	int btn_generate_setup;
	virtual void buttonCommand( int button_id );
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update );

public:
	RPMethodProc2RDO_MJ( RPObject* _parent );
	static rpMethod::RPMethod* registerMethod();
	rbool checkModelStructure();
	virtual rp::string getVersionDesc() const { return "�����"; }
	virtual rp::string getDescription() const { return "��������� ���������� � ��������"; }
	virtual rp::string getClassName() const { return "RPMethodProc2RDO_MJ"; }
	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent );
	virtual void generate();
	double generate_time_MJ;
	RPCreationRDOFilesMJ* RDOfiles;
	std::list< CString > list_pattern_names; // MJ 7.04.06 �������� ����� ���� ��������� ��� ������ � ���� *.opr generate() ��������� ���
	
	// ��������� ��������� ������� saveToXML � loadFromXML � RPMethod
	// (��-�� ��������� ������ - ����� RPMethod �� ������ ������������� �� RPObject)
	virtual void saveToXML  (REF (pugi::xml_node) parentNode) const;
	virtual void loadFromXML(CREF(pugi::xml_node) node);
};

extern RPMethodProc2RDO_MJ* proc2rdo;

#endif // RDO_PROCESS_METHOD_PROC2RDO_H
