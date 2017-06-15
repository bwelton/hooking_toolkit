#include <stdio.h>
#include "common_defs.h"

extern "C" {
	int orig_func2(void);

	void basic_replacefunction(void) {
		#ifdef VERBOSE
		fprintf(stderr, "Replacement function called\n");
		#endif
		return;
	}

	int basic_wrapperFunction(void) {
		#ifdef VERBOSE
		fprintf(stderr, "Wrapper function called, calling underlying\n");
		#endif
		return orig_func2()+1;
	}

}