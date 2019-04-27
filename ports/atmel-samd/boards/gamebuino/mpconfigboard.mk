LD_FILE = boards/samd21x18-bootloader-16k.ld
USB_VID = 0x239A
USB_PID = 0x8015
USB_PRODUCT = "Arduino Zero"
USB_MANUFACTURER = "Arduino"

INTERNAL_FLASH_FILESYSTEM = 1
LONGINT_IMPL = NONE
CIRCUITPY_SMALL_BUILD = 1

CHIP_VARIANT = SAMD21G18A
CHIP_FAMILY = samd21

EXCLUDE_PIXELBUF = 1

SRC_C_EXTRA = \
	common-hal/gamebuino-meta/__init__.c

SRC_CPP_EXTRA = \
	common-hal/gamebuino-meta/gamebuino-meta.cpp \
	common-hal/gamebuino-meta/Print-Compat.cpp

INC_EXTRA = \
	-Icommon-hal/gamebuino-meta

DEFINES_EXTRA = \
	-DARDUINO=10808 \
	-D__SKETCH_NAME__ \
	-DF_CPU=48000000L \
	-DARDUINO_SAMD_ZERO \
	-DDARDUINO_ARCH_SAMD \
	-DUSB_VID=$(USB_VID) \
	-DUSB_PID=$(USB_PID) \
	-DUSBCON \
	"-DUSB_MANUFACTURER=\"Arduino LLC\"" \
	"-DUSB_PRODUCT=\"Arduino Zero\"" \
	-DNO_ARDUINO_ID \

BOOTLOADER_SIZE := 0x4000

$(shell mkdir -p $(BUILD))
$(shell $(PYTHON3) common-hal/gamebuino-meta/make_extra_sources.py > $(BUILD)/extra_sources.mk)

include $(BUILD)/extra_sources.mk
