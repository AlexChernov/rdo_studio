#ifndef RDOSMR_H
#define RDOSMR_H

#include "rdo_common/rdocommon.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse 
{

int  smr_file_parse(PTR(void) lexer);
int  smr_file_lex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void smr_file_error(PTR(char) mes);

int  smr_sim_parse (PTR(void) lexer);
int  smr_sim_lex   (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void smr_sim_error (PTR(char) mes);

class RDOFUNLogic;
class RDOFUNArithm;

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
class RDOFUNConstant;
class RDOFUNSequence;

class RDOSMR: public RDOParserObject
{
public:
	typedef std::map<std::string, std::string> StringTable;

	RDOSMR( RDOParser* _parser, const std::string& _modelName );

	void setFile( const std::string& file_type, const std::string& file_name )
	{
		files[ file_type ] = file_name;
	}
	bool hasFile( const std::string& file_type ) const
	{
		return files.find( file_type ) != files.end();
	}
	std::string getFile( const std::string& file_type ) const
	{
		StringTable::const_iterator it = files.find(file_type);
		return it != files.end() ? it->second : _T("");
	}
	std::string modelName() const
	{
		return getFile(_T("Model_name"));
	}
	void setExternalModelName(const std::string& alias, const std::string& modelID )
	{
		m_extModelList[alias] = modelID;
	}
	std::string getExternalModelName( const std::string& alias ) const
	{
		StringTable::const_iterator it = m_extModelList.find(alias);
		return it != m_extModelList.end() ? it->second : "";
	}
	const StringTable& getExternalModelList() const
	{
		return m_extModelList;
	}

	rdoSimulator::ShowMode getShowMode() const { return showMode;       }
	int    getFrameNumber() const              { return frameNumber;    }
	double getShowRate() const                 { return showRate;       }
	double getRunStartTime() const             { return runStartTime;   }
	double getTraceStartTime() const           { return traceStartTime; }
	double getTraceEndTime() const             { return traceEndTime;   }

	void setShowMode( rdoSimulator::ShowMode _showMode );
	void setFrameNumber( int value, const YYLTYPE& pos );
	void setShowRate( double value, const YYLTYPE& pos );
	void setRunStartTime( double value, const YYLTYPE& pos );
	void setTraceStartTime( double value, const YYLTYPE& pos );
	void setTraceEndTime( double value, const YYLTYPE& pos );

	void setTerminateIf( RDOFUNLogic* logic );
	void setConstValue( const RDOParserSrcInfo& const_info, RDOFUNArithm* arithm );
	void setResParValue( const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, RDOFUNArithm* arithm );
	void setSeed( const RDOParserSrcInfo& seq_info, int base );
	void insertBreakPoint( const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic );

private:
	StringTable  files;
	StringTable  m_extModelList;
	rdoSimulator::ShowMode showMode;
	int    frameNumber;
	double showRate;
	double runStartTime;
	double traceStartTime;
	double traceEndTime;
	YYLTYPE traceStartTime_pos;
	YYLTYPE traceEndTime_pos;
	RDOFUNLogic* terminateIf;

	class BreakPoint: public RDOParserObject, public RDOParserSrcInfo {
	public:
		BreakPoint( RDOSMR* smr, const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic );
	};
	std::vector< BreakPoint* > breakPoints;
};

} // namespace rdoParse

#endif // RDOSMR_H
