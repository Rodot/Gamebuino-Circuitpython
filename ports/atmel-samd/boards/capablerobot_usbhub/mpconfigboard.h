#define MICROPY_HW_BOARD_NAME "Capable Robot Programmable USB Hub"
#define MICROPY_HW_MCU_NAME "samd51g19"

#define CIRCUITPY_MCU_FAMILY samd51


#define MICROPY_HW_LED_STATUS   (&pin_PA22)

// These are pins not to reset.
// QSPI Data pins
#define MICROPY_PORT_A (PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA11)
// QSPI CS, and QSPI SCK
#define MICROPY_PORT_B (PORT_PB10 | PORT_PB11)
#define MICROPY_PORT_C (0)
#define MICROPY_PORT_D (0)

#define AUTORESET_DELAY_MS 500

// If you change this, then make sure to update the linker scripts as well to
// make sure you don't overwrite code
#define CIRCUITPY_INTERNAL_NVM_SIZE 8192

#define BOARD_FLASH_SIZE (FLASH_SIZE - 0x4000 - CIRCUITPY_INTERNAL_NVM_SIZE)

#define DEFAULT_I2C_BUS_SCL (&pin_PA23)
#define DEFAULT_I2C_BUS_SDA (&pin_PA22)

#define DEFAULT_SPI_BUS_SCK (&pin_PA01)
#define DEFAULT_SPI_BUS_MOSI (&pin_PA00)
#define DEFAULT_SPI_BUS_MISO (&pin_PB23)

#define DEFAULT_UART_BUS_RX (&pin_PA16)
#define DEFAULT_UART_BUS_TX (&pin_PA17)

// USB is always used internally so skip the pin objects for it.
#define IGNORE_PIN_PA24     1
#define IGNORE_PIN_PA25     1
