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

void gamebuino_meta_reset() {
	ptr = nullptr;
}

void gamebuino_meta_display_clear() {
	gb()->display.clear();
}
void gamebuino_meta_display_print_int(const int str) {
	gb()->display.print((int)str);
}
void gamebuino_meta_display_print(const char* str) {
	gb()->display.print(str);
}
void gamebuino_meta_display_println_int(const int str) {
	gb()->display.println((int)str);
}
void gamebuino_meta_display_println(const char* str) {
	gb()->display.println(str);
}
void gamebuino_meta_display_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	gb()->display.drawLine(x0, y0, x1, y1);
}
void gamebuino_meta_display_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h) {
	gb()->display.drawRect(x, y, w, h);
}
void gamebuino_meta_display_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h) {
	gb()->display.fillRect(x, y, w, h);
}
void gamebuino_meta_display_set_color(uint16_t c) {
	gb()->display.setColor((Gamebuino_Meta::Color)c);
}

const uint8_t gbm_BUTTON_DOWN = (const uint8_t)BUTTON_DOWN;
const uint8_t gbm_BUTTON_LEFT = (const uint8_t)BUTTON_LEFT;
const uint8_t gbm_BUTTON_RIGHT = (const uint8_t)BUTTON_RIGHT;
const uint8_t gbm_BUTTON_UP = (const uint8_t)BUTTON_UP;
const uint8_t gbm_BUTTON_A = (const uint8_t)BUTTON_A;
const uint8_t gbm_BUTTON_B = (const uint8_t)BUTTON_B;
const uint8_t gbm_BUTTON_MENU = (const uint8_t)BUTTON_MENU;
const uint8_t gbm_BUTTON_HOME = (const uint8_t)BUTTON_HOME;
bool gamebuino_meta_buttons_pressed(uint8_t b) {
	return gb()->buttons.pressed((Gamebuino_Meta::Button)b);
}
bool gamebuino_meta_buttons_released(uint8_t b) {
	return gb()->buttons.released((Gamebuino_Meta::Button)b);
}
bool gamebuino_meta_buttons_held(uint8_t b, uint16_t time) {
	return gb()->buttons.held((Gamebuino_Meta::Button)b, time);
}
bool gamebuino_meta_buttons_repeat(uint8_t b, uint16_t time) {
	return gb()->buttons.repeat((Gamebuino_Meta::Button)b, time);
}
uint16_t gamebuino_meta_buttons_time_held(uint8_t b) {
	return gb()->buttons.timeHeld((Gamebuino_Meta::Button)b);
}

void gamebuino_meta_begin() {
	gb()->begin();
}
bool gamebuino_meta_update() {
	return gb()->update();
}
void gamebuino_meta_update_display() {
	return gb()->updateDisplay();
}
void gamebuino_meta_set_frame_rate(uint8_t fps) {
	gb()->setFrameRate(fps);
}
uint8_t gamebuino_meta_get_cpu_load(){
	return gb()->getCpuLoad();
}
void gamebuino_meta_titlescreen() {
	if (PM->RCAUSE.bit.POR) {
		gb()->startScreen();
		gb()->titleScreen();
	}
}

} // extern "C"
