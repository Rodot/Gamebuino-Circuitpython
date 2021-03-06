# This file is part of the MicroPython project, http://micropython.org/
#
# The MIT License (MIT)
#
# Copyright (c) 2019 Dan Halbert for Adafruit Industries
# Copyright (c) 2019 Aadalie - adding C++ support by Sorunome
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Select the board to build for.
ifeq ($(BOARD),)
  $(error You must provide a BOARD parameter)
else
  ifeq ($(wildcard boards/$(BOARD)/.),)
    $(error Invalid BOARD specified)
  endif
endif

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)

include ../../py/mkenv.mk
# Board-specific
include boards/$(BOARD)/mpconfigboard.mk
# Port-specific
include mpconfigport.mk
# CircuitPython-specific
include $(TOP)/py/circuitpy_mpconfig.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

include $(TOP)/supervisor/supervisor.mk

# Include make rules and variables common across CircuitPython builds.
include $(TOP)/py/circuitpy_defns.mk

CROSS_COMPILE = arm-none-eabi-

HAL_DIR=hal/$(MCU_SERIES)

INC += -I. \
       -I../.. \
       -I../lib/mp-readline \
       -I../lib/timeutils \
       -Iasf4/$(CHIP_FAMILY) \
       -Iasf4/$(CHIP_FAMILY)/hal/include \
       -Iasf4/$(CHIP_FAMILY)/hal/utils/include \
       -Iasf4/$(CHIP_FAMILY)/hri \
       -Iasf4/$(CHIP_FAMILY)/hpl/core \
       -Iasf4/$(CHIP_FAMILY)/hpl/gclk \
       -Iasf4/$(CHIP_FAMILY)/hpl/pm \
       -Iasf4/$(CHIP_FAMILY)/hpl/port \
       -Iasf4/$(CHIP_FAMILY)/hpl/rtc \
       -Iasf4/$(CHIP_FAMILY)/hpl/tc \
       -Iasf4/$(CHIP_FAMILY)/include \
       -Iasf4/$(CHIP_FAMILY)/CMSIS/Include \
       -Iasf4_conf/$(CHIP_FAMILY) \
       -Iboards/$(BOARD) \
       -Iboards/ \
       -Iperipherals/ \
       -Ifreetouch \
       -I../../lib/tinyusb/src \
       -I../../supervisor/shared/usb \
       -I$(BUILD) \
       $(INC_EXTRA)

ifeq ($(CHIP_FAMILY), samd21)
CFLAGS += -Os -DNDEBUG
# TinyUSB defines
CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_SAMD21 -DCFG_TUD_MIDI_RX_BUFSIZE=128 -DCFG_TUD_CDC_RX_BUFSIZE=128 -DCFG_TUD_MIDI_TX_BUFSIZE=128 -DCFG_TUD_CDC_TX_BUFSIZE=128 -DCFG_TUD_MSC_BUFSIZE=512
CPPFLAGS += -Os -DNDEBUG
# TinyUSB defines
CPPFLAGS += -DCFG_TUSB_MCU=OPT_MCU_SAMD21 -DCFG_TUD_MIDI_RX_BUFSIZE=128 -DCFG_TUD_CDC_RX_BUFSIZE=128 -DCFG_TUD_MIDI_TX_BUFSIZE=128 -DCFG_TUD_CDC_TX_BUFSIZE=128 -DCFG_TUD_MSC_BUFSIZE=512
endif

ifeq ($(CHIP_FAMILY), samd51)
CFLAGS += -Os -DNDEBUG
# TinyUSB defines
CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_SAMD51 -DCFG_TUD_MIDI_RX_BUFSIZE=128 -DCFG_TUD_CDC_RX_BUFSIZE=256 -DCFG_TUD_MIDI_TX_BUFSIZE=128 -DCFG_TUD_CDC_TX_BUFSIZE=256 -DCFG_TUD_MSC_BUFSIZE=1024
endif

#Debugging/Optimization
ifeq ($(DEBUG), 1)
  # Turn on Python modules useful for debugging (e.g. uheap, ustack).
  CFLAGS += -ggdb
  # You may want to disable -flto if it interferes with debugging.
  CFLAGS += -flto
  # You may want to enable these flags to make setting breakpoints easier.
  # CFLAGS += -fno-inline -fno-ipa-sra
  ifeq ($(CHIP_FAMILY), samd21)
    CFLAGS += -DENABLE_MICRO_TRACE_BUFFER
  endif
else
  # -finline-limit can shrink the image size.
  # -finline-limit=80 or so is similar to not having it on.
  # There is no simple default value, though.
  ifeq ($(INTERNAL_FLASH_FILESYSTEM),1)
    CFLAGS += -finline-limit=50
    CPPFLAGS += -finline-limit=50
  endif
  ifdef CFLAGS_INLINE_LIMIT
    CFLAGS += -finline-limit=$(CFLAGS_INLINE_LIMIT)
    CPPFLAGS += -finline-limit=$(CFLAGS_INLINE_LIMIT)
  endif
  CFLAGS += -flto
  CPPFLAGS += -flto
endif

CFLAGS += $(INC) -Wall -Werror -std=gnu11 -nostdlib $(BASE_CFLAGS) $(CFLAGS_MOD) $(COPT) -g
CPPFLAGS += $(INC) -Wall -Werror -std=gnu++11 -nostdlib $(BASE_CPPFLAGS) $(CFLAGS_MOD) $(COPT) -g

ifeq ($(CHIP_FAMILY), samd21)
CFLAGS += \
	-mthumb \
	-mabi=aapcs-linux \
	-mcpu=cortex-m0plus \
	-msoft-float \
	-mfloat-abi=soft \
	-DSAMD21
CPPFLAGS += \
	-mthumb \
	-mabi=aapcs-linux \
	-mcpu=cortex-m0plus \
	-msoft-float \
	-mfloat-abi=soft \
	-DSAMD21
endif
ifeq ($(CHIP_FAMILY), samd51)
CFLAGS += \
	-mthumb \
	-mabi=aapcs-linux \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-DSAMD51
endif

LDFLAGS = $(CFLAGS) -static -nostartfiles -ffreestanding -nostartfiles -fshort-enums -Wl,-nostdlib -Wl,-T,$(LD_FILE) -Wl,-Map=$@.map -Wl,-cref -Wl,-gc-sections -specs=nano.specs -specs=nosys.specs -g -lc -lnosys
LIBS := -lgcc -lc

# Use toolchain libm if we're not using our own.
ifndef INTERNAL_LIBM
LIBS += -lm
endif

ifeq ($(CHIP_FAMILY), samd21)
LDFLAGS += -mthumb -mcpu=cortex-m0plus -Lasf/thirdparty/CMSIS/Lib/GCC/
ifeq ($(BOOTLOADER_SIZE),)
BOOTLOADER_SIZE := 0x2000
endif
else ifeq ($(CHIP_FAMILY), samd51)
LDFLAGS += -mthumb -mcpu=cortex-m4
ifeq ($(BOOTLOADER_SIZE),)
BOOTLOADER_SIZE := 0x4000
endif
endif

SRC_ASF := \
	gcc/gcc/startup_$(CHIP_FAMILY).c \
	gcc/system_$(CHIP_FAMILY).c \
	hal/src/hal_adc_sync.c \
	hal/src/hal_atomic.c \
	hal/src/hal_calendar.c \
	hal/src/hal_dac_sync.c \
	hal/src/hal_delay.c \
	hal/src/hal_flash.c \
	hal/src/hal_i2c_m_sync.c \
	hal/src/hal_io.c \
	hal/src/hal_sleep.c \
	hal/src/hal_spi_m_sync.c \
	hal/src/hal_timer.c \
	hal/src/hal_usart_async.c \
	hpl/adc/hpl_adc.c \
	hpl/core/hpl_init.c \
	hpl/dac/hpl_dac.c \
	hpl/gclk/hpl_gclk.c \
	hpl/nvmctrl/hpl_nvmctrl.c \
	hpl/pm/hpl_pm.c \
	hpl/rtc/hpl_rtc.c \
	hpl/sercom/hpl_sercom.c \
	hpl/systick/hpl_systick.c \
	hal/utils/src/utils_list.c \
	hal/utils/src/utils_ringbuffer.c \

ifeq ($(CHIP_FAMILY), samd21)
SRC_ASF += \
	hpl/core/hpl_core_m0plus_base.c \
	hpl/sysctrl/hpl_sysctrl.c \

else ifeq ($(CHIP_FAMILY), samd51)
SRC_ASF += \
    hal/src/hal_rand_sync.c \
	hpl/core/hpl_core_m4.c \
	hpl/mclk/hpl_mclk.c \
	hpl/osc32kctrl/hpl_osc32kctrl.c \
	hpl/oscctrl/hpl_oscctrl.c \
	hpl/trng/hpl_trng.c \

endif

SRC_ASF := $(addprefix asf4/$(CHIP_FAMILY)/, $(SRC_ASF))

SRC_C = \
	audio_dma.c \
	background.c \
	bindings/samd/Clock.c \
	bindings/samd/__init__.c \
	boards/$(BOARD)/board.c \
	boards/$(BOARD)/pins.c \
	eic_handler.c \
	fatfs_port.c \
	freetouch/adafruit_ptc.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c \
	lib/oofatfs/ff.c \
	lib/oofatfs/option/ccsbcs.c \
	lib/timeutils/timeutils.c \
	lib/tinyusb/src/portable/microchip/$(CHIP_FAMILY)/dcd_$(CHIP_FAMILY).c \
	lib/utils/buffer_helper.c \
	lib/utils/context_manager_helpers.c \
	lib/utils/interrupt_char.c \
	lib/utils/pyexec.c \
	lib/utils/stdout_helpers.c \
	lib/utils/sys_stdio_mphal.c \
	mphalport.c \
	peripherals/samd/$(CHIP_FAMILY)/adc.c \
	peripherals/samd/$(CHIP_FAMILY)/cache.c \
	peripherals/samd/$(CHIP_FAMILY)/clocks.c \
	peripherals/samd/$(CHIP_FAMILY)/dma.c \
	peripherals/samd/$(CHIP_FAMILY)/events.c \
	peripherals/samd/$(CHIP_FAMILY)/external_interrupts.c \
	peripherals/samd/$(CHIP_FAMILY)/pins.c \
	peripherals/samd/$(CHIP_FAMILY)/sercom.c \
	peripherals/samd/$(CHIP_FAMILY)/timers.c \
	peripherals/samd/clocks.c \
	peripherals/samd/dma.c \
	peripherals/samd/events.c \
	peripherals/samd/external_interrupts.c \
	peripherals/samd/sercom.c \
	peripherals/samd/timers.c \
	reset.c \
	supervisor/shared/memory.c \
	tick.c \
	timer_handler.c \
	$(SRC_C_EXTRA)

SRC_CPP = \
	$(SRC_CPP_EXTRA)

ifeq ($(CIRCUITPY_NETWORK),1)
CFLAGS += -DMICROPY_PY_NETWORK=1

SRC_MOD += lib/netutils/netutils.c

ifneq ($(MICROPY_PY_WIZNET5K),0)
WIZNET5K_DIR=drivers/wiznet5k
INC += -I$(TOP)/$(WIZNET5K_DIR)
CFLAGS_MOD += -DMICROPY_PY_WIZNET5K=$(MICROPY_PY_WIZNET5K) -D_WIZCHIP_=$(MICROPY_PY_WIZNET5K)
SRC_MOD += $(addprefix $(WIZNET5K_DIR)/,\
        ethernet/w$(MICROPY_PY_WIZNET5K)/w$(MICROPY_PY_WIZNET5K).c \
        ethernet/wizchip_conf.c \
        ethernet/socket.c \
        internet/dns/dns.c \
        internet/dhcp/dhcp.c \
        )

endif # MICROPY_PY_WIZNET5K
endif # CIRCUITPY_NETWORK

ifeq ($(CIRCUITPY_NETWORK),1)
ifneq ($(MICROPY_PY_WIZNET5K),0)
SRC_SHARED_MODULE += wiznet/__init__.c wiznet/wiznet5k.c
endif
endif

# The smallest SAMD51 packages don't have I2S. Everything else does.
ifeq ($(CIRCUITPY_AUDIOBUSIO),1)
SRC_C += peripherals/samd/i2s.c peripherals/samd/$(CHIP_FAMILY)/i2s.c
endif

SRC_COMMON_HAL_EXPANDED = $(addprefix shared-bindings/, $(SRC_COMMON_HAL)) \
                          $(addprefix shared-bindings/, $(SRC_BINDINGS_ENUMS)) \
                          $(addprefix common-hal/, $(SRC_COMMON_HAL))

SRC_SHARED_MODULE_EXPANDED = $(addprefix shared-bindings/, $(SRC_SHARED_MODULE)) \
                             $(addprefix shared-module/, $(SRC_SHARED_MODULE))

SRC_S = supervisor/$(CHIP_FAMILY)_cpu.s

OBJ = $(PY_O) $(SUPERVISOR_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CPP:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_ASF:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_COMMON_HAL_EXPANDED:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_SHARED_MODULE_EXPANDED:.c=.o))
ifeq ($(INTERNAL_LIBM),1)
OBJ += $(addprefix $(BUILD)/, $(SRC_LIBM:.c=.o))
endif
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_MOD:.c=.o))

SRC_QSTR += $(SRC_C) $(SRC_CPP) $(SRC_SUPERVISOR) $(SRC_COMMON_HAL_EXPANDED) $(SRC_SHARED_MODULE_EXPANDED)
# Sources that only hold QSTRs after pre-processing.
SRC_QSTR_PREPROCESSOR += peripherals/samd/$(CHIP_FAMILY)/clocks.c

all: $(BUILD)/firmware.bin $(BUILD)/firmware.uf2

$(BUILD)/firmware.elf: $(OBJ)
	$(STEPECHO) "LINK $@"
	$(Q)$(CXX) -o $@ $(LDFLAGS) $^ -Wl,--start-group $(LIBS) -Wl,--end-group
	$(Q)$(SIZE) $@ | $(PYTHON3) $(TOP)/tools/build_memory_info.py $(LD_FILE)

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(STEPECHO) "Create $@"
	$(Q)$(OBJCOPY) -O binary -j .vectors -j .text -j .data $^ $@

$(BUILD)/firmware.uf2: $(BUILD)/firmware.bin
	$(STEPECHO) "Create $@"
	$(Q)$(PYTHON3) $(TOP)/tools/uf2/utils/uf2conv.py -b $(BOOTLOADER_SIZE) -c -o $@ $^

include $(TOP)/py/mkrules.mk

# Print out the value of a make variable.
# https://stackoverflow.com/questions/16467718/how-to-print-out-a-variable-in-makefile
print-%:
	@echo $* = $($*)
