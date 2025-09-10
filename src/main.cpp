#include <Arduino.h>
//#include <WiFi.h>
#include <BluetoothSerial.h>
#include <FastLED.h>
#include <HTTPClient.h>
#include <queue.h>

#include "wiring.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "freertos/semphr.h"
// #include <ArduinoJson.h>

// Cấu hình cho OLED SSD1306
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Cấu hình cho I2S và MAX98357A
#include <driver/i2s.h>
#include <math.h>


//#include "config.h"
#include "ui_led.h"
#include "network_wifi.h"


// Tạo queue để gửi trạng thái đến LED task
QueueHandle_t ledQueue;


// Định nghĩa bus I2S
#define I2S_NUM I2S_NUM_0
#define SAMPLE_RATE 44100

// Cấu hình I2S
i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
  .intr_alloc_flags = 0,
  .dma_buf_count = 8,
  .dma_buf_len = 64
};
// Cấu hình chân I2S
i2s_pin_config_t i2s_pin_config = {
  .bck_io_num = I2S_BCLK_PIN,
  .ws_io_num = I2S_LRCK_PIN,
  .data_out_num = I2S_DOUT_PIN,
  .data_in_num = I2S_PIN_NO_CHANGE
};
// Hàm tạo và phát sóng sin
void playSineWave(int frequency, int duration_ms) {
  int num_samples = (SAMPLE_RATE * duration_ms) / 1000;
  int16_t sample[2];
  const float PI_2 = 6.28318530718;
  const float sine_amp = 32767.0;
  const float volume_factor = 1.0;

  for (int i = 0; i < num_samples; i++) {
    float val = sine_amp*volume_factor  * sin((PI_2 * frequency * i) / SAMPLE_RATE);
    int16_t s_val = (int16_t)val;
    sample[0] = s_val;
    sample[1] = s_val;
    size_t bytes_written;
    i2s_write(I2S_NUM, sample, sizeof(sample), &bytes_written, portMAX_DELAY);
  }
}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Định nghĩa chân SDA và SCL tùy chỉnh
// #define CUSTOM_SDA 1
// #define CUSTOM_SCL 2
TwoWire customI2C = TwoWire(0); // Sử dụng bus I2C thứ 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &customI2C, OLED_RESET);


void setup()
{
  Serial.begin(115200);

  ledQueue = xQueueCreate(5, sizeof(int));
  ui_led_init();
  wifi_init();
  
  

  // Bắt đầu giao tiếp I2C với các chân tùy chỉnh
  customI2C.begin(CUSTOM_SDA, CUSTOM_SCL, 400000); // Tốc độ 400kHz
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0, 28);
  display.println("Hello world!");
  display.display();
  delay(200);
  //display.clearDisplay();



  //FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  // 1. Nhấp nháy màu vàng khi khởi động
  //fillLED(COLOR_BLUE); 


  // Khởi tạo kết nối WiFi
  //Serial.println("Connecting to WiFi...");
  // WiFi.begin(ssid, password);

  // // Chờ kết nối
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }

  // // 3. Chuyển sang màu xanh khi kết nối thành công
  // fillLED(COLOR_PURPLE);
  // Serial.println("\nWiFi connected successfully!");
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());



  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &i2s_pin_config);
  // Phát tiếng bíp duy nhất
  playSineWave(1000, 200); // 1000 Hz trong 200ms
    // Dừng I2S sau khi phát xong
  i2s_stop(I2S_NUM);
  

  // Cấu hình chân nút bấm
  pinMode(BUTTON_Touch_PIN_1, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_DOWN, INPUT);
  pinMode(BUTTON_PIN_UP, INPUT);
 
  


}

void loop()
{

  // if (digitalRead(BUTTON_Touch_PIN_1) == HIGH) // Kiểm tra trạng thái nút bấm
  // {
  //   Serial.println("ButtonTouch 1 Pressed");
  //   display.setCursor(0, 0);
  //   display.setTextSize(1);
  //   display.println("ButtonTouch 1 Pressed");
  //   display.display();
  // }
  // else
  // {
  //   // Scroll full screen
  //   display.clearDisplay();

  //   display.setCursor(0, 0);
  //   display.setTextSize(1);
  //   display.println("Fabio");
  //   display.println("TTI");
  //   display.println("scrolling!");
  //   display.display();
  //   display.startscrollright(0x00, 0x07);
  //   delay(1500);
  //   display.stopscroll();
  //   delay(1000);
  //   display.startscrollleft(0x00, 0x07);
  //   delay(1500);
  //   display.stopscroll();
  //   display.clearDisplay();
  //   //test git commit
  //}
}
