#ifndef RDO_LOGIC_DPTSOME_H
#define RDO_LOGIC_DPTSOME_H

#include "rdo_logic.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogic
{
public:
	RDODPTSome( RDOSimulator* sim );
	virtual ~RDODPTSome();

protected:
	// ��� ����������� ���� ��������� (����������) ���� ����������� ����� �����.
	// ������ ���������� ����������� ���������� RDOLogic::onCheckCondition,
	// ������� ������� ������ ��������� ��������.
	// virtual bool onCheckCondition( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSOME_H
