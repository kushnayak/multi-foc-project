PROJECT = gimble-driver
BUILD_DIR = bin

# SHARED_DIR = ../my-common-code
# AFILES += api-asm.S

# CFILES += $(wildcard ./*.c )
CFILES += main.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f103c8t6
# OOCD_FILE = board/stm32f4discovery.cfg

# enable print floats 
LDFLAGS += -u _printf_float

# You shouldn't have to edit anything below here.
# VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . include)
OPENCM3_DIR=../libopencm3


# --------free rtos stuff
# 
# FREE_RTOS_DIR=../FreeRTOS-Kernel
# FREE_RTOS_INC=$(FREE_RTOS_DIR)/include
# FREE_RTOS_PORT=$(FREE_RTOS_DIR)/portable/GCC/ARM_CM4F
# FREE_RTOS_HEAP=4
# 
# VPATH += $(FREE_RTOS_DIR) $(FREE_RTOS_PORT) $(FREE_RTOS_DIR)/portable/MemMang
# CFILES += $(notdir $(wildcard $(FREE_RTOS_DIR)/*.c) \
# 		   $(FREE_RTOS_PORT)/port.c	    \
# 	   	   $(FREE_RTOS_DIR)/portable/MemMang/heap_$(FREE_RTOS_HEAP).c)
# 
# 
# INCLUDES += $(patsubst %,-I%, $(FREE_RTOS_INC) $(FREE_RTOS_PORT))
$(info cfiles : $(CFILES))
$(info includes : $(INCLUDES))
# ----------

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
