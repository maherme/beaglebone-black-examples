/********************************************************************************************************//**
* @file button_7seg.c
*
* @brief Application for controlling a 7 segment display using a push button.
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
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

/** @brief GPIO connected to the push button */
#define GPIO_49_P9_23_BUTTON    49

/**
 * @defgroup SEGMENT_ON Possible values for settin a segment of the display as ON/OFF.
 * @{
 */
#define SEGMENT_ON              1   /**< @brief Value for setting a segment to ON */
#define SEGMENT_OFF             0   /**< @brief Value for setting a segment to OFF */
/** @} */

/** @brief Timeout of the poll process for detecting button press */
#define POLL_TIMEOUT            3000 /* In miliseconds */

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

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(int argc, char* argv[]){

    int fd = 0;
    int timeout = POLL_TIMEOUT;
    struct pollfd fdset[2];
    uint8_t counter = 0;
    uint8_t buffer[100] = {0};

    if(ini_all_gpio() < 0){
        printf("Error: GPIO init failed\n");
    }
    else{
        printf("Push button for counting...\n");

        /* Open file of the button GPIO */
        fd = open("/sys/class/gpio/gpio49/value", O_RDONLY | O_NONBLOCK );
        if(fd <= 0){
            perror("Error, filer for reading button GPIO could not be opened");
            exit(EXIT_FAILURE);
        }
    }

    while(1){
        /* Initialize fdset struct */
        memset((void*)fdset, 0, sizeof(fdset));
        fdset[1].fd = fd;
        fdset[1].events = POLLPRI;

        /* Waiting until button is pressed */
        poll(fdset, 2, timeout);

        if(fdset[1].revents & POLLPRI){
            lseek(fdset[1].fd, 0, SEEK_SET);
            read(fdset[1].fd, buffer, 100);
            write_7seg(counter);
            counter++;
        }
    }

    return 0;
}

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
    if(gpio_export(GPIO_49_P9_23_BUTTON)){return 1;}

    /* Configure the direction of all required GPIOs */
    if(gpio_config_dir(GPIO_66_P8_7_SEGA, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_67_P8_8_SEGB, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_69_P8_9_SEGC, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_68_P8_10_DP, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_45_P8_11_SEGD, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_44_P8_12_SEGE, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_26_P8_14_SEGF, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_46_P8_16_SEGG, GPIO_DIR_OUT)){return 1;}
    if(gpio_config_dir(GPIO_49_P9_23_BUTTON, GPIO_DIR_IN)){return 1;}

    /* Initialize to low the value of all required GPIOs */
    if(gpio_write_value(GPIO_66_P8_7_SEGA, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_67_P8_8_SEGB, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_69_P8_9_SEGC, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_68_P8_10_DP, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_45_P8_11_SEGD, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_44_P8_12_SEGE, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_26_P8_14_SEGF, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_46_P8_16_SEGG, GPIO_LOW_VALUE)){return 1;}
    if(gpio_write_value(GPIO_49_P9_23_BUTTON, GPIO_LOW_VALUE)){return 1;}

    /* Set edge detection for button pin to rising */
    if(gpio_config_edge(GPIO_49_P9_23_BUTTON, "rising")){return 1;}

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
