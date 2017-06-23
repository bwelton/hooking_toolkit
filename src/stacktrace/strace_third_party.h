#ifndef STRACE_3RDP_HEADER
#define STRACE_3RDP_HEADER 1
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cassert>
#include <deque>
#include <sys/time.h>
#include <cstdlib>
#include <sstream>
#include <tuple>
#include <utility> 
#include <stdarg.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <condition_variable>
#include "PCProcess.h"
#include <signal.h>


// Dyninst includes
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
#include "BPatch.h"
#include "BPatch_process.h"
#include "BPatch_function.h"
using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;
using namespace std;


// Boost locking
//#include <boost/thread/recursive_mutex.hpp>

enum StraceOutpurLocation {
	STDOUT = 0, 
	STDERR = 1,
	TOFILE = 2
};

class STraceThirdParty {
private:
	// Lock to protect function in case of multi-thread usage;
	//boost::recursive_mutex _mtx; 
	// Generate first party stack trace at the current location
	std::string GenStackTrace();
	StraceOutpurLocation _storage_location; 
	FILE * _fd_out;
	int _pid;
	BPatch bpatch;
	std::map<size_t, std::string> _storedStacks;

public:
	// Construct the stack trace
	STraceThirdParty();
	STraceThirdParty(StraceOutpurLocation out, char * fname);
	void WriteStacks();
	void waitUntilStopped(BPatch *bpatch, BPatch_process *appProc, int proc_id);
	std::vector<std::string> GenStackTrace(BPatch *bpatch, BPatch_process *appProc, int proc_id);
	void AddTrace(std::string i);
	// Flush all output data
	void flush();
	// Log output to StraceOutpurLocation, similar to printf
	void LogOut(const char * fmt, ...);
	~STraceThirdParty();
};

extern std::shared_ptr<STraceThirdParty> StraceThird;
#define BUILD_STORAGE_CLASS_ARGS(ARG1, ARG2) \
	if (StraceThird.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		StraceThird.reset(new STraceThirdParty(ARG1, ARG2)); \
	} 

#define BUILD_STORAGE_CLASS \
	if (StraceThird.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		StraceThird.reset(new STraceThirdParty()); \
	} 

#define STORAGE_PTR StraceThird.get()

#endif
