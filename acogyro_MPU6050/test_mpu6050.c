/********************************************************************************************************//**
* @file test_mpu6050.c
*
* @brief File for testing the BSP of mpu6050 device.
*/

#include <stdio.h>
#include <unistd.h>
#include "mpu6050.h"

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(void){

    short int aco_value[3] = {0};
    short gyr_value[3] = {0};
    double aco_x, aco_y, aco_z;
    double gyr_x, gyr_y, gyr_z;
    double temp = 0;

    mpu6050_init();

    while(1){
        mpu6050_read_aco(aco_value);
        mpu6050_read_gyr(gyr_value);
        temp = mpu6050_read_temp();

        /* Convert raw values from accelerometer in to g values */
        aco_x = (double)aco_value[0]/ACO_FS_SENS_3;
        aco_y = (double)aco_value[1]/ACO_FS_SENS_3;
        aco_z = (double)aco_value[2]/ACO_FS_SENS_3;

        /* Convert raw values for gyroscope in to "deg/seconds" values */
        gyr_x = (double)gyr_value[0]/GYR_FS_SENS_0;
        gyr_y = (double)gyr_value[1]/GYR_FS_SENS_0;
        gyr_z = (double)gyr_value[2]/GYR_FS_SENS_0;

        /* Print raw values */
        printf("Aco(raw)-> X:%d Y:%d Z:%d Gyr(raw)-> X:%d Y:%d Z:%d\n",
                aco_value[0], aco_value[1], aco_value[2], gyr_value[0], gyr_value[1], gyr_value[2]);

        /* Print converted values */
        printf("Aco(g)-> X:%.2f Y:%.2f Z:%.2f Gyr(d/s)-> X:%.2f Y:%.2f Z:%.2f Temp(C)-> %.2f\n",
                aco_x, aco_y, aco_z, gyr_x, gyr_y, gyr_z, temp);

        /* Wait 1s before next iteration */
        sleep(1);
    }

    return 0;
}
