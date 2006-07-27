#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

#include <rdocommon.h>
#include <rdokernel.h>
#include <rdothread.h>

#include <rdobinarystream.h>
#include <rdocommon.h>

namespace rdoSimulator {
class RDOThreadSimulator;
}

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
	struct RDOConfig;

// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
class RDOThreadRunTime: public RDOThreadMT
{
friend class rdoSimulator::RDOThreadSimulator;
protected:
	rdoSimulator::RDOThreadSimulator* simulator;
	bool                              runtime_error;

	RDOThreadRunTime();
	virtual ~RDOThreadRunTime() {}; // ����� ������ ���� ������� ����� delete
	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();
	virtual void start();
	virtual void stop();

public:
	struct GetFrame {
		rdoSimulator::RDOFrame* frame;
		int                     frame_number;
		GetFrame( rdoSimulator::RDOFrame* _frame, int _frame_number ): frame( _frame ), frame_number( _frame_number ) {}
	};
};

} // namespace rdoRuntime

class RDOTrace;
namespace rdoParse {
	class RDOParser;
}

namespace rdoSimulator
{

// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
class RDOThreadSimulator: public RDOThreadMT
{
friend class rdoRuntime::RDOThreadRunTime;
friend void frameCallBack( rdoRuntime::RDOConfig* config, void* param );
friend void tracerCallBack( std::string* newString, void* param );

private:
	rdoParse::RDOParser*    parser;
	rdoRuntime::RDORuntime* runtime;
	bool canTrace;

	rdoRuntime::RDOThreadRunTime* thread_runtime;
	rdoModel::RDOExitCode exitCode;

	std::vector<RDOFrame *> frames;
	std::vector<int> scanCodes;
//	bool shiftPressed;
//	bool ctrlPressed;
	std::vector<std::string> areasActivated;

	void terminateModel();
	void closeModel(); 

	ShowMode showMode; // current show mode
	double showRate; // current show mode

	std::stringstream resultString;

protected:
	virtual ~RDOThreadSimulator(); // ����� ������ ���� ������� ����� delete �������� ��� � protected

	virtual void proc( RDOMessageInfo& msg );

	bool parseModel();
	void runModel();
	void stopModel();
	std::vector< RDOSyntaxError >* getErrors();

public:
	RDOThreadSimulator();

	void parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info );

	const std::vector<RDOFrame *>& getFrames();
//	void addKeyPressed(int scanCode);
	void keyDown(int scanCode);
	void keyUp(int scanCode);
	void addAreaPressed(std::string& areaName);

	ShowMode getInitialShowMode();
	int getInitialFrameNumber();
	double getInitialShowRate();

	std::stringstream& getModelStructure();
	std::stringstream& getResults();

	ShowMode getShowMode()                 { return showMode;      }
	void setShowMode( ShowMode _showMode ) { showMode = _showMode; }
	double getShowRate()                   { return showRate;      }
	void setShowRate( double _showRate )   { showRate = _showRate; }

	struct GetList {
		enum Type {
			frames,
			bitmaps 
		} type;
		std::list< std::string >* list;
		GetList( Type _type, std::list< std::string >* _list ): type( _type ), list( _list ) {}
	};
};

} // namespace rdoSimulator

#endif // RDO_SIMULATOR_INTERFACE__
