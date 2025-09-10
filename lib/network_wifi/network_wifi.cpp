#include <WiFi.h>
#include "network_wifi.h"
#include "ui_led.h"


void wifi_task(void *pvParameters)
{
    system_state_led_e new_led_state;

    // Khởi tạo WiFi (chạy 1 lần)
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        new_led_state = STATE_WIFI_CONNECTING;
        xQueueSend(ledQueue, &new_led_state, portMAX_DELAY);
        vTaskDelay(200 / portTICK_PERIOD_MS);

    }
    new_led_state = STATE_WIFI_CONNECTED;
    xQueueSend(ledQueue, &new_led_state, portMAX_DELAY);
    Serial.println("\nWiFi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    bool wifi_disconent_flag = false;
    
    while(1) {
        // Duy trì kết nối WiFi
        
        if(WiFi.status() != WL_CONNECTED && !wifi_disconent_flag)
        {   
            wifi_disconent_flag = true;
            WiFi.reconnect();
            Serial.print("WiFi reconnecting...");
            new_led_state = STATE_WIFI_CONNECTING;
            xQueueSend(ledQueue, &new_led_state, portMAX_DELAY);
        }
        else
        {
            if (wifi_disconent_flag && WiFi.status() == WL_CONNECTED)
            {
                wifi_disconent_flag = false;
                Serial.print("WiFi reconnected. IP: ");
                Serial.println(WiFi.localIP());
                new_led_state = STATE_WIFI_CONNECTED;
                xQueueSend(ledQueue, &new_led_state, portMAX_DELAY);
            }
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}


void wifi_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(wifi_task, "WiFi", 4096, NULL, 3, NULL, 0);
}