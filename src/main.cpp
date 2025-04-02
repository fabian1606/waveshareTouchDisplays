#include <lvglDriver.h>

// Global variables
static lv_display_t* display = nullptr;
static uint32_t button_press_count = 0;
static lv_obj_t* counter_label = nullptr;

void buttonEventCallback(lv_event_t *e)
{
    button_press_count++;
    
    // Update counter text
    if (counter_label) {
        String count_text = "Button pressed: " + String(button_press_count) + " times";
        lv_label_set_text(counter_label, count_text.c_str());
    }
}


// Initialize the UI elements
void setup_ui(void)
{
    // Create counter label
    counter_label = lv_label_create(lv_scr_act());
    lv_label_set_text(counter_label, "Button pressed: 0 times");
    lv_obj_align(counter_label, LV_ALIGN_TOP_MID, 0, 10);
    
    // Create a simple test button 
    // for other examples look at : https://docs.lvgl.io/master/examples.html
    lv_obj_t *btn = lv_btn_create(lv_scr_act()); // create an button object
    lv_obj_set_pos(btn, 180, 215); // set the position of the button
    lv_obj_set_size(btn, 120, 50); // set the size of the button

    lv_obj_t *label = lv_label_create(btn); // create a label object
    lv_label_set_text(label, "Press Me!"); // set the text of the label
    lv_obj_center(label); // center the label on the button

    lv_obj_add_event_cb(btn, buttonEventCallback, LV_EVENT_CLICKED, NULL); //set that the button will call the buttonEventCallback function when clicked
}

void setup()
{
    Serial.begin(115200);

    // Initialize display and get the display object
    display = Display_Init();
    Serial.println("Display initialized");
    
    setup_ui();
    Serial.println("UI setup completed");
}

void loop()
{
    // Update display
    Display_Update();
    delay(10); // Add a small delay to avoid busy-waiting
}
