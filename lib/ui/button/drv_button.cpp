#include <Arduino.h>
#include "button/button.h"
#include "button/drv_button.h"

// Mảng lưu trữ các con trỏ hàm callback
btnCallback g_btnCallbacks[BTN_INDEX_MAX] ={0};

// Global button instances
button_t g_buttons[BTN_INDEX_MAX];

void drv_button_init()
{
    pinMode(BUTTON_TOUCH_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    
    memset(g_buttons, 0 , sizeof(g_buttons));

    g_buttons[BTN_INDEX_TOUCH].button_pin = BUTTON_TOUCH_PIN;
    g_buttons[BTN_INDEX_TOUCH].active_level = true;  // PULLDOWN -> HIGH active

    g_buttons[BTN_INDEX_UPSIDE].button_pin = BUTTON_UP_PIN;
    g_buttons[BTN_INDEX_UPSIDE].active_level = false; // PULLUP -> LOW active
    
    g_buttons[BTN_INDEX_DOWNSIDE].button_pin = BUTTON_DOWN_PIN;
    g_buttons[BTN_INDEX_DOWNSIDE].active_level = false; // PULLUP -> LOW active

    register_button_callback(BTN_INDEX_TOUCH, on_touch_button_pressed);
    register_button_callback(BTN_INDEX_UPSIDE, on_upside_button_pressed);
    register_button_callback(BTN_INDEX_DOWNSIDE, on_downside_button_pressed);
    register_button_callback(BTN_GROUP_UPSIDE_DOWNSIDE, on_upside_downside_pressed);
    register_button_callback(BTN_GROUP_TOUCH_DOWNSIDE, on_touch_downside_pressed);
    register_button_callback(BTN_GROUP_TOUCH_UPSIDE, on_touch_upside_pressed);
    register_button_callback(BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE,on_upside_downside_pressed);
}

//---------------------------------------------------------
// Hàm đăng ký callback
//---------------------------------------------------------
// Hàm này được sử dụng để gán một hàm xử lý cho một sự kiện nút nhấn cụ thể.
void register_button_callback(button_index_e button_state_id, btnCallback callback_func) 
{
    if (button_state_id < BTN_INDEX_MAX) 
    {
        g_btnCallbacks[button_state_id] = callback_func;
    }
}


button_index_e get_button_state() 
{
    // Đọc trạng thái chân vật lý và xác định nút nào đang được nhấn

    if (digitalRead(BUTTON_TOUCH_PIN) == HIGH && digitalRead(BUTTON_UP_PIN) == LOW && digitalRead(BUTTON_DOWN_PIN) == LOW) {
        return BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE;
    }
    else if (digitalRead(BUTTON_TOUCH_PIN) == HIGH && digitalRead(BUTTON_DOWN_PIN) == LOW) {
        return BTN_GROUP_TOUCH_DOWNSIDE;
    }
    else if (digitalRead(BUTTON_TOUCH_PIN) == HIGH && digitalRead(BUTTON_UP_PIN) == LOW) {
        return BTN_GROUP_TOUCH_UPSIDE;
    }
    else if (digitalRead(BUTTON_UP_PIN) == LOW && digitalRead(BUTTON_DOWN_PIN) == LOW) {
        return BTN_GROUP_UPSIDE_DOWNSIDE;
    }
    else if (digitalRead(BUTTON_TOUCH_PIN) == HIGH) {
        return BTN_INDEX_TOUCH;
    }
    else if (digitalRead(BUTTON_UP_PIN) == LOW) {
        return BTN_INDEX_UPSIDE;
    }
    else if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
        return BTN_INDEX_DOWNSIDE;
    }


    // Trả về một giá trị không hợp lệ nếu không có nút nào được nhấn
    return BTN_INDEX_MAX; 
}

