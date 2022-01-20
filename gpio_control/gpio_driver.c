/********************************************************************************************************//**
* @file gpio_driver.c
*
* @brief Functions for controlling the gpio.
*
* Public Functions:
*       - int gpio_export(uint8_t gpio_no)
*       - int gpio_config_dir(uint8_t gpio_no, uint8_t dir_val)
*       - int gpio_write_value(uint8_t gpio_no, uint8_t out_val)
*       - int gpio_read_value(uint8_t gpio_no)
*       - int gpio_config_edge(uint8_t gpio_no, char* edge)
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpio_driver.h"

/***********************************************************************************************************/
/*                                       Public API Definitions                                            */
/***********************************************************************************************************/

int gpio_export(uint8_t gpio_no){

    int fd = 0;
    int len = 0;
    char buf[4] = {0};

    fd = open(SYS_FS_GPIO_PATH "/export", O_WRONLY);
    if(fd < 0){
        perror("Error, filer for exporting gpio could not be opened");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio_no);
    write(fd, buf, len);

    close(fd);

    return 0;
}

/**
 * @brief Function for setting the direction of a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @param[in] dir_val Is the direction (1 is output and 0 is input).
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_config_dir(uint8_t gpio_no, uint8_t dir_val){

    int fd = 0;
    char buf[100] = {0};

    snprintf(buf, sizeof(buf), SYS_FS_GPIO_PATH "/gpio%d/direction", gpio_no);

    fd = open(buf, O_WRONLY);
    if(fd < 0){
        perror("Error, file for managing gpio could not be opened");
        return fd;
    }

    if(dir_val){
        write(fd, "out", 4);
    }
    else{
        write(fd, "in", 3);
    }

    close(fd);

    return 0;
}

/**
 * @brief Function for setting an output value to a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @param[in] out_val Is the value to be set as output.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_write_value(uint8_t gpio_no, uint8_t out_val){

    int fd = 0;
    char buf[100] = {0};

    snprintf(buf, sizeof(buf), SYS_FS_GPIO_PATH "/gpio%d/value", gpio_no);

    fd = open(buf, O_WRONLY);
    if(fd < 0){
        perror("Error, file for managing gpio could not be opened");
        return fd;
    }

    if(out_val){
        write(fd, "1", 2);
    }
    else{
        write(fd, "0", 2);
    }

    close(fd);

    return 0;
}

/**
 * @brief Function for reading an input value to a gpio number.
 * @param[in] gpio_no Is the gpio number for configuring.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int gpio_read_value(uint8_t gpio_no){

    int fd = 0;
    uint8_t read_val = 0;
    char buf[100] = {0};

    snprintf(buf, sizeof(buf), SYS_FS_GPIO_PATH "/gpio%d/value", gpio_no);

    fd = open(buf, O_WRONLY);
    if(fd < 0){
        perror("Error, file for managing gpio could not be opened");
        return fd;
    }

    read(fd, &read_val, 1);

    close(fd);

    return read_val;
}

int gpio_config_edge(uint8_t gpio_no, char* edge){

    int fd = 0;
    char buf[100] = {0};

    snprintf(buf, sizeof(buf), SYS_FS_GPIO_PATH "/gpio%d/edge", gpio_no);

    fd = open(buf, O_WRONLY);
    if(fd < 0){
        perror("Error, file for managing gpio could not be opened");
        return fd;
    }

    write(fd, edge, strlen(edge) + 1);

    close(fd);

    return 0;
}
