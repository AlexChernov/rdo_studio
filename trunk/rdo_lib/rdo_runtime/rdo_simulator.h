#ifndef RDO_SIMULATOR_H
#define RDO_SIMULATOR_H

#include "rdo.h"
#include "rdo_logic_opr.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - ���� �� ������� ������� ��� RDORuntime
// ----------------------------------------------------------------------------
class RDOSimulator: public RDOSimulatorBase
{
friend class RDOTrace;

public:
	RDOSimulator():
		RDOSimulatorBase(),
		m_logics( NULL ),
		opr_must_continue( NULL ),
		m_sizeof_sim( 0 )
	{
		m_logics.reparent( this );
	}
	virtual ~RDOSimulator()
	{
		RDOOperations::destroy( this );
	}

	void addBaseLogic( RDOBaseOperation* logic )
	{
		m_logics.append( logic );
	}

	RDOBaseOperation* getMustContinueOpr() const       { return opr_must_continue;  }
	void setMustContinueOpr( RDOBaseOperation* value ) { opr_must_continue = value; }

	virtual void onPutToTreeNode() = 0;

	std::string writeActivitiesStructure( int& counter );

	RDOSimulator* createCopy();
	// ��� DPT ���������� ��������� ��� ������������� �������:
	// 1. ������� ���� RDOSimulator � ������� ���� ��������, �� �� �����
	virtual RDOSimulator* clone()                    = 0;
	// 2. ��������� ���� ����������� �� ��������
	virtual bool operator == ( RDOSimulator& other ) = 0;

	unsigned int getSizeofSim() const
	{
		return m_sizeof_sim;
	}

protected:
	virtual void addTemplateBaseOperation( RDOBaseOperation* op )
	{
		RDOOperations::getInstance( this )->append( op );
	}

	// �������������� ������������ ������� � ����� GENERATE: ������ ����� ������� ������������
	virtual void preProcess();

	virtual void onResetPokaz()      = 0;
	virtual void onCheckPokaz()      = 0;
	virtual void onAfterCheckPokaz() = 0;

	unsigned int m_sizeof_sim;

private:
	RDOBaseLogic m_logics;

	RDOBaseOperation* opr_must_continue;
	virtual bool doOperation();
};

} // namespace rdoRuntime

#endif // RDO_SIMULATOR_H
