#ifndef WIRING_HW_H
#define WIRING_HW_H

    // Định nghĩa các chân I2S kết nối với MAX98357A
    #define I2S_LRCK_PIN 3
    #define I2S_BCLK_PIN 4
    #define I2S_DOUT_PIN 5

    // Định nghĩa Microphone Pin
    #define MIC_SCK 10
    #define MIC_WS 9
    #define MIC_SD 8


    // Định nghĩa chân SDA và SCL tùy chỉnh
    #define CUSTOM_SDA 1
    #define CUSTOM_SCL 2

    // Định nghĩa chân Input cho nút bấm
    #define BUTTON_Touch_PIN_1 11
    #define BUTTON_PIN_DOWN 6
    #define BUTTON_PIN_UP 7 


    // // Định nghĩa chân RGB LED
    // #define RGB_LED_PIN 48
    // #define RGB_MAX_BRIGHTNESS 255
    // #define RGB_DEFAULT_BRIGHTNESS 100
    // #define RGB_MIN_BRIGHTNESS 10
    // #define LEDS_NUM 2                 // Số lượng LED trên bo mạch

#endif