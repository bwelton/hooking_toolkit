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

	// Log output to StraceOutpurLocation, similar to printf
	void LogOut(const char * fmt, ...);
	~STrace();
};