#include <Arduino.h>
#include "button/button.h"
#include "button/drv_button.h"

// Mảng lưu trữ các con trỏ hàm callback
//btnCallback g_btnCallbacks[BTN_INDEX_MAX] ={0};

// Global button instances
button_t g_buttons[BTN_INDEX_MAX];

static void debug_print_buttons() {
    Serial.println("\n ==== BUTTON CONFIGURATION DEBUG ====");
    
    for (int i = 0; i < BTN_INDEX_MAX; i++) {
        button_t *btn = &g_buttons[i];
        
        Serial.printf("Button[%d]: ", i);
        
        // Print button name
        switch(i) {
            case BTN_INDEX_TOUCH:   Serial.print("TOUCH    "); break;
            case BTN_INDEX_UPSIDE:  Serial.print("UPSIDE   "); break;
            case BTN_INDEX_DOWNSIDE:Serial.print("DOWNSIDE "); break;
            case BTN_GROUP_UPSIDE_DOWNSIDE: Serial.print("UP+DOWN  "); break;
            case BTN_GROUP_TOUCH_DOWNSIDE:  Serial.print("TOUCH+DOWN "); break;
            case BTN_GROUP_TOUCH_UPSIDE:    Serial.print("TOUCH+UP "); break;
            case BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE: Serial.print("ALL 3 "); break;
            default: Serial.print("UNKNOWN ");
        }
        
        // Print hardware info (chỉ cho nút vật lý)
        if (btn->button_pin != 0) {
            Serial.printf("| Pin: %d | Mode: ", btn->button_pin);
            switch(btn->mode) {
                case INPUT: Serial.print("INPUT"); break;
                case INPUT_PULLUP: Serial.print("INPUT_PULLUP"); break;
                case INPUT_PULLDOWN: Serial.print("INPUT_PULLDOWN"); break;
                default: Serial.print(btn->mode);
            }
            
            Serial.printf(" | Active_LV: %s", btn->active_level ? "HIGH" : "LOW");
            Serial.printf(" | Current State: %s", digitalRead(btn->button_pin) ? "HIGH" : "LOW");
        } else {
            Serial.print("| [VIRTUAL GROUP]");
        }
        
        // Print state info
        Serial.printf(" | State: ");
        switch(btn->state) {
            case BTN_STATE_IDLE: Serial.print("IDLE"); break;
            case BTN_STATE_DEBOUNCING: Serial.print("DEBOUNCING"); break;
            case BTN_STATE_PRESSED: Serial.print("PRESSED"); break;
            case BTN_STATE_PRESSED_TIMEOUT: Serial.print("PRESSED_TIMEOUT"); break;
            case BTN_STATE_FINISHED: Serial.print("FINISHED"); break;
            default: Serial.print(btn->state);
        }
        
        Serial.printf(" | Pressed: %s", btn->is_pressed ? "YES" : "NO");
        Serial.printf(" | Event: %s", btn->event_triggered ? "TRIGGERED" : "NO");
        Serial.printf(" | Callback: %s", btn->callback != NULL ? "SET" : "NULL");
        
        Serial.println();
    }
    Serial.println("=====================================\n");
}


void drv_button_init()
{
    static const struct {
        button_index_e button_id;
        uint8_t button_pin;
        uint8_t mode;
        bool active_level;
        btnCallback callback;
    } config[] = {
        // Individual buttons
        {BTN_INDEX_TOUCH, BUTTON_TOUCH_PIN, INPUT_PULLDOWN, true, on_touch_button_pressed},
        {BTN_INDEX_UPSIDE, BUTTON_UP_PIN, INPUT_PULLUP, false, on_upside_button_pressed},
        {BTN_INDEX_DOWNSIDE, BUTTON_DOWN_PIN, INPUT_PULLUP, false, on_downside_button_pressed},

        //Group buttons
        {BTN_GROUP_UPSIDE_DOWNSIDE, 0, 0, false, on_upside_downside_pressed},
        {BTN_GROUP_TOUCH_UPSIDE, 0, 0, false, on_touch_upside_pressed},
        {BTN_GROUP_TOUCH_DOWNSIDE, 0, 0, false, on_touch_downside_pressed},
        {BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE, 0, 0, false, on_touch_upside_downside_pressed},

    };
    
    memset(g_buttons, 0 , sizeof(g_buttons));

    for (size_t i = 0; i < sizeof(config)/sizeof(config[0]); i++)
    {
        button_t *btn = &g_buttons[config[i].button_id];
        
        btn->button_id = config[i].button_id;
        if (config[i].button_pin)     // only config these parameter for the hardware button pin
        {
            btn->button_pin = config[i].button_pin;
            btn->mode = config[i].mode;
            btn->active_level = config[i].active_level;
            pinMode(config[i].button_pin, config[i].mode);
        }
        btn->callback = config[i].callback;
        btn->state = BTN_STATE_IDLE;
        btn->is_pressed = false; 
    }

    debug_print_buttons();
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

