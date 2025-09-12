#ifndef DRV_BUTTON_H
#define DRV_BUTTON_H

#include <stdio.h>

// Định nghĩa chân Input cho nút bấm
#define BUTTON_TOUCH_PIN    11
#define BUTTON_DOWN_PIN     6
#define BUTTON_UP_PIN       7 

#define BUTTON_UPDATE_PERIOD_MS      10
#define BUTTON_UPDATE_DEBOUNCE_MS(x) (x / BUTTON_UPDATE_PERIOD_MS)


typedef enum 
{
    BTN_INDEX_TOUCH     = 0, 
    BTN_INDEX_UPSIDE    = 1, 
    BTN_INDEX_DOWNSIDE  = 2, 


    BTN_GROUP_UPSIDE_DOWNSIDE,
    BTN_GROUP_TOUCH_UPSIDE,  
    BTN_GROUP_TOUCH_DOWNSIDE,   
    BTN_GROUP_TOUCH_UPSIDE_DOWNSIDE,  

    BTN_INDEX_MAX
} button_index_e;



// Định nghĩa kiểu dữ liệu cho hàm callback
typedef void (*btnCallback)(void);

// Khai báo một mảng các con trỏ hàm để lưu các hàm callback
extern btnCallback g_btnCallbacks[BTN_INDEX_MAX]; // 3 nút đơn + 4 nhóm nút


void drv_button_init(void);
button_index_e get_button_state();
void register_button_callback(button_index_e button_state_id, btnCallback callback_func);

#endif