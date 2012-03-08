#ifndef RDO_PROCESS_MESSAGES_H
#define RDO_PROCESS_MESSAGES_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "utils/rdodebug.h"

// --------------------------------------------------------------------------------
// -------------------- RPMessages
// --------------------------------------------------------------------------------
class RPProject;

namespace rp {

class msg: public RPObject
{
friend class RPProject;

public:
	enum Messages {
		RP_OBJ_BEFOREDELETE = 0,   // from - ��������� ������                        / param �� ������������
		RP_OBJ_NAMECHANGED,        // from - ������, � �������� ��������� ���        / param �� ������������
		RP_OBJ_SELCHANGED,         // from - ������, � �������� ���������� ��������� / param = bool*
		RP_FLOWSTATE_CHANGED       // from - ��������� �� ������                     / param = RPProject::FlowState*
	};

protected:
	msg();
	virtual ~msg();

	typedef std::multimap< UINT, RPObject* > Connected;
	Connected connected;

	virtual void notify( RPObject* from, UINT message, void* param = NULL );

	void sendMessage( RPObject* from, UINT message, void* param = NULL );

public:
	virtual rp::string getClassName() const { return "msg"; }

	void connect( RPObject* to, UINT message );
	void disconnect( RPObject* to );
	void disconnect( RPObject* to, UINT message );

	// ��������� ��������� � ������ ����� ������� saveToXML � loadFromXML
	// (��-�� ��������� ������ - ����� msg �� ������ ������������� �� RPObject).
	virtual void saveToXML  (REF (pugi::xml_node) parentNode) const;
	virtual void loadFromXML(CREF(pugi::xml_node) node);
};

} // namespace rp

#endif // RDO_PROCESS_MESSAGES_H
