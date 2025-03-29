/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include "lvglDriver.h"

// #define LV_CONF_INCLUDE_SIMPLE true
// #include "LVGL_Example.h"
void Driver_Loop(void *parameter)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void Driver_Init()
{
    I2C_Init();
    TCA9554PWR_Init(0x00);
    Set_EXIO(EXIO_PIN8, Low);
    // PCF85063_Init();

    xTaskCreatePinnedToCore(
        Driver_Loop,
        "Other Driver task",
        4096,
        NULL,
        3,
        NULL,
        0);
}
void setup()
{
    Driver_Init();
    LCD_Init(); // If you later reinitialize the LCD, you must initialize the SD card again !!!!!!!!!!
    Lvgl_Init();

    // Lvgl_Example1();
    // lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_printer();
    // lv_demo_stress();
}

void loop()
{
    Lvgl_Loop();
    vTaskDelay(pdMS_TO_TICKS(5));
}
