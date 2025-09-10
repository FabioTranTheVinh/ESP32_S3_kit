#include <FastLED.h>
#include "ui_led.h"


system_state_led_t currentState = STATE_IDLE;
static CRGB leds[LEDS_NUM];

void ui_led_task(void *parameters)
{
    /** Init the FastLED library */
    FastLED.addLeds<NEOPIXEL, RGB_LED_PIN>(leds, LEDS_NUM);

    /** Init the user button */

    while (1)
    {
        updateLedStateFromQueue(&currentState);
        switch (currentState)
        {
            case STATE_IDLE:
                leds[0] = CRGB::Purple;
                //leds[1] = CRGB::Black;
                break;
            case STATE_WIFI_CONNECTING:
                leds[0] = CRGB::Blue; // Network connecting
                //leds[1] = CRGB::Orange; // System connecting
                break;
            case STATE_WIFI_CONNECTED:
                leds[0] = CRGB::MediumSpringGreen; // Network connected
                //leds[1] = CRGB::Purple; // System ready
                break;
            case STATE_WIFI_DISCONNECTED:
                leds[0] = CRGB::Orange; // Network disconnected
                //leds[1] = CRGB::Orange; // System reconnecting
                break;
            case STATE_AUDIO_RECORDING:
                leds[0] = CRGB::Purple; // Audio recording
                //leds[1] = CRGB::Purple; // Audio recording
                break;
            case STATE_ERROR:
                leds[0] = CRGB::Red; // System error
                //leds[1] = CRGB::Red; // System error
                break;
            default:
                leds[0] = CRGB::Black;
                //leds[1] = CRGB::Black;
                break;
        }
        blinkLED(leds, 200, 3);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Trong led task
void updateLedStateFromQueue(system_state_led_t *currentState) 
{
    int newState = -1;
    if(xQueueReceive(ledQueue, &newState, 0) == pdTRUE) 
    {
        *currentState = (system_state_led_t)newState;
        Serial.print("Trạng thái mới:  ");
        //Serial.println(currentState);
    }
}

// Hàm để nhấp nháy đèn LED
void blinkLED(CRGB leds[], int delayTime, int count)
{
  for (int i = 0; i < count; i++)
  {
    FastLED.show();
    delay(delayTime);
    FastLED.clear();
    FastLED.show();
    delay(delayTime);
  }
}

void ui_led_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(ui_led_task, "LED_RGB", 2048, NULL, 1, NULL, 1);
}

