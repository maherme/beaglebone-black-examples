TOOLCHAIN ?= /usr/bin/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/bin/
ARM_CC ?= $(TOOLCHAIN)arm-linux-gnueabihf-gcc
MCPU = cortex-a8
MFPU = neon # Alias for neon-vfpv3
CFLAGS = -Wall -mcpu=$(MCPU) -mfloat-abi=hard -mfpu=$(MFPU) -mtune=$(MCPU)

TARGET1 = $(BIN_DIR)/test_led
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin
OBJS1 = $(OBJ_DIR)/led_user_control.o

$(TARGET1) : $(OBJS1)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS1) -o $(TARGET1)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(ARM_CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	@rm -r $(OBJ_DIR) $(BIN_DIR)

.PHONY : led
led: $(TARGET1)
