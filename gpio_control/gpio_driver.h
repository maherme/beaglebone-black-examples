/********************************************************************************************************//**
* @file gpio_driver.h
*
* @brief Header file containing the prototypes fo the APIs for controlling the gpio.
*
* Public Functions:
*       - int gpio_export(uint8_t gpio_no)
*       - int gpio_config_dir(uint8_t gpio_no, uint8_t dir_val)
*       - int gpio_write_value(uint8_t gpio_no, uint8_t out_val)
*       - int gpio_read_value(uint8_t gpio_no)
*       - int gpio_config_edge(uint8_t gpio_no, char* edge)
*/

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

#define SYS_FS_GPIO_PATH    "/sys/class/gpio"

/**
 * @defgroup GPIO_DIR Possible configuration values for direction of GPIOs.
 * @{
 */
#define GPIO_DIR_OUT            1   /**< @brief Value for configuring the direction as output */
#define GPIO_DIR_IN             0   /**< @brief Value for configuring the direction as input */
/** @} */

/**
 * @defgroup GPIO_VALUE Possible output values of GPIOs.
 * @{
 */
#define GPIO_HIGH_VALUE         1   /**< @brief Value for setting the output of the GPIO as high.*/
#define GPIO_LOW_VALUE          0   /**< @brief Value for setting the output of the GPIO as low */
/** @} */

/***********************************************************************************************************/
/*                                       APIs Supported                                                    */
/***********************************************************************************************************/

/**
 * @brief Function for exporting a gpio number.
 * @param[in] gpio_no Is the gpio number for exporting.
 * @return 0 is success.
 * @return != 0 if fail.
 */
int gpio_export(uint8_t gpio_no);

/**
 * @brief Function for setting the direction of a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @param[in] dir_val Is the direction (1 is output and 0 is input).
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_config_dir(uint8_t gpio_no, uint8_t dir_val);

/**
 * @brief Function for setting an output value to a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @param[in] out_val Is the value to be set as output.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_write_value(uint8_t gpio_no, uint8_t out_val);

/**
 * @brief Function for reading an input value to a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_read_value(uint8_t gpio_no);

/**
 * @brief Function for configuring the edge of a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @param[in] edge Is the config value: "rising", "falling" or "none".
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_config_edge(uint8_t gpio_no, char* edge);

#endif
