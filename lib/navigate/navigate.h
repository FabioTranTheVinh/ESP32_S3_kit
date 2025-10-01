#pragma once
#include <ChronosESP32.h>

typedef struct
{
    String title;          // Mô tả ngắn gọn về bước dẫn đường (ví dụ: "Turn right onto Main St.")
    float  f_dist;
    char   units[3]; // Đủ chỗ cho "km", "m" và ký tự null '\0' 
    float  f_dist_convert; // chuyển đổi sang m hoặc km
} lane_track_t;


void navigate_init();
void navigate_task(void *pvParameters);
void connectionCallback(bool state);
void notificationCallback(Notification notification);
void configCallback(Config config, uint32_t a, uint32_t b);
void updateNavigationDisplay();
void draw_line_time();
float get_distance_to_next_turn(const String& title);