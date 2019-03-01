include $(CHIBIOS)/os/hal/ports/STM32/STM32F0xx/platform.mk
MCU  = cortex-m0

include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v6m.mk

# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f0xx.mk

LDSCRIPT= $(STARTUPLD)/STM32F030x4.ld
