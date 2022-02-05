/********************************************************************************************************//**
* @file print_lcd.c
*
* @brief Application with some examples for printing in an hd44780 LCD.
*/

#include <time.h>
#include "lcd_hd44780.h"

/***********************************************************************************************************/
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for printing time and date in an hd44780 LCD.
 * @return void.
 */
void print_time(void);

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(int argc, char* argv[]){

    hd44780_init();

    while(1){
        print_time();
    }

    return 0;
}

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

void print_time(void){

    time_t time_val;
    struct tm* timeinfo;

    time(&time_val);
    timeinfo = localtime(&time_val);

    hd44780_send_cmd(DDRAM_FST_LN_BASE_ADDR);
    hd44780_printf("%d-%d-%d", 1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday);
    hd44780_send_cmd(DDRAM_SND_LN_BASE_ADDR);
    hd44780_printf("%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}
