#define MICROPY_HW_BOARD_NAME "Escornabot Makech"
#define MICROPY_HW_MCU_NAME "samd21g18"

#define MICROPY_HW_LED_STATUS   (&pin_PA02)

// These are pins not to reset.
#define MICROPY_PORT_A        (0)
#define MICROPY_PORT_B        (0)
#define MICROPY_PORT_C        (0)

// If you change this, then make sure to update the linker scripts as well to
// make sure you don't overwrite code.
#define CIRCUITPY_INTERNAL_NVM_SIZE 256

#define BOARD_FLASH_SIZE (0x00040000 - 0x2000 - 0x010000 - CIRCUITPY_INTERNAL_NVM_SIZE)

#define CALIBRATE_CRYSTALLESS 1

// Explanation of how a user got into safe mode.
#define BOARD_USER_SAFE_MODE_ACTION "pressing both buttons at start up"

#define DEFAULT_I2C_BUS_SCL (&pin_PA08)
#define DEFAULT_I2C_BUS_SDA (&pin_PA09)

#define DEFAULT_SPI_BUS_SCK (&pin_PA14)
#define DEFAULT_SPI_BUS_MOSI (&pin_PA19)
#define DEFAULT_SPI_BUS_MISO (&pin_PA20)

#define DEFAULT_UART_BUS_RX (&pin_PA01)
#define DEFAULT_UART_BUS_TX (&pin_PA00)

// USB is always used internally so skip the pin objects for it.
#define IGNORE_PIN_PA24     1
#define IGNORE_PIN_PA25     1
