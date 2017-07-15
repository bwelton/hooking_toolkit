#include "deduplicator.h"
extern "C" {
void TRACK_COPIES(int identifier, int64_t transfer) {
	BUILD_STORAGE_CLASS

	STORAGE_PTR->TrackTransfer(identifier, transfer);
}
void TRACK_COPIES_DATA(int identifier, int64_t transfer, char * data) {
	BUILD_STORAGE_CLASS

	STORAGE_PTR->TrackTransfer(identifier, transfer, data);
}
void LIBRARY_HOOK_LOCATION() {

}

}
