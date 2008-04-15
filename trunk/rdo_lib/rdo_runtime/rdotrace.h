#ifndef RDOTRACE_H
#define RDOTRACE_H

#include "rdo.h"

#include <fstream>

namespace rdoRuntime {

class RDOSimulator;
class RDOSimulatorTrace;
class TreeNodeTrace;
class RDORuleTrace;
class RDOIETrace;
class RDOOperationTrace;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;
class TreeNode;
class TreeRoot;

// ----------------------------------------------------------------------------
// ---------- RDOEndL - ��������� ����������� ������ �����������
// ----------------------------------------------------------------------------
class RDOEndL
{
public:
	virtual void onEndl() {}
};

inline std::ostream &operator << (std::ostream &stream, RDOEndL& rdoEndL)
{
	rdoEndL.onEndl();
	return stream;
}

// ----------------------------------------------------------------------------
// ---------- RDOTrace - ��������� ������ �����������
// ----------------------------------------------------------------------------
class RDOTrace
{
friend RDOSimulatorTrace;
friend RDOResource;

public:
	RDOTrace(): m_isNullTracer( true ), m_canWriteToStream( false ) {}
	virtual ~RDOTrace() {}

	bool canTrace() const { return !isNull() && canWrite();   }

	void startWriting()   { m_canWriteToStream = true;        }
	void stopWriting()    { m_canWriteToStream = false;       }
	bool canWrite() const { return m_canWriteToStream;        }
	bool isNull() const   { return m_isNullTracer;            }

	// Search in tree
	virtual void writeSearchBegin(double currentTime, std::string decisionPointId);
	virtual void writeSearchDecisionHeader();
	virtual void writeSearchDecision(RDOSimulator *sim, TreeNode *node);
	virtual void writeString(std::string);
	virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
	virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
	virtual void writeSearchResult(char letter, RDOSimulatorTrace *simTr, TreeRoot *treeRoot);

	virtual void writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim);
	virtual void writeRule(RDORuleTrace *rule, RDOSimulatorTrace *sim);
	virtual void writeAfterOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim);
	virtual void writeAfterOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim);

	virtual void writeTraceBegin(RDOSimulatorTrace *sim);
	virtual void writeModelBegin(RDOSimulatorTrace *sim);
	virtual void writeTraceEnd(RDOSimulatorTrace *sim);
	virtual void writeStatus(RDOSimulatorTrace *sim, char *status);

	virtual void writePermanentResources( RDOSimulatorTrace* sim, const std::list< RDOResource* >& res_perm );

	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim, const std::list< RDOResource* >& rel_res_list );

	virtual void writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok);

public:
	virtual std::ostream& getOStream() { return m_emptyOut; }
	virtual RDOEndL&      getEOL()     { return m_emptyEndL;}

protected:
	bool          m_isNullTracer;

private:
	bool          m_canWriteToStream;
	std::ofstream m_emptyOut;
	RDOEndL       m_emptyEndL;
};

// ----------------------------------------------------------------------------
// ---------- RDOTraceableObject
// ----------------------------------------------------------------------------
class RDOTraceableObject
{
public:
	enum { NONE = 0xFFFFFFFF };

	bool traceable() const                { return m_trace;  }
	void setTrace( bool trace )           { m_trace = trace; }

	unsigned int getTraceID() const       { return m_id;     }
	void setTraceID( unsigned int id )
	{
		setTraceID( id, id );
	}
	void setTraceID( unsigned int id, unsigned int str_id )
	{
		m_id     = id;
		m_str_id = toString( str_id );
	}

	std::string& traceId() const
	{
		if ( m_str_id.empty() ) {
			m_str_id = toString( m_id );
		}
		return m_str_id;
	}

protected:
	RDOTraceableObject( bool trace ):
		m_trace( trace ),
		m_id( NONE ),
		m_str_id( "" )
	{
	}
	virtual ~RDOTraceableObject()
	{
	}

private:
	bool                m_trace;
	unsigned int        m_id;
	mutable std::string m_str_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternTrace
// ----------------------------------------------------------------------------
class RDOPatternTrace: public RDOTraceableObject
{
friend class RDOSimulatorTrace;
protected:
	RDOPatternTrace( bool trace ): RDOTraceableObject( trace ) {};

	virtual std::string traceResourcesList       ( char prefix, RDOSimulatorTrace* sim                   ) = 0;
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOPokazTrace
// ----------------------------------------------------------------------------
class RDOPokazTrace: public RDOPokaz, public RDOTraceableObject, public RDORuntimeContainer
{
public:
	RDOPokazTrace( RDORuntime* runtime, bool trace );

	bool tracePokaz();
	virtual std::string traceValue() = 0;

protected:
	bool m_wasChanged;
};

} // namespace rdoRuntime

#endif // RDOTRACE_H
