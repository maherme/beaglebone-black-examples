/********************************************************************************************************//**
* @file lcd_hd44780.h
*
* @brief Header file containing the prototypes of the APIs for the LCD HD44780 module.
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

#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include <stdint.h>

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

/**
 * @defgroup GPIO_PIN Connected GPIO pins to the HD44780 module.
 * @{
 */
#define GPIO_66_P8_7_RS_4       66  /**< @brief Register Selection (character of command) */
#define GPIO_67_P8_8_RW_5       67  /**< @brief Read/write */
#define GPIO_69_P8_9_EN_6       69  /**< @brief Enable */
#define GPIO_68_P8_10_D4_11     68  /**< @brief Data line 4 */
#define GPIO_45_P8_11_D5_12     45  /**< @brief Data line 5 */
#define GPIO_44_P8_12_D6_13     44  /**< @brief Data line 6 */
#define GPIO_26_P8_14_D7_14     26  /**< @brief Data line 7 */
/** @} */

/**
 * @defgroup CMD_FUNC_SET Sets interface data length, number of display lines and character font.
 * @{
 */
#define HD44780_CMD_FUNC_SET    0x20
#define DATA_LEN_8              (1 << 4)
#define DATA_LEN_4              (HD44780_CMD_FUNC_SET & ~(DATA_LEN_8))
#define DISPLAY_2_LINES         (1 << 3)
#define DISPLAY_1_LINE          (HD44780_CMD_FUNC_SET & ~(DISPLAY_2_LINES))
#define MATRIX_5_X_10           (1 << 2)
#define MATRIX_5_X_8            (HD44780_CMD_FUNC_SET & ~(MATRIX_5_X_10))
/** @} */

/**
 * @defgroup CMD_DISPLAY Sets entire display on/off, cursor on/off and blinking of cursor position.
 * @{
 */
#define HD44780_CMD_DISPLAY     0x08
#define DISPLAY_ON              (1 << 2)
#define DISPLAY_OFF             (HD44780_CMD_DISPLAY & ~(DISPLAY_ON))
#define CURSOR_ON               (1 << 1)
#define CURSOR_OFF              (HD44780_CMD_DISPLAY & ~(CURSOR_ON))
#define BLINK_CURSOR_ON         (1 << 0)
#define BLINK_CURSOR_OFF        (HDD44780_CMD_DISPLAY & ~(BLINK_CURSOR_ON))
/** @} */

/**
 * @defgroup CMD_MODESET Sets cursor move direction and specifies display shift.
 * @{
 */
#define HD44780_CMD_MODESET     0x04
#define INC_CURSOR              (1 << 1)
#define DEC_CURSOR              (HD44780_CMD_MODESET & ~(INC_CURSOR))
#define ACCOMPANY_DIS_SHIFT     (1 << 0)
#define NOT_ACCOMPANY_DIS_SHIFT (HD44780_CMD_MODESET & ~(ACCOMPANY_DIS_SHIFT))
/** @} */

#define COMMAND_MODE            0
#define USER_DATA_MODE          1

#define INS_WAIT_TIME           8000

#define CMD_SET_DDRAM_ADDR      0x80
#define DDRAM_FST_LN_BASE_ADDR  CMD_SET_DDRAM_ADDR
#define DDRAM_SND_LN_BASE_ADDR  (CMD_SET_DDRAM_ADDR | 0x40)

#define HD44780_CMD_CLEAR_DISP  0x01

/***********************************************************************************************************/
/*                                       APIs Supported                                                    */
/***********************************************************************************************************/

/**
 * @brief Function for initialize the LCD HD44780 module.
 * @return void.
 */
void hd44780_init(void);

/**
 * @brief Function for sending a command to the HD44780 module.
 * @param[in] cmd Is the command to send.
 * @return void.
 */
void hd44780_send_cmd(uint8_t cmd);

/**
 * @brief Function for setting the cursor in a row and column.
 * @param[in] row Is the selected row.
 * @param[in] column Is the selected column.
 * @return void.
 */
void hd44780_set_cursor(uint8_t row, uint8_t column);

/**
 * @brief Function for printing a char.
 * @param[in] value Is the char value.
 * @return void.
 */
void hd44780_print_char(uint8_t value);

/**
 * @brief Function for printing a string.
 * @param[in] msg Is a pointer to first char of the string for printing.
 * @return void.
 */
void hd44780_print_string(char* msg);

/**
 * @brief Function for performing a printf in the lcd as output.
 * @param[in] fmt Is a pointer to the string for printing.
 * @return void.
 */
void hd44780_printf(const char* fmt, ...);

/**
 * @brief Function for initializing character genrator CGRAM with custom characters data.
 * @param[in] tab Is a table containing characters definition values.
 * @param[in] charnum Is the number of character defined in the table.
 * @return void.
 */
void hd44780_load_cgram(char tab[], uint8_t charnum);

#endif
