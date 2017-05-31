#include "strace.h"

// Any function call in an application matching the below will be replaced to one pointing to
// libstrace.so
extern "C" {
	void STRACE_INIT(StraceOutpurLocation location, char * fname) {
		BUILD_STORAGE_CLASS_ARGS(location, fname)
	}

	void STRACE_RECORD_STACKTRACE() {
		// We may be the first call, always check if the storage structure 
		// has been created.
		BUILD_STORAGE_CLASS

		STORAGE_PTR->WriteMyStack();
	}

	void STRACE_FLUSH() {
		BUILD_STORAGE_CLASS

		STORAGE_PTR->flush();
	}
}