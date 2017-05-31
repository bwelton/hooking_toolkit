#ifndef STRACE_HEADER
#define STRACE_HEADER 1
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

// Dyninst includes
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;

// Boost locking
#include <boost/thread/recursive_mutex.hpp>

enum StraceOutpurLocation {
	STDOUT = 0, 
	STDERR = 1,
	TOFILE = 2
};

class STrace {
private:
	// Lock to protect function in case of multi-thread usage;
	boost::recursive_mutex _mtx; 
	// Generate first party stack trace at the current location
	std::string GenStackTrace();
	StraceOutpurLocation _storage_location; 
	FILE * _fd_out;
public:
	// Construct the stack trace
	STrace(StraceOutpurLocation out, char * fname);
	// Flush all output data
	void flush();
	// Write the stack at the current location
	void WriteMyStack();
	// Log output to StraceOutpurLocation, similar to printf
	void LogOut(const char * fmt, ...);
	~STrace();
};

extern std::shared_ptr<STrace> StraceStore;
#define BUILD_STORAGE_CLASS_ARGS(ARG1, ARG2) \
	if (StraceStore.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		StraceStore.reset(new STrace(ARG1, ARG2)); \
	} 

#define BUILD_STORAGE_CLASS BUILD_STORAGE_CLASS_ARGS(STDERR, NULL);

#define STORAGE_PTR StraceStore.get()

#endif