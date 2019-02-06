#include <string.h>
#include <Gamebuino-Meta.h>
#include "common-hal/analogio/AnalogOut.h"

extern "C" {
void common_hal_analogio_analogout_construct(analogio_analogout_obj_t* self, const mcu_pin_obj_t *pin);
void common_hal_analogio_analogout_set_value(analogio_analogout_obj_t *self,
        uint16_t value);
}

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
void gamebuino_meta_display_clear_color(const uint16_t c) {
	gb()->display.clear((Gamebuino_Meta::Color)c);
}
void gamebuino_meta_display_fill() {
	gb()->display.fill();
}
void gamebuino_meta_display_fill_color(const uint16_t c) {
	gb()->display.fill((Gamebuino_Meta::Color)c);
}
void gamebuino_meta_display_print_int(const int str) {
	gb()->display.print((int)str);
}
void gamebuino_meta_display_print_int_xy(const int16_t x, const int16_t y, const int str) {
	gb()->display.print(x, y, (int)str);
}
void gamebuino_meta_display_print(const char* str) {
	gb()->display.print(str);
}
void gamebuino_meta_display_print_xy(const int16_t x, const int16_t y, const char* str) {
	gb()->display.print(x, y, str);
}
void gamebuino_meta_display_println_int(const int str) {
	gb()->display.println((int)str);
}
void gamebuino_meta_display_println_int_xy(const int16_t x, const int16_t y, const int str) {
	gb()->display.println(x, y, (int)str);
}
void gamebuino_meta_display_println(const char* str) {
	gb()->display.println(str);
}
void gamebuino_meta_display_println_xy(const int16_t x, const int16_t y, const char* str) {
	gb()->display.println(x, y, str);
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
void gamebuino_meta_display_draw_pixel(int16_t x, int16_t y) {
	gb()->display.drawPixel(x, y);
}
void gamebuino_meta_display_draw_pixel_color(int16_t x, int16_t y, uint16_t c) {
	gb()->display.drawPixel(x, y, (Gamebuino_Meta::Color)c);
}
void gamebuino_meta_display_draw_circle(int16_t x, int16_t y, int16_t r) {
	gb()->display.drawCircle(x, y, r);
}
void gamebuino_meta_display_fill_circle(int16_t x, int16_t y, int16_t r) {
	gb()->display.fillCircle(x, y, r);
}
void gamebuino_meta_display_draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	gb()->display.drawTriangle(x0, y0, x1, y1, x2, y2);
}
void gamebuino_meta_display_fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	gb()->display.fillTriangle(x0, y0, x1, y1, x2, y2);
}
void gamebuino_meta_display_draw_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t r) {
	gb()->display.drawRoundRect(x0, y0, w, h, r);
}
void gamebuino_meta_display_fill_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t r) {
	gb()->display.fillRoundRect(x0, y0, w, h, r);
}
void gamebuino_meta_display_set_color(uint16_t c) {
	gb()->display.setColor((Gamebuino_Meta::Color)c);
}

void gamebuino_meta_lights_clear() {
	gb()->lights.clear();
}
void gamebuino_meta_lights_clear_color(const uint16_t c) {
	gb()->lights.clear((Gamebuino_Meta::Color)c);
}
void gamebuino_meta_lights_fill() {
	gb()->lights.fill();
}
void gamebuino_meta_lights_fill_color(const uint16_t c) {
	gb()->lights.fill((Gamebuino_Meta::Color)c);
}
void gamebuino_meta_lights_draw_pixel(int16_t x, int16_t y) {
	gb()->lights.drawPixel(x, y);
}
void gamebuino_meta_lights_draw_pixel_color(int16_t x, int16_t y, uint16_t c) {
	gb()->lights.drawPixel(x, y, (Gamebuino_Meta::Color)c);
}
void gamebuino_meta_lights_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h) {
	gb()->lights.fillRect(x, y, w, h);
}
void gamebuino_meta_lights_set_color(uint16_t c) {
	gb()->lights.setColor((Gamebuino_Meta::Color)c);
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

analogio_analogout_obj_t sound_pin;
void gamebuino_meta_begin() {
	gb()->begin();
	hri_dac_write_CTRLA_ENABLE_bit(DAC, false);
	common_hal_analogio_analogout_construct(&sound_pin, &pin_PA02);
	common_hal_analogio_analogout_set_value(&sound_pin, 0);
}
bool gamebuino_meta_update() {
	return gb()->update();
}
void gamebuino_meta_wait_for_update() {
	gb()->waitForUpdate();
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
