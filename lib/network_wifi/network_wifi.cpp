#include <WiFi.h>
#include <WiFiManager.h>
#include "network_wifi.h"
#include "led/ui_led.h"
#include "button/drv_button.h"

const int WIFI_CONFIG_TIMEOUT = 90;
const int WIFI_CONNECT_TIMEOUT = 5;

unsigned long last_check_time = 0;
bool is_wifi_lost_connect = false;
bool is_portal_opened = true;


void wifi_task(void *pvParameters)
{
    WiFi.mode(WIFI_STA);
    system_state_led_e _new_led_state;
    static WiFiManager wifi_manager;
    wifi_manager.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
    wifi_manager.setConnectTimeout(WIFI_CONNECT_TIMEOUT);
    wifi_manager.setDebugOutput(false);          // optional   
    wifi_manager.setEnableConfigPortal(false);

    while(1) 
    {
        if(digitalRead(BUTTON_TOUCH_PIN) == HIGH) 
        {
            Serial.println("Button pressed - WiFi Check and Auto Connect....!");

            is_portal_opened = true;
            if(!wifi_manager.autoConnect("ESP32_S3")) 
            {
                Serial.println("Failed to auto-connect, starting Portal...");
                if(!wifi_manager.startConfigPortal("ESP32_S3")) 
                {
                    Serial.println("Faild to Config Porttal and hit timeout.");
                    ESP.restart();
                    //vTaskDelay(10 / portTICK_PERIOD_MS);
                } 
                else
                {
                    is_portal_opened = false;
                    is_wifi_lost_connect = false;
                    _new_led_state = STATE_WIFI_CONNECTED;
                    xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
                    Serial.println("WiFi Connected......");
                    Serial.print("IP Address: ");
                    Serial.println(WiFi.localIP());
                }
            }
            else
            {
                is_portal_opened = false;
                is_wifi_lost_connect = false;
                _new_led_state = STATE_WIFI_CONNECTED;
                xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
                Serial.println("WiFi Connected......");
                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());
            } 
            
            // Chờ button nhả ra
            while(digitalRead(BUTTON_TOUCH_PIN) == HIGH) 
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }   
        }

        // Duy trì kết nối WiFi
        
        if(!is_portal_opened && millis() - last_check_time > 5000)
        {
            last_check_time = millis();

            if(WiFi.status() != WL_CONNECTED || is_wifi_lost_connect)
            {   
                Serial.print("Lost connecting!! WiFi reconnecting..."); 
                
                if(wifi_manager.autoConnect())
                {
                    is_wifi_lost_connect = false;
                    _new_led_state = STATE_WIFI_CONNECTED;
                    xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
                    Serial.println("WiFi Re-Connected......");
                    Serial.print("IP Address: ");
                    Serial.println(WiFi.localIP());
                }
                else
                {
                    is_wifi_lost_connect = true;
                    Serial.print("WiFi Re-Connected FAILED..... ");
                    _new_led_state = STATE_WIFI_DISCONNECTED;
                    xQueueSend(ledQueue, &_new_led_state, portMAX_DELAY);
                }
            }


        }  
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}


void wifi_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(wifi_task, "WiFi_task", 4096, NULL, 0, NULL, 0); // mức độ ưu tiên set 0 mới không bị Watchdog reset
    //12288, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1
}