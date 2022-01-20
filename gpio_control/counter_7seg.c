/********************************************************************************************************//**
* @file counter_7seg.c
*
* @brief Application for controlling an 7 segment display.
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "gpio_driver.h"

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

/**
 * @defgroup GPIO_7SEG GPIO connected to the 7 segment display.
 * @{
 */
#define GPIO_66_P8_7_SEGA       66  /**< @brief GPIO regarding segment A */
#define GPIO_67_P8_8_SEGB       67  /**< @brief GPIO regarding segment B */
#define GPIO_69_P8_9_SEGC       69  /**< @brief GPIO regarding segment C */
#define GPIO_68_P8_10_DP        68  /**< @brief GPIO regarding decimal point */
#define GPIO_45_P8_11_SEGD      45  /**< @brief GPIO regarding segment D */
#define GPIO_44_P8_12_SEGE      44  /**< @brief GPIO regarding segment E */
#define GPIO_26_P8_14_SEGF      26  /**< @brief GPIO regarding segment F */
#define GPIO_46_P8_16_SEGG      46  /**< @brief GPIO regarding segment G */
/** @} */

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

/**
 * @defgroup SEGMENT_ON Possible values for settin a segment of the display as ON/OFF.
 * @{
 */
#define SEGMENT_ON              1   /**< @brief Value for setting a segment to ON */
#define SEGMENT_OFF             0   /**< @brief Value for setting a segment to OFF */
/** @} */

/***********************************************************************************************************/
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for initializing needed gpios.
 * @return 0 if success.
 * @return != 0 if fail.
 */
static int ini_all_gpio(void);

/**
 * @brief Function for set a number in the display.
 * @param[in] number_dis Is the number to be set.
 * @return void.
 */
static void write_7seg(uint8_t number_dis);

/**
 * @brief Function for setting an upcounting process in the display.
 * @param[in] delay_ms Is a delay in ms for refreshing the counter.
 * @return void.
 */
static void start_upcounting(int delay_ms);

/**
 * @brief Function for setting a downcounting process in the display.
 * @param[in] delay_ms Is a delay in ms for refreshing the counter.
 * @return void.
 */
static void start_downcounting(int delay_ms);

/**
 * @brief Function for setting an upcounting and downcounting process in the display.
 * @param[in] delay_ms Is a delay in ms for refreshing the counter.
 * @return void.
 */
static void start_updowncounting(int delay_ms);

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

static int ini_all_gpio(void){

    /* Export all required GPIOs */
    if(gpio_export(GPIO_66_P8_7_SEGA)){return 1;}
    if(gpio_export(GPIO_67_P8_8_SEGB)){return 1;}
    if(gpio_export(GPIO_69_P8_9_SEGC)){return 1;}
    if(gpio_export(GPIO_68_P8_10_DP)){return 1;}
    if(gpio_export(GPIO_45_P8_11_SEGD)){return 1;}
    if(gpio_export(GPIO_44_P8_12_SEGE)){return 1;}
    if(gpio_export(GPIO_26_P8_14_SEGF)){return 1;}
    if(gpio_export(GPIO_46_P8_16_SEGG)){return 1;}

    /* Configure the direction of all required GPIOs */
    if(gpio_config_dir(GPIO_66_P8_7_SEGA, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_67_P8_8_SEGB, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_69_P8_9_SEGC, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_68_P8_10_DP, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_45_P8_11_SEGD, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_44_P8_12_SEGE, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_26_P8_14_SEGF, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_46_P8_16_SEGG, GPIO_DIR_OUT)){return 1;}

    /* Initialize to low the value of all required GPIOs */
    if(gpio_write_value(GPIO_66_P8_7_SEGA, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_67_P8_8_SEGB, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_69_P8_9_SEGC, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_68_P8_10_DP, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_45_P8_11_SEGD, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_44_P8_12_SEGE, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_26_P8_14_SEGF, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_46_P8_16_SEGG, GPIO_LOW_VALUE)){return 1;}

    return 0;
}

static void write_7seg(uint8_t number_dis){

    switch(number_dis){
        case 0:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
            break;
        case 1:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_OFF);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
            break;
        case 2:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_OFF);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 3:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 4:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_OFF);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 5:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_OFF);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 6:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_OFF);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 7:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_OFF);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_OFF);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_OFF);
            break;
        case 8:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_ON);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        case 9:
            gpio_write_value(GPIO_66_P8_7_SEGA, SEGMENT_ON);
            gpio_write_value(GPIO_67_P8_8_SEGB, SEGMENT_ON);
            gpio_write_value(GPIO_69_P8_9_SEGC, SEGMENT_ON);
            gpio_write_value(GPIO_45_P8_11_SEGD, SEGMENT_ON);
            gpio_write_value(GPIO_44_P8_12_SEGE, SEGMENT_OFF);
            gpio_write_value(GPIO_26_P8_14_SEGF, SEGMENT_ON);
            gpio_write_value(GPIO_46_P8_16_SEGG, SEGMENT_ON);
            break;
        default:
            break;
    }
}

static void start_upcounting(int delay_ms){

    uint8_t i = 0;

    if(ini_all_gpio() < 0){
        printf("Error: GPIO init failed\n");
    }
    else{
        printf("Up counting...\n");
        while(1){
            for(i = 0; i < 10; i++){
                write_7seg(i);
                usleep(delay_ms * 1000);
            }
        }
    }
}

static void start_downcounting(int delay_ms){

    uint8_t i = 0;

    if(ini_all_gpio() < 0){
        printf("Error: GPIO init failed\n");
    }
    else{
        printf("Down counting...\n");
        while(1){
            for(i = 0; i < 10; i++){
                write_7seg(9-i);
                usleep(delay_ms * 1000);
            }
        }
    }
}

static void start_updowncounting(int delay_ms){

    uint8_t i = 0;

    if(ini_all_gpio() < 0){
        printf("Error: GPIO init failed\n");
    }
    else{
        printf("Up and down counting...\n");
        while(1){
            for(i = 0; i < 10; i++){
                write_7seg(i);
                usleep(delay_ms * 1000);
            }
            for(i = 1; i < 9; i++){
                write_7seg(9-i);
                usleep(delay_ms * 1000);
            }
        }
    }
}

static void start_randomcounting(int delay_ms){

    uint8_t i = 0;

    if(ini_all_gpio() < 0){
        printf("Error: GPIO init failed\n");
    }
    else{
        printf("Random counting...\n");
        while(1){
            i = rand() % 10;
            write_7seg(i);
            usleep(delay_ms * 1000);
        }
    }
}

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(int argc, char* argv[]){

    int delay_value = 0;

    printf("Application for up/down/random counter on 7 segment display\n");

    /* Check the right number of arguments */
    if(argc!= 3){
        printf("Usage: %s <direction> <delay>\n", argv[0]);
        printf("Valid direction: up, down, updown, random\n");
        printf("Recommended delay range in ms: 0 to 1000\n");
    }
    else{
        delay_value = atoi(argv[2]);

        if(!strcmp(argv[1], "up")){
            start_upcounting(delay_value);
        }
        else if(!strcmp(argv[1], "down")){
            start_downcounting(delay_value);
        }
        else if(!strcmp(argv[1], "updown")){
            start_updowncounting(delay_value);
        }
        else if(!strcmp(argv[1], "random")){
            start_randomcounting(delay_value);
        }
        else{
            printf("Invalid direction values\n");
            printf("Valid direction values: up, down, updown, random\n");
        }
    }

    return 0;
}
