/********************************************************************************************************//**
* @file mpu6050.h
*
* @brief Header file containing the prototypes of the APIs for MPU6050 module.
*
* Public Functions:
*       - void mpu6050_init(void)
*       - void mpu6050_set_aco_scale(aco_scale_t aco_scale)
*       - void mpu6050_set_gyr_scale(gyr_scale_t gyr_scale)
*       - void mpu6050_read_aco(short int* buf)
*       - void mpu6050_read_gyr(short* buf)
*       - double mpu6050_read_temp(void)
*/

#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

/***********************************************************************************************************/
/*                                       Defines and Macros                                                */
/***********************************************************************************************************/

/**
 * @defgroup MPU6050_CFG Configuration registers for MPU6050 sensor.
 * @{
 */
#define MPU6050_REG_POWER           0x6B    /**< @brief Power register */
#define MPU6050_REG_ACO_CFG         0x1C    /**< @brief Accelerometer configuration register */
#define MPU6050_REG_GYR_CFG         0x1B    /**< @brief Gyroscope configuration register */
/** @} */

/**
 * @defgroup MPU6050_ACO_REG Registers for reading accelerometer values for MPU6050 sensor.
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
 * @defgroup MPU6050_TEMP_REG Registers for reading temperature values from MPU6050 sensor.
 * @{
 */
#define MPU6050_REG_TEMP_HIGH       0x41
#define MPU6050_REG_TEMP_LOW        0x42
/** @} */

/**
 * @defgroup MPU6050_GYR_REG Registers for reading gyroscope values for MPU6050 sensor.
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
/*                                       Structs and Enums                                                 */
/***********************************************************************************************************/

/** @brief Possible values for accelerometer scale. */
typedef enum{
    ACO_SCALE_2G    = 0x00,     /**< @brief 2g scale */
    ACO_SCALE_4G    = 0x01,     /**< @brief 4g scale */
    ACO_SCALE_8G    = 0x02,     /**< @brief 8g scale */
    ACO_SCALE_16G   = 0x03      /**< @brief 16g scale */
}aco_scale_t;

/** @brief Possible values for gyroscope scale */
typedef enum{
    GYR_SCALE_250   = 0x00,     /**< @brief 250 deg/sec scale */
    GYR_SCALE_500   = 0x01,     /**< @brief 500 deg/sec scale */
    GYR_SCALE_1000  = 0x02,     /**< @brief 1000 deg/sec scale */
    GYR_SCALE_2000  = 0x03      /**< @brief 2000 deg/sec scale */
}gyr_scale_t;

/***********************************************************************************************************/
/*                                       APIs Supported                                                    */
/***********************************************************************************************************/

/**
 * @brief Function for initializing the mpu6050 sensor.
 * @return void.
 */
void mpu6050_init(void);

/**
 * @brief Function for setting the scale of the accelerometer.
 * @param[in] aco_scale Is the selected scale, possible values from aco_scale_t.
 * @return void.
 */
void mpu6050_set_aco_scale(aco_scale_t aco_scale);

/**
 * @brief Function for setting the scale of the gyroscope.
 * @param[in] aco_scale Is the selected scale, possible values from gyr_scale_t.
 * @return void.
 */
void mpu6050_set_gyr_scale(gyr_scale_t gyr_scale);

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

/**
 * @brief Function for reading the temperature value.
 * @return Read temperature in celsius degree.
 */
double mpu6050_read_temp(void);

#endif
