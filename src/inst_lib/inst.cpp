#include "inst.h"

InstStorage * OpenBinary(char * app_binary) {
	InstStorage * ret = new InstStorage[1];
	BPatch_binaryEdit *app = bpatch.openBinary(app_binary, true);
	if (!app)
		fprintf(stderr, "COULD NOT OPEN BINARY: %s\n", app_binary);
	ret->app = app;
	return ret;
}
int ReplaceFunction(InstStorage * storage, char * binary_function, char * replacement_function, 
					char * replacement_library) {

	assert(storage != NULL);
	if (storage->replaceFuncs.find(binary_function) != storage->replaceFuncs.end()){
		fprintf(stderr, "%s: %s\n", "Replacment function alread exists", binary_function);
		return -1;
	}
	storage->replaceFuncs[binary_function] = std::make_pair(replacement_function, replacement_library);
	return 1;
}

int WrapFunction(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library) {
	assert(storage != NULL);
	if (storage->wrapFunctions.find(binary_function) != storage->wrapFunctions.end()){
		fprintf(stderr, "%s: %s\n", "Replacment function alread exists", replacement_function);
		return -1;
	}
	storage->wrapFunctions[binary_function] = std::make_pair(wrapper_function, wrapper_library);
}
//	int PerformRewrite(char * outputName);