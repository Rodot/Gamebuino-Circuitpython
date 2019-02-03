#include <string.h>
#include <Gamebuino-Meta.h>

Gamebuino_Meta::Gamebuino* ptr = nullptr;
Gamebuino_Meta::Gamebuino* gb() {
	if (ptr) {
		return ptr;
	}
	ptr = new Gamebuino_Meta::Gamebuino;
	return ptr;
};

extern "C" {

int somevar = 42;

void gambeuino_meta_test() {
	memset(&somevar, 0, sizeof(somevar));
}

void gamebuino_meta_begin() {
	gb()->begin();
}

bool gamebuino_meta_is_inited() {
//	init_gb_pointer();
//	return gbptr->inited;
	return true;
}

} // extern "C"
