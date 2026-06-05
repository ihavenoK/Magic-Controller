# ============================================================================
# Makefile for CyberryPotter - STM32F103CB using GCC ARM toolchain
# ============================================================================

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# GCC ARM Toolchain path (add to PATH or specify here)
# GCC_PATH = "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/14.2 Rel1/bin"

# Target
TARGET = CyberryPotter

# Build directory
BUILD_DIR = build

# Device parameters
MCU = cortex-m3
FLOAT_ABI = soft

# Keil DFP path (for StdPeriph drivers and device headers)
KEIL_DFP = D:/keil5/ARM/PACK/Keil/STM32F1xx_DFP/2.4.1
CMSIS_INC = D:/keil5/ARM/PACK/ARM/CMSIS/6.2.0/CMSIS/Core/Include

# ----------------------------------------------------------------------------
# Sources
# ----------------------------------------------------------------------------

# C sources - Startup / System
C_SOURCES = \
	RTE/Device/STM32F103CB/system_stm32f10x.c

# C sources - StdPeriph Driver (all modules)
C_SOURCES += \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/misc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_adc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_bkp.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_can.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_cec.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_crc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_dac.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_dbgmcu.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_dma.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_exti.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_flash.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_fsmc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_gpio.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_i2c.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_iwdg.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_pwr.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_rcc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_rtc.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_sdio.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_spi.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_tim.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_usart.c \
	$(KEIL_DFP)/Device/StdPeriph_Driver/src/stm32f10x_wwdg.c

# C sources - MainBoard
C_SOURCES += \
	MainBoard/CyberryPotter.c \
	MainBoard/Delay.c \
	MainBoard/IIC.c \
	MainBoard/IMU.c \
	MainBoard/main.c \
	MainBoard/MPU6050.c \
	MainBoard/eMPL/inv_mpu.c \
	MainBoard/eMPL/inv_mpu_dmp_motion_driver.c \
	MainBoard/SPI.c \
	MainBoard/USART.c \
	MainBoard/button.c \
	MainBoard/LED.c \
	MainBoard/new_study_IR.c \
	MainBoard/protocol.c

# C sources - Module
C_SOURCES += \
	Module/module0.c \
	Module/module1.c \
	Module/module2.c \
	Module/module3.c \
	Module/module4.c \
	Module/module5.c \
	Module/module6.c \
	Module/module7.c \
	Module/module8.c \
	Module/module9.c \
	Module/module10.c

# C sources - CNN/nnom
C_SOURCES += \
	CNN/nnom_lib/src/backends/nnom_local.c \
	CNN/nnom_lib/src/backends/nnom_local_q15.c \
	CNN/nnom_lib/src/core/nnom.c \
	CNN/nnom_lib/src/core/nnom_layers.c \
	CNN/nnom_lib/src/core/nnom_tensor.c \
	CNN/nnom_lib/src/core/nnom_utils.c \
	CNN/nnom_lib/src/layers/nnom_activation.c \
	CNN/nnom_lib/src/layers/nnom_avgpool.c \
	CNN/nnom_lib/src/layers/nnom_baselayer.c \
	CNN/nnom_lib/src/layers/nnom_concat.c \
	CNN/nnom_lib/src/layers/nnom_conv2d.c \
	CNN/nnom_lib/src/layers/nnom_conv2d_trans.c \
	CNN/nnom_lib/src/layers/nnom_cropping.c \
	CNN/nnom_lib/src/layers/nnom_dense.c \
	CNN/nnom_lib/src/layers/nnom_dw_conv2d.c \
	CNN/nnom_lib/src/layers/nnom_flatten.c \
	CNN/nnom_lib/src/layers/nnom_global_pool.c \
	CNN/nnom_lib/src/layers/nnom_gru_cell.c \
	CNN/nnom_lib/src/layers/nnom_input.c \
	CNN/nnom_lib/src/layers/nnom_lambda.c \
	CNN/nnom_lib/src/layers/nnom_lstm_cell.c \
	CNN/nnom_lib/src/layers/nnom_matrix.c \
	CNN/nnom_lib/src/layers/nnom_maxpool.c \
	CNN/nnom_lib/src/layers/nnom_output.c \
	CNN/nnom_lib/src/layers/nnom_reshape.c \
	CNN/nnom_lib/src/layers/nnom_rnn.c \
	CNN/nnom_lib/src/layers/nnom_simple_cell.c \
	CNN/nnom_lib/src/layers/nnom_softmax.c \
	CNN/nnom_lib/src/layers/nnom_sumpool.c \
	CNN/nnom_lib/src/layers/nnom_upsample.c \
	CNN/nnom_lib/src/layers/nnom_zero_padding.c

# ASM sources
ASM_SOURCES = \
	GCC/startup_stm32f103xb.s

# ----------------------------------------------------------------------------
# Include paths
# ----------------------------------------------------------------------------

C_INCLUDES = \
	-I$(CMSIS_INC) \
	-I$(KEIL_DFP)/Device/Include \
	-I$(KEIL_DFP)/Device/StdPeriph_Driver/inc \
	-IRTE/Device/STM32F103CB \
	-IRTE/_Target_1 \
	-ICNN/nnom_lib/inc \
	-ICNN/nnom_lib/inc/layers \
	-ICNN/nnom_lib/port \
	-ICNN \
	-IMainBoard \
	-IMainBoard/eMPL \
	-IModule

# ----------------------------------------------------------------------------
# Compiler flags
# ----------------------------------------------------------------------------

OPT = -O1

DEFS = -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -DIP=IPR

CFLAGS = $(OPT) -mcpu=$(MCU) -mthumb -mfloat-abi=$(FLOAT_ABI) \
	$(DEFS) $(C_INCLUDES) \
	-Wall -Wextra -Wno-unused-parameter -Wno-sign-compare \
	-fdata-sections -ffunction-sections \
	-std=gnu99

ASFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=$(FLOAT_ABI) $(DEFS) $(C_INCLUDES)

# ----------------------------------------------------------------------------
# Linker flags
# ----------------------------------------------------------------------------

LDSCRIPT = GCC/STM32F103CB.ld

LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=$(FLOAT_ABI) \
	-T$(LDSCRIPT) \
	--specs=nano.specs \
	--specs=nosys.specs \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# ----------------------------------------------------------------------------
# Build rules
# ----------------------------------------------------------------------------

# Generate object file names
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(OBJCOPY) -O binary -S $< $@

$(BUILD_DIR):
	-mkdir $(BUILD_DIR)

# Clean
clean:
	-if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

# Flash (requires OpenOCD or ST-Link utilities)
flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

.PHONY: all clean flash
