#include <stdio.h>

void TestingFunction1(void) {
	fprintf(stderr,"we shouldn't be here if hooked correctly\n");
}

int main(int argc, char * argv[]){
	TestingFunction1();
}