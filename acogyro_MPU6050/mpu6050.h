/********************************************************************************************************//**
* @file mpu6050.h
*
* @brief Header file containing the prototypes of the APIs for MPU6050 module.
*
* Public Functions:
*       - int mpu6050_write(uint8_t addr, uint8_t data)
*       - int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len)
*       - void mpu6050_init(void)
*       - void mpu6050_read_aco(short int* buf)
*       - void mpu6050_read_gyr(short* buf)
*/

#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

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
#define ACO_FS_SENS_0               16384   /**< @brief 2g scale */
#define ACO_FS_SENS_1               8192    /**< @brief 4g scale */
#define ACO_FS_SENS_2               4096    /**< @brief 8g scale */
#define ACO_FS_SENS_3               2048    /**< @brief 16g scale */
/** @} */

/**
 * @defgroup MPU6050_GYR_SENS Sensivity values for full scale ranges of gyroscope for MPU6050 sensor.
 * @{
 */
#define GYR_FS_SENS_0               131     /**< @brief 250 deg/sec scale */
#define GYR_FS_SENS_1               65.5    /**< @brief 500 deg/sec scale */
#define GYR_FS_SENS_2               32.8    /**< @brief 1000 deg/sec scale */
#define GYR_FS_SENS_3               16.4    /**< @brief 2000 deg/sec scale */
/** @} */

/** @brief The I2C slave address of the MPU6050 sensor */
#define MPU6050_SLAVE_ADDR          0x68

/** @brief Linux OS device file for the I2C3 controller of the SOC */
#define SYS_FS_I2C_DEVICE           "/dev/i2c-2"

/***********************************************************************************************************/
/*                                       APIs Supported                                                    */
/***********************************************************************************************************/

/**
 * @brief Function for writing an 8bit data to the mpu6050 at the indicated address.
 * @param[in] addr Is the mpu6050 address where the data will be written.
 * @param[in] data Is the 8bit data to be written in the indicated address.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int mpu6050_write(uint8_t addr, uint8_t data);

/**
 * @brief Function for reading data of the mpu6050 from an indicated address.
 * @param[in] addr Is the address from reading data.
 * @param[out] pbuf Is a pointer to the read data.
 * @param[in] len Is the number of bytes to be read.
 * @return 0 if success.
 * @return != 0 if fail.
 */
int mpu6050_read(uint8_t addr, char* pbuf, uint32_t len);

/**
 * @brief Function for initializing the mpu6050 sensor.
 * @return void.
 */
void mpu6050_init(void);

/**
 * @brief Function for reading the accelerometer values.
 * param[out] buf Is a pointer to the read values (3 integers).
 * @return void.
 */
void mpu6050_read_aco(short int* buf);

/**
 * @brief Function for reading the gyroscope values.
 * @param[out] buf Is a pointer to the read values (3 shorts).
 * @return void.
 */
void mpu6050_read_gyr(short* buf);

#endif
