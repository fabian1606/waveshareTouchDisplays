/*****************************************************************************
  | File        :   LVGL_Driver.c

  | help        :
    The provided LVGL library file must be installed first
******************************************************************************/
#include "lvglDriver.h"

lv_display_t *disp;

#define TFT_HOR_RES 480
#define TFT_VER_RES 480

// static lv_disp_draw_buf_t draw_buf;
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

void *buf1 = NULL;
void *buf2 = NULL;
// static lv_color_t buf1[ LVGL_BUF_LEN ];
// static lv_color_t buf2[ LVGL_BUF_LEN ];
// static lv_color_t* buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);
// static lv_color_t* buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN, MALLOC_CAP_SPIRAM);

/* Serial debugging */
void Lvgl_print(const char *buf)
{
  // Serial.printf(buf);
  // Serial.flush();
}

/*  Display flushing
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
// void Lvgl_Display_LCD( lv_display_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
// {
// LCD_addWindow(area->x1, area->y1, area->x2, area->y2, ( uint8_t *)&color_p->full);
//   lv_display_flush_ready( disp_drv );
// }
/*Read the touchpad*/

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, px_map);

  /*Call it to tell LVGL you are ready*/
  lv_display_flush_ready(disp);
}
void Lvgl_Touchpad_Read(lv_indev_t *indev, lv_indev_data_t *data)
{
  Touch_Read_Data();
  if (touch_data.points != 0x00)
  {
    data->point.x = touch_data.x;
    data->point.y = touch_data.y;
    data->state = LV_INDEV_STATE_PR;
    printf("LVGL : X=%u Y=%u points=%d\r\n", touch_data.x, touch_data.y, touch_data.points);
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  if (touch_data.gesture != NONE)
  {
  }
  touch_data.x = 0;
  touch_data.y = 0;
  touch_data.points = 0;
  touch_data.gesture = NONE;
}
void example_increase_lvgl_tick(void *arg)
{
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static uint32_t my_tick(void)
{
  return millis();
}

void Lvgl_Init(void)
{
  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);

  // esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &buf1, &buf2);
  // lv_disp_draw_buf_init( &draw_buf, buf1, buf2, ESP_PANEL_LCD_WIDTH * ESP_PANEL_LCD_HEIGHT);

  lv_init();

  /*Initialize the display*/
  lv_display_t *disp;
  disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

  // lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  // disp_drv.flush_cb = Lvgl_Display_LCD;
  // disp_drv.full_refresh = 1;                    /**< 1: Always make the whole screen redrawn*/
  // disp_drv.draw_buf = &draw_buf;
  // lv_disp_drv_register( &disp_drv );

  /*Initialize the (dummy) input device driver*/
  // static lv_indev_drv_t indev_drv;
  // lv_indev_drv_init( &indev_drv );
  // indev_drv.type = LV_INDEV_TYPE_POINTER;
  // indev_drv.read_cb = Lvgl_Touchpad_Read;
  // lv_indev_drv_register( &indev_drv );

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, Lvgl_Touchpad_Read);

  /* Create simple label */
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello Arduino, I'm LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  // lv_obj_t *label = lv_label_create( lv_scr_act() );
  // lv_label_set_text( label, "Hello Ardino and LVGL!");
  // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

  // const esp_timer_create_args_t lvgl_tick_timer_args = {
  //   .callback = &example_increase_lvgl_tick,
  //   .name = "lvgl_tick"
  // };
  // esp_timer_handle_t lvgl_tick_timer = NULL;
  // esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  // esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);
}
void Lvgl_Loop(void)
{
  lv_timer_handler(); /* let the GUI do its work */
  // vTaskDelay(pdMS_TO_TICKS(5));
}
