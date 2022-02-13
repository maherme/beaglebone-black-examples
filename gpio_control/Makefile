TOOLCHAIN ?= /usr/bin/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/bin/
ARM_CC ?= $(TOOLCHAIN)arm-linux-gnueabihf-gcc
MCPU = cortex-a8
MFPU = neon # Alias for neon-vfpv3
CFLAGS = -Wall -mcpu=$(MCPU) -mfloat-abi=hard -mfpu=$(MFPU) -mtune=$(MCPU) $(INCLUDE)

TARGET1 = $(BIN_DIR)/test_led
TARGET2 = $(BIN_DIR)/test_7seg
TARGET3 = $(BIN_DIR)/test_button7seg
TARGET4 = $(BIN_DIR)/test_4dig7seg
TARGET5 = $(BIN_DIR)/test_lcd
SRC_DIR = .
DRV_DIR = ./drv
BSP_DIR = ./bsp
OBJ_DIR = ./obj
BIN_DIR = ./bin
INCLUDE = -I./ -I./drv -I./bsp
OBJS1 = $(OBJ_DIR)/led_user_control.o
OBJS2 = $(OBJ_DIR)/gpio_driver.o \
		$(OBJ_DIR)/counter_7seg.o
OBJS3 = $(OBJ_DIR)/gpio_driver.o \
		$(OBJ_DIR)/button_7seg.o
OBJS4 = $(OBJ_DIR)/gpio_driver.o \
		$(OBJ_DIR)/counter_4dig7seg.o
OBJS5 = $(OBJ_DIR)/print_lcd.o \
		$(OBJ_DIR)/gpio_driver.o \
		$(OBJ_DIR)/lcd_hd44780.o

$(TARGET1) : $(OBJS1)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS1) -o $(TARGET1)

$(TARGET2) : $(OBJS2)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS2) -o $(TARGET2)

$(TARGET3) : $(OBJS3)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS3) -o $(TARGET3)

$(TARGET4) : $(OBJS4)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS4) -o $(TARGET4)

$(TARGET5) : $(OBJS5)
	@mkdir -p $(BIN_DIR)
	$(ARM_CC) $(CFLAGS) $(OBJS5) -o $(TARGET5)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(ARM_CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(DRV_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(ARM_CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(BSP_DIR)/%.c
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

.PHONY : button7seg
button7seg: $(TARGET3)

.PHONY : 4dig7seg
4dig7seg: $(TARGET4)

.PHONY : lcd
lcd: $(TARGET5)
