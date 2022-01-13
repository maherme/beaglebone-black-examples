/********************************************************************************************************//**
* @file led_user_control.c
*
* @brief Application for controlling the user led.
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

/** @brief Path to the file system for controlling the user leds */
#define SYS_FS_LEDS_PATH "/sys/class/leds"

/***********************************************************************************************************/
/*                                       Global Variables                                                  */
/***********************************************************************************************************/

/** @brief Variable for storing the number of the selected user led (argv[1])*/
static uint8_t led_number = 0;

/***********************************************************************************************************/
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for processing the introduced option for trigger.
 * @param[in] value Is a string with the selected option for trigger.
 * @return 0 if success.
 * @return 1 if fail.
 */
static uint8_t process_trigger(char* value);

/**
 * @brief Function for writing the option for trigger into the file system.
 * @param[in] led_no Is the number of the selected user led.
 * @param[in] value Is a string with the selected option for trigger.
 * @return 0 if success.
 * @return 1 if fail.
 */
static uint8_t write_trigger_value(uint8_t led_no, char* value);

/**
 * @brief Function for processing the introduced value for brightness.
 * @param[in] value Is an integer with the value for brightness.
 * @return 0 if success.
 * @return 1 if fail.
 */
static uint8_t process_brightness(int value);

/**
 * @brief Function for writing the value for brightness into the file system.
 * @param[in] led_no Is the number of the selected user led.
 * @param[in] value Is an integer with the value for brightness.
 * @return 0 if success.
 * @return 1 if fail.
 */
static uint8_t write_brightness_value(uint8_t led_no, int value);

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(int argc, char* argv[]){

    int value = 0;

    printf("Application for controlling the user LEDs\n");

    /* Check the right number of arguments */
    if(argc != 4){
        printf("Usage: %s <usr led number> <control option> <value>\n", argv[0]);
        printf("Valid <usr led number> : 0, 1, 2 or 3\n");
        printf("Valid <control option> : brightness, trigger\n");
        printf("Valid brightness values : 0 or 1\n");
        printf("Valid trigger values : heartbeat, timer, none, oneshot, default-on\n");
        return 1;
    }
    else{
        /* Store the led number */
        led_number = atoi(argv[1]);
        /* Check if led number is valid */
        if(led_number > 3){
            printf("Invalid usr led number\n");
            printf("Valid <usr led number> : 0, 1, 2 or 3\n");
            return 1;
        }

        if(strcmp(argv[2], "trigger") == 0){
            if(process_trigger(argv[3])){
                return 1;
            }
        }
        else if(strcmp(argv[2], "brightness") == 0){
            value = atoi(argv[3]);
            if(process_brightness(value)){
                return 1;
            }
        }
        else{
            printf("Invalid control option\n");
            printf("Valid <control option> : brightness, trigger\n");
            return 1;
        }
    }

    return 0;
}

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

static uint8_t process_trigger(char* value){

    /* Check valid trigger value */
    if(strcmp(value, "timer") && strcmp(value, "heartbeat") && strcmp(value, "none") &&
       strcmp(value, "oneshot") && strcmp(value, "default-on")){
        printf("Invalid trigger value\n");
        printf("Valid trigger values : heartbeat, timer, none, oneshot, default-on\n");
        return 1;
    }
    else{
        write_trigger_value(led_number, value);
    }

    return 0;
}

static uint8_t write_trigger_value(uint8_t led_no, char* value){

    int fd = 0;
    char buf[100] = {0};

    /* Concatenate two strings and store in to buf */
    snprintf(buf, sizeof(buf), SYS_FS_LEDS_PATH "/beaglebone:green:usr%d/trigger", led_no);

    fd = open(buf, O_WRONLY);
    if(fd <= 0){
        perror("Error, file for managing user led could not be opened");
        exit(EXIT_FAILURE);
    }

    if(write(fd, (void*)value, strlen(value)) <= 0){
        close(fd);
        perror("Error, failed writing trigger value");
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}

static uint8_t process_brightness(int value){

    if((value != 0) && (value != 1)){
        printf("Invalid brightness value\n");
        printf("Valid brightness values : 0 or 1\n");
        return 1;
    }
    else{
        write_brightness_value(led_number, value);
    }

    return 0;
}

static uint8_t write_brightness_value(uint8_t led_no, int value){

    int fd = 0;
    char buf[100] = {0};

    /* Concatenate two strings and store in to buf */
    snprintf(buf, sizeof(buf), SYS_FS_LEDS_PATH "/beaglebone:green:usr%d/brightness", led_no);

    fd = open(buf, O_WRONLY);
    if(fd <= 0){
        perror("Error, file for managing user led could not be opened");
        exit(EXIT_FAILURE);
    }

    if(write(fd, (void*)&value, sizeof(value)) <= 0){
        close(fd);
        perror("Error, failed writing brightness value");
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}
