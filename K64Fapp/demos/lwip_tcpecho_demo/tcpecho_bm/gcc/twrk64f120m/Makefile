#-----------------------------------------------
# Make command:
# mingw32-make build=<build> target=<target> stack=0x2000 heap=0x2000
# <build>: debug or release, release by default.
# <target>: sram or flash, flash by default.
#-----------------------------------------------

#-----------------------------------------------
# setup variables
# ----------------------------------------------

SDK_ROOT := $(abspath ../../../../../)

#-----------------------------------------------
# set board infomation
# The CHIP must be set correctly, otherwise the
# platform_lib cannot be linked.
# CPU type: cortex-m0, cortex-m0plus,
# cortex-m1, cortex-m3, cortex-m4, 
#-----------------------------------------------
BOARD := twrk64f120m
CHIP := K64F12
CPU := cortex-m4

#-----------------------------------------------
# Choose the float point method.
# cortex-m4 can use: NO_FP, SOFT_FP and HARD_FP.
# Other cortex-m series should only use NO_FP.
# Application's float point configuration must
# be the same with the libraries it linked.
#-----------------------------------------------
CHOOSE_FLOAT = HARD_FP

APP_NAME = lwip_tcpecho_demo
#-----------------------------------------------
# Debug or Release
# Release by default
#-----------------------------------------------
build ?= release

#-----------------------------------------------
# Sram target or flash target
# flash target by defult.
#-----------------------------------------------
target ?= flash

include $(SDK_ROOT)/mk/common.mk

#-----------------------------------------------
# Link libraries
#-----------------------------------------------
ifeq "$(build)" "debug"
LINK_LIB_NAME := $(SDK_ROOT)/lib/ksdk_platform_lib/gcc/$(CHIP)/Debug/ksdk_platform_lib.a
else
LINK_LIB_NAME := $(SDK_ROOT)/lib/ksdk_platform_lib/gcc/$(CHIP)/Release/ksdk_platform_lib.a
endif

#-----------------------------------------------
# Include path. Add the include paths like this:
# INCLUDES += ./include/
#-----------------------------------------------
INCLUDES += $(SDK_ROOT)/platform/startup/M$(CHIP)         \
            $(SDK_ROOT)/boards/$(BOARD)                   \
            $(SDK_ROOT)/platform/drivers/adc   \
            $(SDK_ROOT)/platform/drivers/can   \
            $(SDK_ROOT)/platform/drivers/clock \
            $(SDK_ROOT)/platform/drivers/dspi/dspi_master  \
            $(SDK_ROOT)/platform/drivers/dspi/dspi_slave \
            $(SDK_ROOT)/platform/drivers/edma  \
            $(SDK_ROOT)/platform/drivers/enet  \
            $(SDK_ROOT)/platform/drivers/flash \
            $(SDK_ROOT)/platform/drivers/flextimer \
            $(SDK_ROOT)/platform/drivers/gpio \
            $(SDK_ROOT)/platform/drivers/i2c/i2c_master \
            $(SDK_ROOT)/platform/drivers/i2c/i2c_slave \
            $(SDK_ROOT)/platform/drivers/interrupt \
            $(SDK_ROOT)/platform/drivers/khci \
            $(SDK_ROOT)/platform/drivers/pit \
            $(SDK_ROOT)/platform/drivers/rtc \
            $(SDK_ROOT)/platform/drivers/sai \
            $(SDK_ROOT)/platform/drivers/sdhc \
            $(SDK_ROOT)/platform/drivers/smc \
            $(SDK_ROOT)/platform/drivers/spi \
            $(SDK_ROOT)/platform/drivers/uart  \
            $(SDK_ROOT)/platform/drivers/wdog \
            $(SDK_ROOT)/platform/hal/adc \
            $(SDK_ROOT)/platform/hal/can \
            $(SDK_ROOT)/platform/hal/dmamux \
            $(SDK_ROOT)/platform/hal/dspi \
            $(SDK_ROOT)/platform/hal/edma \
            $(SDK_ROOT)/platform/hal/enet \
            $(SDK_ROOT)/platform/hal/flextimer \
            $(SDK_ROOT)/platform/hal/gpio \
            $(SDK_ROOT)/platform/hal/i2c \
            $(SDK_ROOT)/platform/hal/mcg \
            $(SDK_ROOT)/platform/hal/osc \
            $(SDK_ROOT)/platform/hal/pit \
            $(SDK_ROOT)/platform/hal/pmc \
            $(SDK_ROOT)/platform/hal/port      \
            $(SDK_ROOT)/platform/hal/rtc \
            $(SDK_ROOT)/platform/hal/sai \
            $(SDK_ROOT)/platform/hal/sdhc \
            $(SDK_ROOT)/platform/hal/sim \
            $(SDK_ROOT)/platform/hal/smc \
            $(SDK_ROOT)/platform/hal/uart      \
            $(SDK_ROOT)/platform/hal/wdog \
            $(SDK_ROOT)/tcpip/lwip                        \
            $(SDK_ROOT)/tcpip/lwip/port                   \
            $(SDK_ROOT)/tcpip/lwip/port/arch              \
            $(SDK_ROOT)/tcpip/lwip/src                    \
            $(SDK_ROOT)/tcpip/lwip/src/include            \
            $(SDK_ROOT)/tcpip/lwip/src/include/ipv4       \
            $(SDK_ROOT)/tcpip/lwip/src/include/ipv4/lwip  \
            $(SDK_ROOT)/tcpip/lwip/src/include/ipv6       \
            $(SDK_ROOT)/tcpip/lwip/src/include/ipv6/lwip  \
            $(SDK_ROOT)/tcpip/lwip/src/include/lwip       \
            $(SDK_ROOT)/tcpip/lwip/src/include/netif      \
            $(SDK_ROOT)/tcpip/lwip/src/include/posix      \
            $(SDK_ROOT)/platform/utilities                \
            $(SDK_ROOT)/boards/common/phyksz8041          \
            ../../src

include $(SDK_ROOT)/demos/common_include.mk

#-----------------------------------------------
# Startup files
#-----------------------------------------------
STARTUP_ROOT := $(SDK_ROOT)/platform/startup
STARTUP_SOURCES := $(STARTUP_ROOT)/startup.c \
                   $(STARTUP_ROOT)/M$(CHIP)/system_M$(CHIP).c \
                   $(STARTUP_ROOT)/M$(CHIP)/gcc/startup_M$(CHIP).S

#-----------------------------------------------
# Application files
#-----------------------------------------------
APP_SOURCES  := $(wildcard $(APPS_ROOT)/$(APP_NAME)/tcpecho_bm/src/*.c)  \

#-----------------------------------------------
# Board related files, e.g. platform_pins.c
#-----------------------------------------------
BOARD_SOURCES := $(wildcard $(BOARD_ROOT)/*.c)

#-----------------------------------------------
# Add all sources needed here
#-----------------------------------------------
SOURCES = $(STARTUP_SOURCES) $(APP_SOURCES) $(BOARD_SOURCES) \
		  $(SDK_ROOT)/platform/utilities/src/fsl_debug_console.c \
		  $(SDK_ROOT)/platform/utilities/src/fsl_misc_utilities.c \
		  $(SDK_ROOT)/boards/common/phyksz8041/fsl_phy_driver.c   \
		  $(SDK_ROOT)/platform/drivers/enet/fsl_enet_irq.c \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/port/*.c)     \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/api/*.c)   \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/core/*.c)   \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/core/ipv4/*.c )  \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/core/snmp/*.c)   \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/netif/*.c )  \
		  $(wildcard $(SDK_ROOT)/tcpip/lwip/src/netif/ppp/*.c)   \

#-----------------------------------------------
# Add all pre-defined symbols here
#-----------------------------------------------
DEFINES += -DCPU_MK64FN1M0VMD12 \
		   -DTWR_K64F120M \
           -DTOWER

#-----------------------------------------------
# Link file
#-----------------------------------------------
ifeq "$(target)" "sram"
LD_FILE_NAME := $(LD_FILE_ROOT)/$(CHIP)/K64FN1Mxxx12_ram.ld
else
LD_FILE_NAME := $(LD_FILE_ROOT)/$(CHIP)/K64FN1Mxxx12_flash.ld
endif

#-----------------------------------------------
# Define the stack size and heap size.
#-----------------------------------------------
stack ?= 0x2000
heap  ?= 0x2000
DEFINES += -D__STACK_SIZE=$(stack) \
           -D__HEAP_SIZE=$(heap)

include $(SDK_ROOT)/mk/targets.mk 
