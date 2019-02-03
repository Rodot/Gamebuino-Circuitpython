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
	
void gamebuino_meta_begin() {
	gb()->begin();
}

bool gamebuino_meta_update() {
	return gb()->update();
}

void gamebuino_meta_display_clear() {
	gb()->display.clear();
}

void gamebuino_meta_display_println(const char* str) {
	gb()->display.println(str);
}

} // extern "C"
