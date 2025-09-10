#include <FastLED.h>
#include "ui_led.h"


system_state_led_e currentState = STATE_IDLE; //kiem tra trang thai hien tai cua led
static CRGB leds[LEDS_NUM];  // Số lượng LED trên bo mạch
CRGB current_color = CRGB::Black; // Màu hiện tại của LED

void blinkLED(int delaytime, int count)
{
    for (int i = 0; i < count; i++)
    {
        leds[0] = current_color;
        FastLED.show();
        vTaskDelay(delaytime / portTICK_PERIOD_MS);
        leds[0] = CRGB::Black;
        FastLED.show();
        vTaskDelay(delaytime / portTICK_PERIOD_MS);
    }
}

void ui_led_task(void *parameters)
{
    /** Init the FastLED library */
    FastLED.addLeds<NEOPIXEL, RGB_LED_PIN>(leds, LEDS_NUM);

    FastLED.setBrightness(50); // Set initial brightness to a moderate level

    /** Init the user button */

    while (1)
    {
        updateLedStateFromQueue(&currentState);
        switch (currentState)
        {
            case STATE_IDLE:
                current_color = CRGB::Purple;
                //leds[0] = CRGB::Purple;
                //leds[1] = CRGB::Black;
                break;
            case STATE_WIFI_CONNECTING:
                current_color = CRGB::Orange; // Network connecting    
                //leds[0] = CRGB::Blue; // Network connecting
                //leds[1] = CRGB::Orange; // System connecting
                break;
            case STATE_WIFI_CONNECTED:
                current_color = CRGB::Pink; // Network connected
                //leds[0] = CRGB::MediumSpringGreen; // Network connected
                //leds[1] = CRGB::Purple; // System ready
                break;
            case STATE_WIFI_DISCONNECTED:
                current_color = CRGB::Blue; // Network disconnected
                //leds[0] = CRGB::Orange; // Network disconnected
                //leds[1] = CRGB::Orange; // System reconnecting
                break;
            case STATE_AUDIO_RECORDING:
                current_color = CRGB::YellowGreen; // Audio recording 
                //leds[0] = CRGB::Purple; // Audio recording
                //leds[1] = CRGB::Purple; // Audio recording
                break;
            case STATE_ERROR:
                current_color = CRGB::Red; // System error
                //leds[0] = CRGB::Red; // System error
                //leds[1] = CRGB::Red; // System error
                break;
            default:
                current_color = CRGB::Black;
                //leds[0] = CRGB::Black;
                //leds[1] = CRGB::Black;
                break;
        }
        blinkLED(200, 3);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Trong led task
void updateLedStateFromQueue(system_state_led_e *currentState) 
{
    int newState = -1;
    if(xQueueReceive(ledQueue, &newState, 0) == pdTRUE) 
    {
        *currentState = (system_state_led_e)newState;
        Serial.print("Trạng thái mới:  ");
        //Serial.println(currentState);
    }
}



//Hàm để nhấp nháy đèn LED
// void blinkLED(CRGB leds[], int delayTime, int count)
// {
//   for (int i = 0; i < count; i++)
//   {
//     FastLED.show();
//     delay(delayTime);
//     FastLED.clear();
//     FastLED.show();
//     delay(delayTime);
//   }
// }

void ui_led_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(ui_led_task, "LED_RGB", 2048, NULL, 1, NULL, 1);
}

