#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdoparsebase.h"
#include <rdotrace.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOResource;
}

namespace rdoParse 
{

int pmdparse( void* lexer );
int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void pmderror( char* mes );

class RDOFUNLogic;
class RDOFUNGroup;
class RDOFUNArithm;

//////////////////////////// class RDOPMDPokaz /////////////////////////////////
class RDOPMDPokaz: public RDOPokazTrace, public RDOParserObject
{
protected:
	const std::string const name;
	void endOfCreation();

public:
	RDOPMDPokaz( RDOParser* _parser, const std::string* const _name, bool _trace );
	virtual ~RDOPMDPokaz() {}
	virtual bool checkResourceErased(rdoRuntime::RDOResource *res) { return false; }
	virtual void writePokazStructure(std::ostream &stream) const = 0;
};

//////////////////////////// class RDOPMDWatchPar /////////////////////////////////
class RDOPMDWatchPar: public RDOPMDPokaz
{
private:
	int resNumber;
	int parNumber;

	int watchNumber;
	double currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchPar( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resName, std::string* _parName );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

//////////////////////////// class RDOPMDWatchState /////////////////////////////////
class RDOPMDWatchState: public RDOPMDPokaz
{
private:
	rdoRuntime::RDOCalc *logicCalc;

	int watchNumber;
	bool currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchState( RDOParser* _parser, std::string* _name, bool _trace, RDOFUNLogic* _logic );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

//////////////////////////// class RDOPMDWatchQuant /////////////////////////////////
class RDOPMDWatchQuant: public RDOPMDPokaz
{
private:
	RDOFUNGroup *funGroup;
	rdoRuntime::RDOCalc *logicCalc;

	int watchNumber;
	int currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

	double timeBegin;
	double timePrev;

public:
	RDOPMDWatchQuant( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName );
	void setLogic(RDOFUNLogic *_logic);
	void setLogicNoCheck();
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

//////////////////////////// class RDOPMDWatchValue /////////////////////////////////
class RDOPMDWatchValue: public RDOPMDPokaz
{
private:
	RDOFUNGroup *funGroup;
	rdoRuntime::RDOCalc *logicCalc;
	rdoRuntime::RDOCalc *arithmCalc;

	int watchNumber;
	double currValue;
	double sum;
	double sumSqr;
	double minValue;
	double maxValue;

public:
	RDOPMDWatchValue( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName );
	void setLogic(RDOFUNLogic *_logic, RDOFUNArithm *_arithm);
	void setLogicNoCheck(RDOFUNArithm *_arithm);
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	bool checkResourceErased(rdoRuntime::RDOResource *res);
	void writePokazStructure(std::ostream &stream) const;
};

//////////////////////////// class RDOPMDGetValue /////////////////////////////////
class RDOPMDGetValue: public RDOPMDPokaz
{
private:
	rdoRuntime::RDOCalc *arithmCalc;

public:
	RDOPMDGetValue( RDOParser* _parser, std::string* _name, RDOFUNArithm* _arithm );
	std::string traceValue();
	bool resetPokaz(RDOSimulator *sim);
	bool checkPokaz(RDOSimulator *sim);
	bool calcStat(RDOSimulator *sim);
	void writePokazStructure(std::ostream &stream) const;
};

} // namespace rdoParse

#endif // RDOPMD_PMD
