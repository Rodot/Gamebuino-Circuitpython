#define MICROPY_HW_BOARD_NAME "Metro M4 Express"
#define MICROPY_HW_MCU_NAME "samd51j19"

#define CIRCUITPY_MCU_FAMILY samd51

// This is for Rev D which is light blue

#define MICROPY_HW_LED_TX   PIN_PA27
#define MICROPY_HW_LED_RX   PIN_PB06

#define MICROPY_HW_NEOPIXEL (&pin_PB17)

// These are pins not to reset.
// QSPI Data pins and TX LED
#define MICROPY_PORT_A (PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA11 | PORT_PA27)
// RX LED, QSPI CS, QSPI SCK and NeoPixel pin
#define MICROPY_PORT_B ( PORT_PB06 | PORT_PB10 | PORT_PB11 | PORT_PB17)
#define MICROPY_PORT_C (0)
#define MICROPY_PORT_D (0)

#define AUTORESET_DELAY_MS 500

#include "external_flash/external_flash.h"

// If you change this, then make sure to update the linker scripts as well to
// make sure you don't overwrite code
// #define CIRCUITPY_INTERNAL_NVM_SIZE 256
#define CIRCUITPY_INTERNAL_NVM_SIZE 0

#define BOARD_FLASH_SIZE (FLASH_SIZE - 0x4000 - CIRCUITPY_INTERNAL_NVM_SIZE)

#include "external_flash/devices/S25FL116K.h"
#include "external_flash/devices/GD25Q16C.h"