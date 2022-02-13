/********************************************************************************************************//**
* @file lcd_hd44780.c
*
* @brief Functions for controlling the LCD HD44780 module.
*
* Public Functions:
*       - void hd44780_init(void)
*       - void hd44780_send_cmd(uint8_t cmd)
*       - void hd44780_set_cursor(uint8_t row, uint8_t column)
*       - void hd44780_print_char(uint8_t value)
*       - void hd44780_print_string(char* msg)
*       - void hd44780_printf(const char* fmt, ...)
*       - void hd44780_load_cgram(char tab[], uint8_t charnum)
*/

#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include "gpio_driver.h"
#include "lcd_hd44780.h"

/***********************************************************************************************************/
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for enabling the HD44780 module.
 * @return void.
 */
static void hd44780_enable(void);

/***********************************************************************************************************/
/*                                       Public API Definitions                                            */
/***********************************************************************************************************/

void hd44780_init(void){

    uint8_t cmd = 0;

    /* Export all needed gpios */
    gpio_export(GPIO_66_P8_7_RS_4);
    gpio_export(GPIO_67_P8_8_RW_5);
    gpio_export(GPIO_69_P8_9_EN_6);
    gpio_export(GPIO_68_P8_10_D4_11);
    gpio_export(GPIO_45_P8_11_D5_12);
    gpio_export(GPIO_44_P8_12_D6_13);
    gpio_export(GPIO_26_P8_14_D7_14);

    /* Configure direction */
    gpio_config_dir(GPIO_66_P8_7_RS_4, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_67_P8_8_RW_5, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_69_P8_9_EN_6, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_68_P8_10_D4_11, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_45_P8_11_D5_12, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_44_P8_12_D6_13, GPIO_DIR_OUT);
    gpio_config_dir(GPIO_26_P8_14_D7_14, GPIO_DIR_OUT);

    /* Set initial value */
    gpio_write_value(GPIO_66_P8_7_RS_4, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_67_P8_8_RW_5, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_69_P8_9_EN_6, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_68_P8_10_D4_11, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_45_P8_11_D5_12, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_44_P8_12_D6_13, GPIO_LOW_VALUE);
    gpio_write_value(GPIO_26_P8_14_D7_14, GPIO_LOW_VALUE);

    cmd = HD44780_CMD_FUNC_SET | DATA_LEN_4 | DISPLAY_2_LINES | MATRIX_5_X_8;
    hd44780_send_cmd(cmd);

    /* Wait until BF is set to 0, for ~5ms following data sheet */
    usleep(INS_WAIT_TIME);

    cmd = HD44780_CMD_MODESET | INC_CURSOR;
    hd44780_send_cmd(cmd);

    usleep(INS_WAIT_TIME);

    cmd = HD44780_CMD_DISPLAY | DISPLAY_ON | CURSOR_OFF;
    hd44780_send_cmd(cmd);

    usleep(INS_WAIT_TIME);
}

void hd44780_send_cmd(uint8_t cmd){

    uint8_t cmd_msb = (cmd >> 4) & 0x0F;
    uint8_t cmd_lsb = cmd & 0x0F;

    /* Set RS to 0 */
    gpio_write_value(GPIO_66_P8_7_RS_4, COMMAND_MODE);

    /* Send the MSB */
    gpio_write_value(GPIO_68_P8_10_D4_11, (cmd_msb & (1 << 0)));
    gpio_write_value(GPIO_45_P8_11_D5_12, (cmd_msb & (1 << 1)));
    gpio_write_value(GPIO_44_P8_12_D6_13, (cmd_msb & (1 << 2)));
    gpio_write_value(GPIO_26_P8_14_D7_14, (cmd_msb & (1 << 3)));
    hd44780_enable();
    /* Send the LSB */
    gpio_write_value(GPIO_68_P8_10_D4_11, (cmd_lsb & (1 << 0)));
    gpio_write_value(GPIO_45_P8_11_D5_12, (cmd_lsb & (1 << 1)));
    gpio_write_value(GPIO_44_P8_12_D6_13, (cmd_lsb & (1 << 2)));
    gpio_write_value(GPIO_26_P8_14_D7_14, (cmd_lsb & (1 << 3)));
    hd44780_enable();
}

void hd44780_set_cursor(uint8_t row, uint8_t column){

    column--;
    switch(row){
        case 1:
            hd44780_send_cmd(column |= DDRAM_FST_LN_BASE_ADDR);
            break;
        case 2:
            hd44780_send_cmd(column |= DDRAM_SND_LN_BASE_ADDR);
            break;
        default:
            break;
    }
}

void hd44780_print_char(uint8_t value){

    uint8_t tmp = 0;

    gpio_write_value(GPIO_66_P8_7_RS_4, USER_DATA_MODE);

    /* Write MSB */
    tmp = (value >> 4) & 0x0F;
    gpio_write_value(GPIO_68_P8_10_D4_11, (tmp & (1 << 0)));
    gpio_write_value(GPIO_45_P8_11_D5_12, (tmp & (1 << 1)));
    gpio_write_value(GPIO_44_P8_12_D6_13, (tmp & (1 << 2)));
    gpio_write_value(GPIO_26_P8_14_D7_14, (tmp & (1 << 3)));
    hd44780_enable();

    /* Write LSB */
    tmp = value & 0x0F;
    gpio_write_value(GPIO_68_P8_10_D4_11, (tmp & (1 << 0)));
    gpio_write_value(GPIO_45_P8_11_D5_12, (tmp & (1 << 1)));
    gpio_write_value(GPIO_44_P8_12_D6_13, (tmp & (1 << 2)));
    gpio_write_value(GPIO_26_P8_14_D7_14, (tmp & (1 << 3)));
    hd44780_enable();

    usleep(5000); /* 5 ms */
}

void hd44780_print_string(char* msg){

    do{
        hd44780_print_char((uint8_t)*msg++);
    }while(*msg != '\0');
}

void hd44780_printf(const char* fmt, ...){

    int i = 0;
    uint32_t text_size = 0;
    uint32_t letter = 0;
    static char text_buffer[32] = {0};
    va_list args;

    va_start(args, fmt);
    text_size = vsprintf(text_buffer, fmt, args);

    for(i = 0; i < text_size; i++){
        letter = text_buffer[i];
        if(letter == 10){
            break;
        }
        else{
            if((letter > 0x1F) && (letter < 0x80))
                hd44780_print_char(letter);
        }
    }
}

void hd44780_load_cgram(char tab[], uint8_t charnum){

    uint8_t i = 0;

    charnum = charnum * 8;

    for(i = 0; i < charnum; i++){
        hd44780_print_char(tab[i]);
        usleep(1000); /* 1 ms */
    }
}

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

static void hd44780_enable(void){

    gpio_write_value(GPIO_69_P8_9_EN_6, 1);
    usleep(2000); /* 2 ms */
    gpio_write_value(GPIO_69_P8_9_EN_6, 0);
}
