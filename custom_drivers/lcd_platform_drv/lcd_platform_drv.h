#ifndef LCD_PLATFORM_DRV_H
#define LCD_PLATFORM_DRV_H

#define LCD_LINES   (4+1+1+1)

/* Device private data structure */
struct lcd_device_private_data{
    int lcd_scroll;
    char lcdxy[8];
    struct gpio_desc* desc[LCD_LINES];
    struct device* dev;
};

/* Driver private data structure */
struct lcd_platform_drv_private_data{
    struct class* class_lcd;
};

#endif /* LCD_PLATFORM_DRV_H */
