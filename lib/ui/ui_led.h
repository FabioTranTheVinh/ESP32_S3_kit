#pragma once

//#include "main.cpp"
// Định nghĩa chân RGB LED
#define RGB_LED_PIN 48
#define RGB_MAX_BRIGHTNESS 255
#define RGB_DEFAULT_BRIGHTNESS 100
#define RGB_MIN_BRIGHTNESS 10
#define LEDS_NUM 1                 // Số lượng LED trên bo mạch

extern QueueHandle_t ledQueue;


// Biến lưu trạng thái
typedef enum {
    STATE_IDLE,
    STATE_WIFI_CONNECTING,
    STATE_WIFI_CONNECTED,
    STATE_WIFI_DISCONNECTED,
    STATE_AUDIO_RECORDING,
    STATE_ERROR
} system_state_led_t;

void ui_led_init(void);
void blinkLED(CRGB leds[], int delayTime, int count);
void updateLedStateFromQueue(system_state_led_t *currentState);