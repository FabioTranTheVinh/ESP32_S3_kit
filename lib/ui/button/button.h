#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



void ui_button_init(void);
void button_task(void *pvParameters);

void on_touch_button_pressed();
void on_upside_button_pressed();
void on_downside_button_pressed();
void on_upside_downside_pressed();
void on_touch_upside_pressed();
void on_touch_downside_pressed();
void on_touch_upside_downside_pressed();