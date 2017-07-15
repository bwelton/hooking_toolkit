#ifndef DEDUPLICATOR
#define DEDUPLICATOR 1
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

#define USE_PRIVATE 1

#ifdef USE_PRIVATE
// Cannot include this header at this time... 
#include "xxhash32.h"
#endif

#define TIMEOUT 10 //Timeout data after 10 successful transfers have taken place.
#define MAX_DATA_SIZE_BYTES 10485760 // Maximum size of data that can be temporarily stored


#include <boost/thread/recursive_mutex.hpp>

enum OutLocation {
	STDOUT = 0, 
	STDERR = 1,
	TOFILE = 2
};

class Deduplicate{
public:
	Deduplicate();
	~Deduplicate();
	void TrackTransfer(int id, int64_t size);
	void TrackTransfer(int id, int64_t size, char * data);
	uint32_t HashData(char * data, size_t size);
private:
	boost::recursive_mutex _mtx; 
	size_t _collisionCount;
	int64_t _collisionSize;
        int64_t _totalSize;
	size_t _totalCount;
	std::map<int, int64_t> _previousTransfers; 
	std::map<int, void *> _storedTransfers;
	uint64_t _missedHashes;
	int64_t _missedSize;
	std::map<uint32_t, int64_t> _previousHash;

};

extern std::shared_ptr<Deduplicate> Dedup;
#define BUILD_STORAGE_CLASS \
	if (Dedup.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		Dedup.reset(new Deduplicate()); \
	} 

#define STORAGE_PTR Dedup.get()
#endif
