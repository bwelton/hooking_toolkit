#include <stdio.h>
#include <stdlib.h>

extern "C" {

void LIBRARY_HOOK_LOCATION() {
    //fprintf(stderr, "I SHOULD NOT BE HERE\n");
}
void TRACK_COPIES(int identifier, int64 transfer) {

}


}
