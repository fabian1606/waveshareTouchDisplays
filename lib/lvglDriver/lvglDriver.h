#pragma once

#include <lvgl.h>
#include <displaySt7701.h>
#include <touchCst820.h>

// Display configuration
#define TFT_HOR_RES 480
#define TFT_VER_RES 480

// Function to initialize the display and return the display object
lv_display_t* Display_Init(void);

// Core display functions
void Display_Update(void);  // Handle display updates and timing
