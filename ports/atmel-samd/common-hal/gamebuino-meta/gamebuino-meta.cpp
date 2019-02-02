#include <string.h>

extern "C" {

int somevar = 42;

void gambeuino_meta_test() {
	memset(&somevar, 0, sizeof(somevar));
}

} // extern "C"
