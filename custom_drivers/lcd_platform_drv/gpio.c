#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include "gpio.h"
#include "lcd_platform_drv.h"

int gpio_configure_dir(u8 desc_id, u8 dir_value, struct device* dev)
{
    int ret;

    struct lcd_device_private_data* lcd_data = dev_get_drvdata(dev);

    if(dir_value)
    {
        ret = gpiod_direction_output(lcd_data->desc[desc_id], 0);
    }
    else
    {
        ret = gpiod_direction_input(lcd_data->desc[desc_id]);
    }

    return ret;
}

void gpio_write_value(u8 desc_id, u8 out_value, struct device* dev)
{
    struct lcd_device_private_data* lcd_data = dev_get_drvdata(dev);

    gpiod_set_value(lcd_data->desc[desc_id], out_value);
}
