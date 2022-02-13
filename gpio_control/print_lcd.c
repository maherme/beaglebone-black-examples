/********************************************************************************************************//**
* @file print_lcd.c
*
* @brief Application with some examples for printing in an hd44780 LCD.
*/

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "lcd_hd44780.h"

/***********************************************************************************************************/
/*                                       Static Function Prototypes                                        */
/***********************************************************************************************************/

/**
 * @brief Function for printing time and date in an hd44780 LCD.
 * @return void.
 */
static void print_time(void);

/**
 * @brief Function for printing ip address in an hd44780 LCD.
 * @return void.
 */
static void print_ip(void);

/**
 * @brief Function for transitioning between printinf info.
 * @return void.
 */
static void print_transition(void);

/***********************************************************************************************************/
/*                                       Main Function                                                     */
/***********************************************************************************************************/

int main(int argc, char* argv[]){

    hd44780_init();

    while(1){
        print_transition();
        print_time();
        print_transition();
        print_ip();
    }

    return 0;
}

/***********************************************************************************************************/
/*                                       Static Function Definitions                                       */
/***********************************************************************************************************/

static void print_time(void){

    time_t time_val;
    struct tm* timeinfo;

    time(&time_val);
    timeinfo = localtime(&time_val);

    hd44780_send_cmd(DDRAM_FST_LN_BASE_ADDR);
    hd44780_printf("%04d-%02d-%02d", 1900 + timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday);
    hd44780_send_cmd(DDRAM_SND_LN_BASE_ADDR);
    hd44780_printf("%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

static void print_ip(void){

    int fd = 0;
    struct ifreq ifr;
    char iface[] = "usb0";

    /* Create an IPv4 endpoint */
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Type of address to retrieve - IPv4 */
    ifr.ifr_addr.sa_family = AF_INET;

    /* Copy the inferface name */
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* Display result */
    hd44780_send_cmd(DDRAM_FST_LN_BASE_ADDR);
    hd44780_print_string((char*)iface);
    hd44780_send_cmd(DDRAM_SND_LN_BASE_ADDR);
    hd44780_print_string((char*)inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
}

static void print_transition(void){

    sleep(1);
    hd44780_set_cursor(1, 1);
    hd44780_send_cmd(HD44780_CMD_CLEAR_DISP);

}
