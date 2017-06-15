#include <stdio.h>
#include <stdlib.h>
#include "common_defs.h"

void TestingFunction1(void) {
	fprintf(stderr,"we shouldn't be here if hooked correctly\n");
}

int TestingFunction2(void) {
	// Return a known value that can be checked by 
	return EXPECTED_RETURN;
}

int main(int argc, char * argv[]){
	TestingFunction1();
	if (TestingFunction2() != EXPECTED_RETURN + 1) {
		fprintf(stderr, "%s\n", "Wrapper function not called");
		exit(-5);
	}
	exit(0);
}