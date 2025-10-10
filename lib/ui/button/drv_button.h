#ifndef DRV_BUTTON_H
#define DRV_BUTTON_H

#include <stdio.h>

// Định nghĩa chân Input cho nút bấm
#define BUTTON_TOUCH_PIN    11
#define BUTTON_DOWN_PIN     6
#define BUTTON_UP_PIN       7 

#define BUTTON_UPDATE_PERIOD_MS      10
#define BUTTON_UPDATE_DEBOUNCE_MS(x) (x / BUTTON_UPDATE_PERIOD_MS)

// Thêm các define và structure
#define BUTTON_DEBOUNCE_TIME_MS    30
#define BUTTON_PRESS_TIME_MS       3000  // Thời gian nhấn tối đa
//#define BUTTON_RELEASE_TIME_MS     20    // Thời gian nhả tối thiểu

// Định nghĩa kiểu dữ liệu cho hàm callback
typedef void (*btnCallback)(void);


typedef enum {
    BTN_STATE_IDLE = 0,
    BTN_STATE_DEBOUNCING,
    BTN_STATE_PRESSED,
    BTN_STATE_PRESSED_TIMEOUT,
    BTN_STATE_FINISHED
    
} button_state_e;


typedef enum 
{
    BTN_INDEX_TOUCH = 0, 
    BTN_INDEX_UPSIDE, 
    BTN_INDEX_DOWNSIDE, 


    BTN_GROUP_UPSIDE_DOWNSIDE,
    BTN_GROUP_TOUCH_UPSIDE,  
    BTN_GROUP_TOUCH_DOWNSIDE,   
    BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE,  

    BTN_INDEX_MAX
} button_index_e;


typedef struct {
    button_index_e button_id;
    uint8_t button_pin;
    uint8_t mode;
    button_state_e state;
    unsigned long last_change_time;
    bool active_level;              // true = HIGH active, false = LOW active
    bool is_pressed;
    bool event_triggered;
    btnCallback callback;
} button_t;

// Khai báo một mảng các con trỏ hàm để lưu các hàm callback
// extern btnCallback g_btnCallbacks[]; // 3 nút đơn + 4 nhóm nút
//extern button_t g_buttons[];
extern button_t g_buttons[BTN_INDEX_MAX];

void drv_button_init(void);
button_index_e get_button_state();
void register_button_callback(button_index_e button_state_id, btnCallback callback_func);

#endif