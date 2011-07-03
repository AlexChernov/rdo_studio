/*
 * copyright: (c) RDO-Team, 2011
 * filename : sequence.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_SEQUENCE_H_
#define _RDOCALC_SEQUENCE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
CALC(RDOCalcSeqInit)
{
DECLARE_FACTORY(RDOCalcSeqInit)
public:
	void setBase(int base)
	{
		m_base = base;
	}

private:
	RDOCalcSeqInit(int base, PTR(RandGenerator) gen)
		: m_base(base)
		, m_gen (gen )
	{}
	virtual ~RDOCalcSeqInit();

	int                m_base;
	PTR(RandGenerator) m_gen;

	DECALRE_ICalc;
};

CALC_SUB(RDOCalcSeqNext, RDOFunCalc)
{
public:
	rbool   m_res_real;
	rbool   m_diap;
	double  m_diap_min;
	double  m_diap_max;

protected:
	RDOCalcSeqNext()
		: m_res_real(true  )
		, m_diap    (false )
		, m_diap_min(0     )
		, m_diap_max(0     )
	{}

	virtual RDOValue getNextValue(PTR(RDORuntime) runtime) = 0;
};

template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
protected:
	RDOCalcRandomDistribution(PTR(T) gen)
		: m_gen(gen)
	{}
	virtual ~RDOCalcRandomDistribution()
	{
		delete m_gen;
	}
	PTR(T) m_gen;

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		RDOValue res = getNextValue(runtime);
		if (m_diap)
		{
			if (res < m_diap_min)
			{
				m_value = m_res_real ? m_diap_min : RDOValue(m_diap_min > 0 ? m_diap_min + 0.5 : m_diap_min - 0.5).getInt();
				return m_value;
			}
			if (res > m_diap_max)
			{
				m_value = m_res_real ? m_diap_max : RDOValue(m_diap_max > 0 ? m_diap_max + 0.5 : m_diap_max - 0.5).getInt();
				return m_value;
			}
			m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
			return m_value;
			// � ����� ��� ���� ������� ������� �������� ����� ��������� �����, ���� ������� ����� �� ��������. �� ��� ���� ��������� ������� (��� � � ������ ������ ����� ��������� imho). ��� ������������� ������� ������ �������.
//			for ( int i = 0; i < 1000; i++ ) {
//				if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//				res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
//			}
//			runtime->error( "�� ������� �������� ��������, ���������� � ����������� ��������", this );
//			return res_real ? diap_min : static_cast<int>(diap_min);
		}
		else
		{
			m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
			return m_value;
		}
	}
};

#define DEFINE_RANDON_DISTRIBUTION(CalcName, Distribution) \
CALC_SUB(RDOCalcSeqNext##CalcName, RDOCalcRandomDistribution<Distribution>) \
{ \
DECLARE_FACTORY(RDOCalcSeqNext##CalcName) \
private: \
	RDOCalcSeqNext##CalcName(PTR(Distribution) gen) \
		: RDOCalcRandomDistribution<Distribution>(gen) \
	{} \
 \
	virtual RDOValue getNextValue(PTR(RDORuntime) runtime); \
};

DEFINE_RANDON_DISTRIBUTION( Uniform    , RandGeneratorUniform     );
DEFINE_RANDON_DISTRIBUTION( Normal     , RandGeneratorNormal      );
DEFINE_RANDON_DISTRIBUTION( Exponential, RandGeneratorExponential );
DEFINE_RANDON_DISTRIBUTION( Triangular , RandGeneratorTriangular  );
DEFINE_RANDON_DISTRIBUTION( ByHist     , RandGeneratorCommonNext  );

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_SEQUENCE_H_
