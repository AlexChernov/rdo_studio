#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdoparser_object.h"
#include <rdopokaz.h>

namespace rdoParse 
{

int pmdparse( void* lexer );
int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void pmderror( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOParserObject
{
protected:
	rdoRuntime::RDOPMDPokaz* pokaz_runtime;
	void endOfCreation( rdoRuntime::RDOPMDPokaz* _pokaz_runtime );

public:
	RDOPMDPokaz( RDOParser* _parser );
	virtual ~RDOPMDPokaz() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
public:
	RDOPMDWatchPar( RDOParser* _parser, const std::string& _name, bool _trace, const std::string& _resName, const std::string& _parName );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
public:
	RDOPMDWatchState( RDOParser* _parser, const std::string& _name, bool _trace, RDOFUNLogic* _logic );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz
{
public:
	RDOPMDWatchQuant( RDOParser* _parser, const std::string& _name, bool _trace, const std::string& _resTypeName );
	void setLogic( RDOFUNLogic* _logic );
	void setLogicNoCheck();
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz
{
public:
	RDOPMDWatchValue( RDOParser* _parser, const std::string& _name, bool _trace, const std::string& _resTypeName );
	void setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm );
	void setLogicNoCheck( RDOFUNArithm* _arithm );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
public:
	RDOPMDGetValue( RDOParser* _parser, const std::string& _name, RDOFUNArithm* _arithm );
};

} // namespace rdoParse

#endif // RDOPMD_PMD
