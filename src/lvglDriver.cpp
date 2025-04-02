#include "lvglDriver.h"

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
static uint32_t draw_buf[DRAW_BUF_SIZE / 4];

/* Display flushing */
static void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    LCD_addWindow(area->x1, area->y1, area->x2, area->y2, px_map);
    lv_display_flush_ready(disp);
}

/* Read the touchpad */
static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    Touch_Read_Data();
    if (touch_data.points != 0x00)
    {
        data->point.x = touch_data.x;
        data->point.y = touch_data.y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    touch_data.x = 0;
    touch_data.y = 0;
    touch_data.points = 0;
    touch_data.gesture = NONE;
}

static uint32_t my_tick(void)
{
    return millis();
}

lv_display_t* Display_Init(void)
{
    // Initialize low-level drivers
    I2C_Init();
    TCA9554PWR_Init(0x00);
    Set_EXIO(EXIO_PIN8, Low);
    LCD_Init();
    
    // Initialize LVGL
    lv_init();
    lv_tick_set_cb(my_tick);

    // Setup display
    lv_display_t* display = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(display, my_disp_flush);
    lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Setup touchpad
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchpad_read);

    return display;
}

void Display_Update(void)
{
    lv_timer_handler();
    delay(5);
}
