#include <Arduino.h>
#include "button/button.h"
#include "button/drv_button.h"
#include "oled/oled.h"


void on_touch_button_pressed() {
    printf("Nút TOUCH đã được nhấn!\n");
    ui_screen_set_new_step(DISP_INDEX_PROFILE_PIC);
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

}
void on_touch_upside_downside_pressed()
{

}





void button_task(void *pvParameters)
{
    drv_button_init();

    while (1)
    {
        button_index_e button_handle_event = get_button_state();
        if (button_handle_event != BTN_INDEX_MAX)
        {
            if (g_btnCallbacks[button_handle_event] != NULL)
            {
                g_btnCallbacks[button_handle_event]();
            }
        }
        // Đọc trạng thái nút bấm và xử lý
        vTaskDelay(50 / portTICK_PERIOD_MS); // Giả sử cập nhật trạng thái nút mỗi 10ms
    }

}

void ui_button_init()
{
    xTaskCreatePinnedToCore(button_task, "Button", 2048, NULL, 2, NULL, 1);
}