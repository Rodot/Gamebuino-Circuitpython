LD_FILE = boards/samd51x19-bootloader-external-flash.ld
USB_VID = 0x239A
USB_PID = 0x8043
USB_PRODUCT = "PyBadge AirLift"
USB_MANUFACTURER = "Adafruit Industries LLC"

CHIP_VARIANT = SAMD51J20A
CHIP_FAMILY = samd51

QSPI_FLASH_FILESYSTEM = 1
EXTERNAL_FLASH_DEVICE_COUNT = 1
EXTERNAL_FLASH_DEVICES = GD25Q16C
LONGINT_IMPL = MPZ

CIRCUITPY_AUDIOIO = 1
CIRCUITPY_DISPLAYIO = 1
CIRCUITPY_GAMEPAD = 1
CIRCUITPY_GAMEPADSHIFT = 1
CIRCUITPY_STAGE = 1
# No touch on SAMD51 yet
CIRCUITPY_TOUCHIO = 0

FROZEN_MPY_DIRS += $(TOP)/frozen/circuitpython-stage/pybadge
