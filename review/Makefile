MAKEFLAGS += -j$(shell nproc)
# Toolchain
COMPILER_DIR = /home/butterfly/Apps/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/
PREFIX = $(COMPILER_DIR)arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

# Microcontroller
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Optimization
OPT = -Og

# Paths
ROOT = .
CORE = $(ROOT)/Core
DRIV = $(ROOT)/Drivers

SRC_DIRS = \
    $(ROOT)/Core/Src \
    $(DRIV)/STM32L4xx_HAL_Driver/Src \
    $(DRIV)/CMSIS/Source

# Собираем все .c файлы из указанных папок
C_SOURCES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

# ASM sources
ASM_SOURCES = \
$(DRIV)/Startup/startup_stm32l476xx.s

# Include paths
C_INCLUDES = \
-I$(CORE)/Inc/ \
-I$(DRIV)/CMSIS/Include/ \
-I$(DRIV)/STM32L4xx_HAL_Driver/Inc/ \
-I$(DRIV)/STM32L4xx_HAL_Driver/Inc/Legacy/

# Compiler flags
CFLAGS = $(MCU) $(OPT) -std=gnu11 \
-DSTM32L476xx -flto \
$(C_INCLUDES) \
-ffunction-sections -fdata-sections \
-Wall -fstack-usage --specs=nano.specs

# Linker flags
LDFLAGS = $(MCU) -T"$(ROOT)/STM32L476.ld" \
--specs=nano.specs \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
-Wl,--gc-sections

# Output files
TARGET = firmware
BUILD_DIR = Build
BIN = bin
OBJ = obj

# Default target
all: $(BUILD_DIR)/$(BIN)/$(TARGET).bin $(BUILD_DIR)/$(BIN)/$(TARGET).hex

# Create build directory


# Compile C files
OBJECTS = $(addprefix $(BUILD_DIR)/$(OBJ)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/$(OBJ)/%.o: %.c | $(BUILD_DIR)/$(OBJ)
	$(CC) -c $(CFLAGS) $< -o $@

# Compile ASM files
OBJECTS += $(addprefix $(BUILD_DIR)/$(OBJ)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/$(OBJ)/%.o: %.s | $(BUILD_DIR)/$(OBJ)
	$(AS) -c $(CFLAGS) $< -o $@

# Link
$(BUILD_DIR)/$(BIN)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

# Create HEX
$(BUILD_DIR)/$(BIN)/$(TARGET).hex: $(BUILD_DIR)/$(BIN)/$(TARGET).elf
	$(CP) -O ihex $< $@

# Create BIN
$(BUILD_DIR)/$(BIN)/$(TARGET).bin: $(BUILD_DIR)/$(BIN)/$(TARGET).elf
	$(CP) -O binary -S $< $@

# Clean
clean:
	rm -rf $(BUILD_DIR)/$(OBJ)/*
	rm -rf $(BUILD_DIR)/$(BIN)/*

# Flash with ST-Link
flash: $(BUILD_DIR)/$(BIN)/$(TARGET).bin
	st-flash write $< 0x8000000

.PHONY: all clean flash