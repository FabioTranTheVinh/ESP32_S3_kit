#include <Arduino.h>
#include "button/button.h"
#include "button/drv_button.h"
#include "oled/oled.h"



void on_touch_button_pressed() {
    printf("Nút TOUCH đã được nhấn!\n");
    //ui_screen_set_new_step(DISP_INDEX_PROFILE_PIC);
}

void on_upside_button_pressed() {
    printf("Nút UPSIDE đã được nhấn!\n");
    ui_screen_set_new_step(DISP_INDEX_MOUNTAIN_PIC);
}

void on_downside_button_pressed() {
    printf("Nút DOWNSIDE đã được nhấn!\n");
    ui_screen_set_new_step(DISP_INDEX_MAP_PIC);
}

void on_upside_downside_pressed() {
    printf("Nhóm nút UPSIDE và DOWNSIDE đã được nhấn!\n");
}

// Thêm các hàm khác cho các nhóm nút còn lại nếu cần...
void on_touch_upside_pressed() {
    printf("Nhóm nút TOUCH và UPSIDE đã được nhấn!\n");
}

void on_touch_downside_pressed()
{
    printf("Nhóm nút TOUCH và DOWN đã được nhấn!\n");
}
void on_touch_upside_downside_pressed()
{
    printf("Nhóm nút TOUCH và UPSIDE và DOWN đã được nhấn!\n");
}


static void trigger_button_callback(button_index_e button_id)
{
    if (button_id < BTN_INDEX_MAX && g_btnCallbacks[button_id] != NULL)
    {
        g_btnCallbacks[button_id]();
    }

}


void button_task(void *pvParameters)
{
    drv_button_init();
    Serial.println("drv_button_init\n");
    uint32_t current_time = 0;

    while (1)
    {
        // Chỉ update các button vật lý (không phải group)
        for(int i = 0; i < BTN_INDEX_MAX; i++)
        {
            button_t *btn = &g_buttons[i];
            if (btn->button_pin == 0) continue; // Skip uninitialized buttons


            bool current_level = digitalRead(btn->button_pin);
            bool is_active = (current_level == btn->active_level);
            current_time = millis();


            switch (btn->state)
            {
                case BTN_STATE_IDLE:
                    if (is_active && (is_active != btn->is_pressed))
                    {
                        //phát hiện press, chuyển sang debouncing
                        btn->state = BTN_STATE_DEBOUNCING;
                        btn->last_change_time = current_time;                 
                    }
                    break;

                case BTN_STATE_DEBOUNCING:
                    if (is_active && (current_time - btn->last_change_time >= BUTTON_DEBOUNCE_TIME_MS)) //verified the debounce
                    {
                        if (is_active != btn->is_pressed)
                        {
                            //Passed the debouce checck
                            btn->is_pressed = is_active;  
                            btn->event_triggered = false;
                            btn->state = BTN_STATE_PRESSED;
                        }
                    }
                    else
                    {
                        //return released (bounce)
                        btn->state = BTN_STATE_IDLE;
                    }
                    break;

                case BTN_STATE_PRESSED:
                    Serial.println("BTN_STATE_PRESSED");
                    // check button is pressed down within 3s
                    
                    if (btn->is_pressed && !btn->event_triggered)
                    {

                        if (current_time - btn->last_change_time > BUTTON_PRESS_TIME_MS)
                        {
                            //Pressing timeout
                            btn->state = BTN_STATE_PRESSED_TIMEOUT;
                            Serial.println("Pressed Button too long");
                            break;
                        }

                        //active an event
                        if(btn->is_pressed != is_active)
                        {
                            btn->event_triggered = true;
                            btn->state = BTN_STATE_FINISHED;
                        }
                    }
                    break;

                case BTN_STATE_PRESSED_TIMEOUT:
                    if(!is_active)
                    { 
                        Serial.println("Button check RESET!!!!");
                        btn->is_pressed = false;
                        btn->last_change_time = current_time;
                        btn->event_triggered = false;
                        btn->state = BTN_STATE_IDLE;

                    }
                    break;

                case BTN_STATE_FINISHED:

                    if (btn->event_triggered)
                    {
                        // Active Callback -call only once
                        trigger_button_callback((button_index_e)i);
                        Serial.println("BTN_STATE_FINISHED!!!!");
                        btn->is_pressed = false;
                        btn->last_change_time = current_time;
                        btn->event_triggered = false;
                        btn->state = BTN_STATE_IDLE;
                    }
                    
                    break;

                default:
                    break;
            }
        }
        // Xử lý button combinations
        //check_group_button();

        // Đọc trạng thái nút bấm và xử lý
        vTaskDelay(100 / portTICK_PERIOD_MS); // Giả sử cập nhật trạng thái nút mỗi 10ms
    }

}

void ui_button_init()
{
    xTaskCreatePinnedToCore(button_task, "Button", (1024 * 2) , NULL, 2, NULL, 1);
}