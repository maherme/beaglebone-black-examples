#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include "lcd_platform_drv.h"
#include "lcd.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

struct lcd_device_private_data lcd_dev_data = {
    .lcd_scroll = 0,
    .lcdxy = "(1,1)"
};

struct lcd_platform_drv_private_data lcd_drv_data;

static ssize_t lcdcmd_store(struct device *dev,
                            struct device_attribute *attr,
                            const char *buf,
                            size_t count)
{
    int status;
    long value;

    status = kstrtol(buf, 0, &value);
    if(!status)
    {
        lcd_send_command((u8)value, dev);
    }

    return status ? status : count;
}

static ssize_t lcdtext_store(struct device *dev,
                             struct device_attribute *attr,
                             const char *buf,
                             size_t count)
{
    if(buf)
    {
        dev_info(dev, "lcdtext: %s\n", buf);
        lcd_print_string((char*)buf, dev);
    }
    else
    {
        return -EINVAL;
    }

    return count;
}

static ssize_t lcdscroll_store(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf,
                               size_t count)
{
    int status = 0;
    struct lcd_device_private_data* dev_data = dev_get_drvdata(dev);

    if(sysfs_streq(buf, "on"))
    {
        dev_data->lcd_scroll = 1;
        lcd_send_command(0x18, dev);
    }
    else if(sysfs_streq(buf, "off"))
    {
        dev_data->lcd_scroll = 0;
        lcd_send_command(0x2, dev);
        lcd_send_command(0x10, dev);
    }
    else
    {
        status = -EINVAL;
    }

    return status ? status : count;
}

static ssize_t lcdscroll_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int status;
    struct lcd_device_private_data* dev_data = dev_get_drvdata(dev);

    if(dev_data->lcd_scroll)
    {
        status = sprintf(buf, "%s\n", "on");
    }
    else
    {
        status = sprintf(buf, "%s\n", "off");
    }

    return status;
}

static ssize_t lcdxy_store(struct device *dev,
                           struct device_attribute *attr,
                           const char *buf,
                           size_t count)
{
    long value;
    int status;
    int x, y;
    struct lcd_device_private_data* dev_data = dev_get_drvdata(dev);

    status = kstrtol(buf, 10, &value);
    if(status)
    {
        return status;
    }
    x = value / 10;
    y = value % 10;
    status = sprintf(dev_data->lcdxy, "(%d, %d)", x, y);
    lcd_set_cursor(x, y, dev);

    return status;
}

static ssize_t lcdxy_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int status;
    struct lcd_device_private_data* dev_data = dev_get_drvdata(dev);

    status = sprintf(buf, "%s\n", dev_data->lcdxy);

    return status;
}

static DEVICE_ATTR_WO(lcdcmd);
static DEVICE_ATTR_WO(lcdtext);
static DEVICE_ATTR_RW(lcdscroll);
static DEVICE_ATTR_RW(lcdxy);

static struct attribute* lcd_attrs[] = {
    &dev_attr_lcdcmd.attr,
    &dev_attr_lcdtext.attr,
    &dev_attr_lcdscroll.attr,
    &dev_attr_lcdxy.attr,
    NULL
};

struct attribute_group lcd_attr_group = {
    .attrs = lcd_attrs
};

static const struct attribute_group* lcd_attr_groups[] = {
    &lcd_attr_group,
    NULL
};

int device_create_files(struct device* dev)
{
    lcd_dev_data.dev = device_create_with_groups(lcd_drv_data.class_lcd,
                                                 dev,
                                                 0,
                                                 &lcd_dev_data,
                                                 lcd_attr_groups,
                                                 "LCD16x2"
                                                );

    if(IS_ERR(lcd_dev_data.dev))
    {
        dev_err(dev, "Error creating class entry\n");
        return PTR_ERR(lcd_dev_data.dev);
    }

    return 0;
}

int lcd_platform_drv_probe(struct platform_device* pdev)
{
    int ret;
    struct device* dev = &pdev->dev;

    /* Save the device private data pointer in the platform device structure */
    dev_set_drvdata(dev, &lcd_dev_data);

    /* Get the GPIO descriptor, for more info: https://www.kernel.org/doc/Documentation/gpio/board.txt */
    lcd_dev_data.desc[LCD_RS] = gpiod_get(dev, "rs", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_RW] = gpiod_get(dev, "rw", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_EN] = gpiod_get(dev, "en", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_D4] = gpiod_get(dev, "d4", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_D5] = gpiod_get(dev, "d5", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_D6] = gpiod_get(dev, "d6", GPIOD_OUT_LOW);
    lcd_dev_data.desc[LCD_D7] = gpiod_get(dev, "d7", GPIOD_OUT_LOW);

    if(IS_ERR(lcd_dev_data.desc[LCD_RS]) ||
       IS_ERR(lcd_dev_data.desc[LCD_RW]) ||
       IS_ERR(lcd_dev_data.desc[LCD_EN]) ||
       IS_ERR(lcd_dev_data.desc[LCD_D4]) ||
       IS_ERR(lcd_dev_data.desc[LCD_D5]) ||
       IS_ERR(lcd_dev_data.desc[LCD_D6]) ||
       IS_ERR(lcd_dev_data.desc[LCD_D7]))
    {
        dev_err(dev, "GPIO Error\n");
        return -EINVAL;
    }

    ret = lcd_init(dev);
    if(ret)
    {
        dev_err(dev, "LCD init failed\n");
        return ret;
    }
    dev_info(dev, "LCD init success\n");

    ret = device_create_files(dev);
    if(ret)
    {
        dev_err(dev, "LCD sysfs dev create failed\n");
        return ret;
    }

    lcd_print_string("16x2 LCD Driver", dev);

    dev_info(dev, "probe sucecess\n");

    return 0;
}

int lcd_platform_drv_remove(struct platform_device* pdev)
{
    struct lcd_device_private_data* dev_data = dev_get_drvdata(&pdev->dev);

    lcd_deinit(&pdev->dev);
    dev_info(&pdev->dev, "Remove called\n");
    device_unregister(dev_data->dev);

    return 0;
}

struct of_device_id lcd_device_match[] =
{
    {.compatible = "org,bone-lcd-16x2"},
    {}
};

struct platform_driver lcd_platform_drv =
{
    .probe = lcd_platform_drv_probe,
    .remove = lcd_platform_drv_remove,
    .driver = {
        .name = "lcd-platform-drv",
        .of_match_table = of_match_ptr(lcd_device_match)
    }
};

int __init lcd_platform_drv_init(void)
{
    lcd_drv_data.class_lcd = class_create(THIS_MODULE, "lcd-16x2");
    if(IS_ERR(lcd_drv_data.class_lcd)){
        pr_err("Error in creating class\n");
        return PTR_ERR(lcd_drv_data.class_lcd);
    }

    platform_driver_register(&lcd_platform_drv);

    pr_info("Module load success\n");
    return 0;
}

void __exit lcd_platform_drv_exit(void)
{
    platform_driver_unregister(&lcd_platform_drv);
    class_destroy(lcd_drv_data.class_lcd);
}

module_init(lcd_platform_drv_init);
module_exit(lcd_platform_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("maherme");
MODULE_DESCRIPTION("LCD 16x2 platform driver");
