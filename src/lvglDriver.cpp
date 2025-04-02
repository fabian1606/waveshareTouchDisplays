/*****************************************************************************
  | File        :   LVGL_Driver.c

  | help        :
    The provided LVGL library file must be installed first
******************************************************************************/
#include "lvglDriver.h"
#include <lvgl.h>

lv_display_t *disp;

#define TFT_HOR_RES 480
#define TFT_VER_RES 480

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

/* Serial debugging */
void Lvgl_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}

/* Display flushing */
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    LCD_addWindow(area->x1, area->y1, area->x2, area->y2, px_map);
    /* Call it to tell LVGL you are ready */
    lv_display_flush_ready(disp);
}

/* Read the touchpad */
void Lvgl_Touchpad_Read(lv_indev_t *indev, lv_indev_data_t *data)
{
    Touch_Read_Data();
    if (touch_data.points != 0x00)
    {
        data->point.x = touch_data.x;
        data->point.y = touch_data.y;
        data->state = LV_INDEV_STATE_PRESSED;
        printf("LVGL : X=%u Y=%u points=%d\r\n", touch_data.x, touch_data.y, touch_data.points);
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

void Lvgl_Init(void)
{
    lv_init();

    /* Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    /* Initialize the display */
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Initialize the input device driver */
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /* Touchpad should have POINTER type */
    lv_indev_set_read_cb(indev, Lvgl_Touchpad_Read);

    /* Create simple label */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello Arduino, I'm LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
void Lvgl_Loop(void)
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */
}
