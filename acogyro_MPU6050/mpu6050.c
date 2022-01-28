/********************************************************************************************************//**
* @file mpu6050.c
*
* @brief File containing the APIs for controlling the MPU6050 module.
*
* Public Functions:
*       - int mpu6050_write(uint8_t addr, uint8_t data)
*       - int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len)
*       - void mpu6050_init(void)
*       - void mpu6050_read_aco(short int* buf)
*       - void mpu6050_read_gyr(short* buf)
*
* @note
*       For further information about functions refer to the corresponding header file.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "mpu6050.h"

/***********************************************************************************************************/
/*                                       Static Variables                                                  */
/***********************************************************************************************************/

/** @brief variable for storing the used file descriptor */
static int fd = 0;

/***********************************************************************************************************/
/*                                       Public APIs Definitions                                           */
/***********************************************************************************************************/

int mpu6050_write(uint8_t addr, uint8_t data){

    char buf[2] = {0};

    buf[0] = addr;
    buf[1] = data;

    if(write(fd, buf, 2) <= 0){
        perror("mpu6050_write API failed in write process\n");
        return -1;
    }

    return 0;
}

int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len){

    char buf[2] = {0};

    buf[0] = addr;

    if(write(fd, buf, 1) <= 0){
        perror("mpu6050_read API failed in write process\n");
        return -1;
    }

    if(read(fd, pbuf, len) <= 0){
        perror("mpu6050_read API failed in read process\n");
        return -1;
    }

    return 0;
}

void mpu6050_init(void){

    if((fd = open(SYS_FS_I2C_DEVICE, O_RDWR)) < 0){
        perror("Failed opening I2C device file\n");
        exit(EXIT_FAILURE);
    }

    if(ioctl(fd, I2C_SLAVE, MPU6050_SLAVE_ADDR) < 0){
        perror("Failed setting I2C slave address\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Disable sleep mode of mpu6050 */
    mpu6050_write(MPU6050_REG_POWER, 0x00);
    usleep(500);

    /* Adjust full scale value for accelerometer */
    mpu6050_write(MPU6050_REG_ACO_CFG, 0x00);
    usleep(500);

    /* Adjust full scale value for gyroscope */
    mpu6050_write(MPU6050_REG_GYR_CFG, 0x00);
    usleep(500);
}

void mpu6050_read_aco(short int* buf){

    /* Each axis value is coded into 2 bytes */
    char values[6] = {0};

    mpu6050_read(MPU6050_REG_ACO_X_HIGH, values, 6);

    buf[0] = (int)((values[0] << 8) | values[1]);
    buf[1] = (int)((values[2] << 8) | values[3]);
    buf[2] = (int)((values[4] << 8) | values[5]);
}

void mpu6050_read_gyr(short* buf){

    /* Each axis value is coded into 2 bytes */
    char values[6] = {0};

    mpu6050_read(MPU6050_REG_GYR_X_HIGH, values, 6);

    buf[0] = (values[0] << 8) + values[1];
    buf[1] = (values[2] << 8) + values[3];
    buf[2] = (values[4] << 8) + values[5];
}
