#pragma once
#include <ChronosESP32.h>


void navigate_init();
void navigate_task(void *pvParameters);
void connectionCallback(bool state);
void notificationCallback(Notification notification);
void configCallback(Config config, uint32_t a, uint32_t b);
void updateNavigationDisplay();