// Cấu hình cho OLED SSD1306
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled/oled.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Định nghĩa chân SDA và SCL tùy chỉnh
#define CUSTOM_SDA 1
#define CUSTOM_SCL 2
TwoWire customI2C = TwoWire(0); // Sử dụng bus I2C thứ 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &customI2C, OLED_RESET);


void ui_oled_init()
{
    // Bắt đầu giao tiếp I2C với các chân tùy chỉnh
    customI2C.begin(CUSTOM_SDA, CUSTOM_SCL, 400000); // Tốc độ 400kHz
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // display.clearDisplay();
    // display.setTextSize(1);
    // //display.setTextColor(BLACK, WHITE); // 'inverted' text
    // display.setTextColor(WHITE); // 'inverted' text
    // display.setCursor(0, 28);
    // display.println("Hello world!");
    ui_screen_set_new_step(DISP_INDEX_PROFILE_PIC);
    display.display();
    delay(1000);
}

void ui_screen_set_new_step(display_index_e new_step)
{
    switch(new_step)
    {
        case DISP_INDEX_MOUNTAIN_PIC:
        {
            display.clearDisplay();
            display.drawBitmap(0, 0, image_mountain_bits, 128, 64, 1);
            display.display();
            //delay(5000);
            break;
        }
        case DISP_INDEX_MAP_PIC:
        {
            display.clearDisplay();

            display.drawRect(0, 0, 128, 64, 1);

            display.setTextColor(1);
            //display.setTextWrap(false);
            //display.setFont(&Org_01);
            //display.setCursor(56, 8);
            //display.print("Map");

            //display.drawLine(0, 11, 127, 11, 1);

            display.drawBitmap(2, 4, image_map_bits, 124, 58, 1);

            display.display();
            break;
        }
        case DISP_INDEX_PROFILE_PIC:
        {
            display.clearDisplay();

            display.setTextColor(1);
            display.setTextWrap(false);
            // display.setFont(&Org_01);
            display.setCursor(1, 0);
            display.print("Profile");

            display.drawLine(0, 7, 126, 7, 1);

            display.drawRect(2, 10, 36, 30, 1);

            display.setCursor(41, 15);
            display.print("Name: FabioTran");

            display.setCursor(41, 24);
            display.print("Title:Soft Eng");

            display.setCursor(41, 33);
            display.print("Age: 29");

            display.setCursor(41, 42);
            display.print("Status: Single");

            // display.setCursor(41, 43);
            // display.print("Lost: ");

            display.setCursor(41, 51);
            display.print("Open To Work");

            // display.setCursor(2, 58);
            // display.print("Special:");

            // display.setCursor(65, 36);
            // display.print("000A");

            // display.setCursor(68, 43);
            // display.print("000A");

            // display.setCursor(33, 51);
            // display.print("Level 1");

            // display.setCursor(37, 58);
            // display.print("Rockets Level 1");

            display.drawBitmap(8, 14, image_profile_bits, 27, 24, 1);

            display.display();

            break;
        }
        default:
            break;
    }
}