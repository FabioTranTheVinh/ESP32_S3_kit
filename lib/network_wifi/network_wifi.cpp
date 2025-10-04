#include <WiFi.h>
#include <WiFiManager.h>
#include "network_wifi.h"
#include "led/ui_led.h"
#include "button/drv_button.h"

const int WIFI_CONFIG_TIMEOUT = 120;
const int WIFI_CONNECT_TIMEOUT = 10;



void wifi_task(void *pvParameters)
{
    WiFi.mode(WIFI_STA);
    system_state_led_e _new_led_state;
    static WiFiManager wifi_manager;
    wifi_manager.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
    wifi_manager.setConnectTimeout(WIFI_CONNECT_TIMEOUT);
    //wifi_manager.setDebugOutput(true); // optional   
    Serial.println("WiFi Config Started.....");



    // // Khởi tạo WiFi (chạy 1 lần)
    // Serial.println("Connecting to WiFi...");
    // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     Serial.print(".");
    //     _new_led_state = STATE_WIFI_CONNECTING;
    //     xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
    //     vTaskDelay(200 / portTICK_PERIOD_MS);

    // }
    // _new_led_state = STATE_WIFI_CONNECTED;
    // xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
    // Serial.println("\nWiFi connected successfully!");
    // Serial.print("IP Address: ");
    // Serial.println(WiFi.localIP());

    // bool wifi_disconent_flag = false;

    while(1) 
    {
        
        if(digitalRead(BUTTON_TOUCH_PIN) == HIGH) 
        {
            Serial.println("Button pressed - Starting Config Portal");
            wifi_manager.resetSettings();

            if(!wifi_manager.startConfigPortal("ESP32_S3")) 
            {
                Serial.println("Faild to connect and hit timeout.");
                ESP.restart();
                //vTaskDelay(10 / portTICK_PERIOD_MS);
            } 
            else
            {
                Serial.println("WiFi Connected......");
                _new_led_state = STATE_WIFI_CONNECTED;
                xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());
            }
            // Chờ button nhả ra
            while(digitalRead(BUTTON_TOUCH_PIN) == HIGH) 
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }   
        }
    vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

        
        
        // // Duy trì kết nối WiFi
        
        // if(WiFi.status() != WL_CONNECTED && !wifi_disconent_flag)
        // {   
        //     wifi_disconent_flag = true;
        //     WiFi.reconnect();
        //     Serial.print("WiFi reconnecting...");
        //     _new_led_state = STATE_WIFI_CONNECTING;
        //     xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
        // }
        // else
        // {
        //     if (wifi_disconent_flag && WiFi.status() == WL_CONNECTED)
        //     {
        //         wifi_disconent_flag = false;
        //         Serial.print("WiFi reconnected. IP: ");
        //         Serial.println(WiFi.localIP());
        //         _new_led_state = STATE_WIFI_CONNECTED;
        //         xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
        //     }
        // }



void wifi_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(wifi_task, "WiFi_task", 4096, NULL, 0, NULL, 0);
    //12288, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1
}