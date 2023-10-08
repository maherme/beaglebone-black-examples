#include <linux/device.h>
#include <linux/delay.h>
#include "lcd.h"
#include "gpio.h"

static void write_4_bits(uint8_t data, struct device* dev)
{
    /* Write 4 bits in parallel */
    gpio_write_value(LCD_D4, ((data >> 0) & 0x1), dev);
    gpio_write_value(LCD_D5, ((data >> 1) & 0x1), dev);
    gpio_write_value(LCD_D6, ((data >> 2) & 0x1), dev);
    gpio_write_value(LCD_D7, ((data >> 3) & 0x1), dev);

    lcd_enable(dev);
}

int lcd_init(struct device* dev)
{
    gpio_configure_dir(LCD_RS, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_RW, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_EN, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_D4, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_D5, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_D6, GPIO_DIR_OUT, dev);
    gpio_configure_dir(LCD_D7, GPIO_DIR_OUT, dev);

    gpio_write_value(LCD_RS, GPIO_LOW_VALUE, dev);
    gpio_write_value(LCD_EN, GPIO_LOW_VALUE, dev);
    gpio_write_value(LCD_D4, GPIO_LOW_VALUE, dev);
    gpio_write_value(LCD_D5, GPIO_LOW_VALUE, dev);
    gpio_write_value(LCD_D6, GPIO_LOW_VALUE, dev);
    gpio_write_value(LCD_D7, GPIO_LOW_VALUE, dev);

    mdelay(40);

    gpio_write_value(LCD_RS, GPIO_LOW_VALUE, dev);

    gpio_write_value(LCD_RW, GPIO_LOW_VALUE, dev);

    write_4_bits(0x03, dev);
    mdelay(5);

    write_4_bits(0x03, dev);
    mdelay(150);

    write_4_bits(0x03, dev);
    write_4_bits(0x02, dev);

    lcd_send_command(LCD_CMD_4DL_2N_5X8F, dev);
    lcd_send_command(LCD_CMD_DON_CURON, dev);

    lcd_display_clear(dev);

    lcd_send_command(LCD_CMD_INCADD, dev);

    return 0;
}

void lcd_deinit(struct device* dev)
{
    lcd_display_clear(dev);
    lcd_display_return_home(dev);
}

void lcd_enable(struct device* dev)
{
    gpio_write_value(LCD_EN, GPIO_LOW_VALUE, dev);
    udelay(10);
    gpio_write_value(LCD_EN, GPIO_HIGH_VALUE, dev);
    udelay(10);
    gpio_write_value(LCD_EN, GPIO_LOW_VALUE, dev);
    udelay(100);
}

void lcd_send_command(uint8_t command, struct device* dev)
{
    /* Set RS low for LCD command */
    gpio_write_value(LCD_RS, GPIO_LOW_VALUE, dev);

    /* Ser RW low for writting */
    gpio_write_value(LCD_RW, GPIO_LOW_VALUE, dev);

    write_4_bits((command >> 4), dev);
    write_4_bits(command, dev);
}

void lcd_display_clear(struct device* dev)
{
    lcd_send_command(LCD_CMD_DIS_CLEAR, dev);
    mdelay(2);
}

void lcd_print_char(uint8_t data, struct device* dev)
{
    /* Set RS high for user data */
    gpio_write_value(LCD_RS, GPIO_HIGH_VALUE, dev);

    /* Set RW low for writting */
    gpio_write_value(LCD_RW, GPIO_LOW_VALUE, dev);

    write_4_bits((data >> 4), dev);
    write_4_bits(data, dev);
}

void lcd_print_string(char* message, struct device* dev)
{
    do
    {
        lcd_print_char((u8)*message++, dev);
    }
    while(*message != '\0');
}


void lcd_display_return_home(struct device* dev)
{
    lcd_send_command(LCD_CMD_DIS_RETURN_HOME, dev);
    mdelay(2);
}

void lcd_set_cursor(u8 row, u8 column, struct device* dev)
{
    column--;
    switch(row)
    {
        case 1:
            lcd_send_command((column |= DDRAM_FIRST_LINE_BASE_ADDR), dev);
            break;
        case 2:
            lcd_send_command((column |= DDRAM_SECOND_LINE_BASE_ADDR), dev);
            break;
        default:
            break;
    }
}
