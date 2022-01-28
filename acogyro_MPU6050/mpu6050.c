/********************************************************************************************************//**
* @file mpu6050.c
*
* @brief File containing the APIs for controlling the MPU6050 module.
*
* Public Functions:
*       - void mpu6050_init(void)
*       - void mpu6050_set_aco_scale(aco_scale_t aco_scale)
*       - void mpu6050_set_gyr_scale(gyr_scale_t gyr_scale)
*       - void mpu6050_read_aco(short int* buf)
*       - void mpu6050_read_gyr(short* buf)
*       - double mpu6050_read_temp(void)
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
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for writing an 8bit data to the mpu6050 at the indicated address.
 * @param[in] addr Is the mpu6050 address where the data will be written.
 * @param[in] data Is the 8bit data to be written in the indicated address.
 * @return 0 if success.
 * @return != 0 if fail.
 */
static int mpu6050_write(uint8_t addr, uint8_t data);

/**
 * @brief Function for reading data of the mpu6050 from an indicated address.
 * @param[in] addr Is the address from reading data.
 * @param[out] pbuf Is a pointer to the read data.
 * @param[in] len Is the number of bytes to be read.
 * @return 0 if success.
 * @return != 0 if fail.
 */
static int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len);

/***********************************************************************************************************/
/*                                       Public APIs Definitions                                           */
/***********************************************************************************************************/

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

void mpu6050_set_aco_scale(aco_scale_t aco_scale){

    char aco_cfg = 0;

    /* Read current configuration register */
    mpu6050_read(MPU6050_REG_ACO_CFG, &aco_cfg, 1);
    /* Clear AFS_SEL bits of ACCEL_CONFIG register */
    aco_cfg &= ~(0b00011000);
    /* Set AFS_SEL bits of ACCEL_CONFIG register */
    aco_cfg |= (aco_scale << 3);
    /* Set configuration register with the selected value */
    mpu6050_write(MPU6050_REG_ACO_CFG, aco_cfg);
    usleep(500);
}

void mpu6050_set_gyr_scale(gyr_scale_t gyr_scale){

    char gyr_cfg = 0;

    /* Read current configuration register */
    mpu6050_read(MPU6050_REG_GYR_CFG, &gyr_cfg, 1);
    /* Clear FS_SEL bits of GYRO_CONFIG register */
    gyr_cfg &= ~(0b00011000);
    /* Set FS_SEL bits of GYRO_CONFIG register */
    gyr_cfg |= (gyr_scale << 3);
    /* Set configuration register with the selected value */
    mpu6050_write(MPU6050_REG_GYR_CFG, gyr_cfg);
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

double mpu6050_read_temp(void){

    double ret = 0;
    int16_t raw_temp = 0;
    char values[2] = {0};   /* Temperature value is coded into 2 bytes */

    mpu6050_read(MPU6050_REG_TEMP_HIGH, values, 2);

    raw_temp = (((int16_t)values[0]) << 8) | values[1];
    ret = (double)raw_temp/340 + 36.53;

    return ret;
}

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

static int mpu6050_write(uint8_t addr, uint8_t data){

    char buf[2] = {0};

    buf[0] = addr;
    buf[1] = data;

    if(write(fd, buf, 2) <= 0){
        perror("mpu6050_write API failed in write process\n");
        return -1;
    }

    return 0;
}

static int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len){

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
