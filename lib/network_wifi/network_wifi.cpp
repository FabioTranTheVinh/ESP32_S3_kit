#include <WiFi.h>
#include "network_wifi.h"

void wifi_task(void *pvParameters)
{
    // Khởi tạo WiFi (chạy 1 lần)
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
    Serial.println("\nWiFi connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    
    while(1) {
        // Duy trì kết nối WiFi
        if(WiFi.status() != WL_CONNECTED) 
        {
            WiFi.reconnect();
            Serial.print("WiFi reconnecting...");
        }
        // else
        // {
        //     Serial.print("WiFi reconnected. IP: ");
        //     Serial.println(WiFi.localIP());
        // }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}


void wifi_init(void)
{
    //static TaskHandle_t ui_task_handle;
    xTaskCreatePinnedToCore(wifi_task, "WiFi", 4096, NULL, 3, NULL, 0);
}