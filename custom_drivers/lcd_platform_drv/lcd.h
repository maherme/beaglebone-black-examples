#ifndef LCD_H
#define LCD_H

#define LCD_CMD_4DL_2N_5X8F             0x28
#define LCD_CMD_DON_CURON               0x0E
#define LCD_CMD_DIS_CLEAR               0x01
#define LCD_CMD_INCADD                  0x06
#define LCD_CMD_DIS_RETURN_HOME         0x02
#define LCD_CMD_SET_DDRAM_ADDRESS       0X80

#define DDRAM_FIRST_LINE_BASE_ADDR      LCD_CMD_SET_DDRAM_ADDRESS
#define DDRAM_SECOND_LINE_BASE_ADDR     (LCD_CMD_SET_DDRAM_ADDRESS | 0x40)

enum{
    LCD_RS,
    LCD_RW,
    LCD_EN,
    LCD_D4,
    LCD_D5,
    LCD_D6,
    LCD_D7
};

int lcd_init(struct device* dev);
void lcd_deinit(struct device* dev);
void lcd_enable(struct device* dev);
void lcd_send_command(uint8_t command, struct device* dev);
void lcd_display_clear(struct device* dev);
void lcd_print_char(uint8_t data, struct device* dev);
void lcd_print_string(char* message, struct device* dev);
void lcd_display_return_home(struct device* dev);
void lcd_set_cursor(u8 row, u8 column, struct device* dev);

#endif /* LCD_H */
