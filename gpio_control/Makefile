TOOLCHAIN ?= /usr/bin/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/bin/
ARM_CC ?= $(TOOLCHAIN)arm-linux-gnueabihf-gcc
MCPU = cortex-a8
MFPU = neon # Alias for neon-vfpv3
CFLAGS = -Wall -mcpu=$(MCPU) -mfloat-abi=hard -mfpu=$(MFPU) -mtune=$(MCPU) $(INCLUDE)

TARGET1 = $(BIN_DIR)/test_led
TARGET2 = $(BIN_DIR)/test_7seg
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin
INCLUDE = -I./
OBJS1 = $(OBJ_DIR)/led_user_control.o
OBJS2 = $(OBJ_DIR)/gpio_driver.o \
		$(OBJ_DIR)/counter_7seg.o

$(TARGET1) : $(OBJS1)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS1) -o $(TARGET1)

$(TARGET2) : $(OBJS2)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS2) -o $(TARGET2)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(ARM_CC) -c $(CFLAGS) $< -o $@

-include $(OBJ_DIR)/*.d

.PHONY : clean
clean:
	@rm -r $(OBJ_DIR) $(BIN_DIR)

.PHONY : led
led: $(TARGET1)

.PHONY : 7seg
7seg: $(TARGET2)
