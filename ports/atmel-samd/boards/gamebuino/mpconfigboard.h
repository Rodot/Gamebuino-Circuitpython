#define MICROPY_HW_BOARD_NAME "Gamebuino"
#define MICROPY_HW_MCU_NAME "samd21g18"

//#define MICROPY_HW_LED_STATUS   (&pin_PA17)

//#define MICROPY_HW_LED_TX   &pin_PA27
//#define MICROPY_HW_LED_RX   &pin_PB03

#define MICROPY_PORT_A        (0)
#define MICROPY_PORT_B        (0)
#define MICROPY_PORT_C        (0)

#define CIRCUITPY_INTERNAL_NVM_SIZE 0

#define OVERRIDE_BOARD_SMALL 1

#define TOTAL_INTERNAL_FLASH_SIZE 0x8000
#define BOARD_FLASH_SIZE (0x00040000 - 0x4000 - TOTAL_INTERNAL_FLASH_SIZE)

#define DEFAULT_I2C_BUS_SCL (&pin_PA23)
#define DEFAULT_I2C_BUS_SDA (&pin_PA22)

#define DEFAULT_SPI_BUS_SCK (&pin_PB11)
#define DEFAULT_SPI_BUS_MOSI (&pin_PB10)
#define DEFAULT_SPI_BUS_MISO (&pin_PA12)

//#define DEFAULT_UART_BUS_RX (&pin_PA11)
//#define DEFAULT_UART_BUS_TX (&pin_PA10)

// USB is always used internally so skip the pin objects for it.
#define IGNORE_PIN_PA24     1
#define IGNORE_PIN_PA25     1

// also skip internal CS pins
#define IGNORE_PIN_PA39     1
#define IGNORE_PIN_PB03     1
#define IGNORE_PIN_PB22     1
#define IGNORE_PIN_PB23     1
#define IGNORE_PIN_PB27     1

// skip audio and neopixels pin
#define IGNORE_PIN_PA13     1
//#define IGNORE_PIN_PA02     1

// #define TC5_IN_USE

extern const struct _mp_obj_module_t mp_module_gamebuino_meta;

#define EXTRA_BUILTIN_MODULES { MP_OBJ_NEW_QSTR(MP_QSTR_gamebuino_meta), (mp_obj_t)&mp_module_gamebuino_meta },

#define NO_HARDFAULT_HANDLER

#define BOARD_ROOT_POINTERS \
	void* gbptr;
