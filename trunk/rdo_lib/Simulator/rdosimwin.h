#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

#include "rdosimcommon.h"

using namespace std;

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
	struct RDOConfig;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

namespace RDOSimulatorNS
{

class RdoSimulator
{
	rdoParse::RDOParser *parser;
	rdoRuntime::RDORuntime *runtime;

	CWinThread* th;

	RDOFrame *frame;
	vector<int> scanCodes;
	vector<string> areasActivated;

	void terminateModel();
	void closeModel(); 
public:
	RdoSimulator();
	~RdoSimulator();
	bool parseModel();
	void runModel();
	void stopModel();
	vector<RDOSyntaxError>* getErrors();
	double getModelTime();
	void parseSMRFileInfo( stringstream& smr, rdoModelObjects::RDOSMRFileInfo& info );

	const RDOFrame* getFrame();
	void addKeyPressed(int scanCode);
	void addAreaPressed(string& areaName);

	vector<const string *> getAllFrames();
	
	friend UINT RunningThreadControllingFunction( LPVOID pParam );
	friend void frameCallBack(rdoRuntime::RDOConfig *config, void *param);
	friend void tracerCallBack(string *newString, void *param);
};

} // namespace RDOSimulatorNS

#endif //RDO_SIMULATOR_INTERFACE__
