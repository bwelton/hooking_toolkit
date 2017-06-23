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
#include <signal.h>


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

public:
	// Construct the stack trace
	STraceThirdParty();
};

extern std::shared_ptr<STraceThirdParty> StraceThird;

#define BUILD_STORAGE_CLASS \
	if (StraceThird.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		StraceThird.reset(new STraceThirdParty()); \
	} 

#define STORAGE_PTR StraceThird.get()

#endif
