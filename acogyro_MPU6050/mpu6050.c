/********************************************************************************************************//**
* @file mpu6050.c
*
* @brief Application for getting values of MPU6050 accelerometer and gyroscope.
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

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

/**
 * @defgroup MPU6050_CFG Configuration register for MPU6050 sensor.
 * @{
 */
#define MPU6050_REG_POWER           0x6B    /**< @brief Power register */
#define MPU6050_REG_ACO_CFG         0x1C    /**< @brief Accelerometer configuration register */
#define MPU6050_REG_GYR_CFG         0x1B    /**< @brief Gyroscope configuration register */
/** @} */

/**
 * @defgropu MPU6050_ACO_REG Register for reading accelerometer read values for MPU6050 sensor.
 * @{
 */
#define MPU6050_REG_ACO_X_HIGH      0x3B
#define MPU6050_REG_ACO_X_LOW       0x3C
#define MPU6050_REG_ACO_Y_HIGH      0x3D
#define MPU6050_REG_ACO_Y_LOW       0x3E
#define MPU6050_REG_ACO_Z_HIGH      0x3F
#define MPU6050_REG_ACO_Z_LOW       0x40
/** @} */

/**
 * @defgropu MPU6050_GYR_REG Register for reading gyroscope read values for MPU6050 sensor.
 * @{
 */
#define MPU6050_REG_GYR_X_HIGH      0x43
#define MPU6050_REG_GYR_X_LOW       0x44
#define MPU6050_REG_GYR_Y_HIGH      0x45
#define MPU6050_REG_GYR_Y_LOW       0x46
#define MPU6050_REG_GYR_Z_HIGH      0x47
#define MPU6050_REG_GYR_Z_LOW       0x48
/** @} */

/**
 * @defgroup MPU6050_ACO_SENS Sensivity values for full scale ranges of accelerometer for MPU6050 sensor.
 * @{
 */
#define ACO_FS_SENS_0               16384
#define ACO_FS_SENS_1               8192
#define ACO_FS_SENS_2               4096
#define ACO_FS_SENS_3               2048
/** @} */

/**
 * @defgroup MPU6050_GYR_SENS Sensivity values for full scale ranges of gyroscope for MPU6050 sensor.
 * @{
 */
#define GYR_FS_SENS_0               131
#define GYR_FS_SENS_1               65.5
#define GYR_FS_SENS_2               32.8
#define GYR_FS_SENS_3               16.4
/** @} */

/** @brief The I2C slave address of the MPU6050 sensor */
#define MPU6050_SLAVE_ADDR          0x68

/** @brief Linux OS device file for the I2C3 controller of the SOC */
#define SYS_FS_I2C_DEVICE           "/dev/i2c-2"

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

/**
 * @brief Function for initializing the mpu6050 sensor.
 * @return void.
 */
static void mpu6050_init(void);

/**
 * @brief Function for reading the accelerometer values.
 * param[out] buf Is a pointer to the read values (3 integers).
 * @return void.
 */
static void mpu6050_read_aco(short int* buf);

/**
 * @brief Function for reading the gyroscope values.
 * @param[out] buf Is a pointer to the read values (3 shorts).
 * @return void.
 */
static void mpu6050_read_gyr(short* buf);

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(void){

    short int aco_value[3] = {0};
    short gyr_value[3] = {0};
    double aco_x, aco_y, aco_z;
    double gyr_x, gyr_y, gyr_z;

    if((fd = open(SYS_FS_I2C_DEVICE, O_RDWR)) < 0){
        perror("Failed opening I2C device file\n");
        exit(EXIT_FAILURE);
    }

    if(ioctl(fd, I2C_SLAVE, MPU6050_SLAVE_ADDR) < 0){
        perror("Failed setting I2C slave address\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    mpu6050_init();

    while(1){
        mpu6050_read_aco(aco_value);
        mpu6050_read_gyr(gyr_value);

        /* Convert raw values from accelerometer in to g values */
        aco_x = (double)aco_value[0]/ACO_FS_SENS_0;
        aco_y = (double)aco_value[1]/ACO_FS_SENS_0;
        aco_z = (double)aco_value[2]/ACO_FS_SENS_0;

        /* Convert raw values for gyroscope in to "deg/seconds" values */
        gyr_x = (double)gyr_value[0]/GYR_FS_SENS_0;
        gyr_y = (double)gyr_value[1]/GYR_FS_SENS_0;
        gyr_z = (double)gyr_value[2]/GYR_FS_SENS_0;

        /* Print raw values */
        printf("Aco(raw)-> X:%d Y:%d Z:%d Gyr(raw)-> X:%d Y:%d Z:%d\n",
                aco_value[0], aco_value[1], aco_value[2], gyr_value[0], gyr_value[1], gyr_value[2]);

        /* Print converted values */
        printf("Aco(g)-> X:%.2f Y:%.2f Z:%.2f Gyr(d/s)-> X:%.2f Y:%.2f Z:%.2f\n",
                aco_x, aco_y, aco_z, gyr_x, gyr_y, gyr_z);

        /* Wait 50ms before next iteration */
        usleep(50000);
    }

    return 0;
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

static void mpu6050_init(void){

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

static void mpu6050_read_aco(short int* buf){

    /* Each axis value is coded into 2 bytes */
    char values[6] = {0};

    mpu6050_read(MPU6050_REG_ACO_X_HIGH,values, 6);

    buf[0] = (int)((values[0] << 8) | values[1]);
    buf[1] = (int)((values[2] << 8) | values[3]);
    buf[2] = (int)((values[4] << 8) | values[5]);
}

static void mpu6050_read_gyr(short* buf){

    /* Each axis value is coded into 2 bytes */
    char values[6] = {0};

    mpu6050_read(MPU6050_REG_GYR_X_HIGH, values, 6);

    buf[0] = (values[0] << 8) + values[1];
    buf[1] = (values[2] << 8) + values[3];
    buf[2] = (values[4] << 8) + values[5];
}
