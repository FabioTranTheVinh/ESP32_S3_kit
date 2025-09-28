#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "navigate.h"
#include <ChronosESP32.h>
#include "oled/oled.h"

ChronosESP32 watch("ESP33Kit"); // set the bluetooth name
bool change = false;
uint32_t nav_crc = 0xFFFFFFFF;

// Thêm biến toàn cục để lưu trữ dữ liệu navigation và trạng thái
Navigation currentNavData;
bool isNavigationActive = false; // Biến theo dõi trạng thái dẫn đường

void navigate_task(void *pvParameters)
{

    //set the callbacks before calling begin funtion
    watch.setConnectionCallback(connectionCallback);
    //watch.setNotificationCallback(notificationCallback);
    watch.setConfigurationCallback(configCallback);
    watch.begin(); // initializes the BLE
    //Serial.println(watch.getAddress()); // mac address, call after begin()
    watch.setBattery(80); // set the battery level, will be synced to the app

    while (1)
    {
        watch.loop(); // handles internal routine functions
        // if (watch.isConnected()) {
        //     Navigation nav = watch.getNavigation();
        //     display.clearDisplay();
        //     display.setTextSize(1);
        //     display.setTextColor(SSD1306_WHITE);
        //     display.setCursor(0, 0);

        //     // Ví dụ hiển thị
        //     display.print("Nav: ");
        //     display.println(nav.directions);
        //     display.print("Dist: ");
        //     display.println(nav.distance);
        //     // Serial.print("Directions: ");
        //     // Serial.println(nav.directions);

        //     // Nếu có icon, bạn có thể vẽ hình nhỏ tương ứng
        //     // ví dụ nếu nav.icon == ICON_TURN_LEFT thì vẽ mũi tên trái,...

        //     display.display();
            
        // }
        //Kiểm tra cờ 'change' để cập nhật màn hình OLED
        //if (change) 
        //{
            updateNavigationDisplay(); // Gọi hàm cập nhật hiển thị
        //    change = false; // Reset cờ để chỉ cập nhật khi có thay đổi mới
        //}
        vTaskDelay(500 / portTICK_PERIOD_MS); // Giả sử cập nhật mỗi giây
    }
}

void navigate_init()
{
    xTaskCreatePinnedToCore(navigate_task, "Navigate", 4096, NULL, 4, NULL, 1);
}

void connectionCallback(bool state)
{
    //Serial.print("Connection state: ");
    //Serial.println(state ? "Connected" : "Disconnected");

    // Cập nhật trạng thái kết nối lên OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("Status: ");
    display.println(state ? "Connected" : "Disconnected");
    display.display();
}

void notificationCallback(Notification notification)
{/*
    Serial.print("Notification received at ");
    Serial.println(notification.time);
    Serial.print("From: ");
    Serial.print(notification.app);
    Serial.print("\tIcon: ");
    Serial.println(notification.icon);
    Serial.println(notification.title);
    Serial.println(notification.message);

    // Hiển thị thông báo lên OLED (có thể cần cuộn hoặc hiển thị từng phần)
    display.clearDisplay();
    display.setTextSize(1); // Kích thước chữ nhỏ
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("NEW NOTIF:");
    display.println("-----------");
    display.print(notification.app);
    display.print(": ");
    // Giới hạn độ dài nội dung để không tràn màn hình
    if (notification.title.length() > 20) { // Giới hạn 20 ký tự
        display.println(notification.title.substring(0, 17) + "...");
    } else {
        display.println(notification.title);
    }
    if (notification.message.length() > 40) { // Giới hạn 40 ký tự
         display.println(notification.message.substring(0, 37) + "...");
    } else {
        display.println(notification.message);
    }
    display.display();*/
}


// Hàm mới để cập nhật hiển thị OLED (bao gồm icon và văn bản)
void updateNavigationDisplay() {
    // Chỉ hiển thị nếu navigation đang hoạt động
    currentNavData = watch.getNavigation();
      
    if (!isNavigationActive) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.println("Navigation Inactive");
        display.display();
        return;
    }

    // Xóa toàn bộ màn hình hoặc chỉ vùng văn bản nếu bạn muốn icon luôn hiện
    // Mình sẽ vẽ lại cả icon và văn bản để đảm bảo mọi thứ đồng bộ
    display.clearDisplay();

    // VẼ ICON ĐIỀU HƯỚNG
    // Vẽ icon ở góc trên bên trái (0,0) nếu có dữ liệu icon hợp lệ
    if (nav_crc != 0xFFFFFFFF) { // nav_crc = 0xFFFFFFFF nghĩa là chưa có icon nào được gửi
        display.drawBitmap(0, 0, currentNavData.icon, 48, 48, SSD1306_WHITE);
    } else {
        // Nếu không có icon, bạn có thể để trống hoặc vẽ một hình nền đen ở đó
        display.fillRect(0, 0, 48, 48, SSD1306_BLACK);
    }


    // THIẾT LẬP VỊ TRÍ VÀ THÔNG SỐ VĂN BẢN
    display.setTextSize(1); // Bạn đang dùng 1
    display.setTextColor(SSD1306_WHITE);

    int text_start_x = 55;   // Bắt đầu văn bản từ cột 55 (bên phải icon 48px + khoảng trống)
    // Với setTextSize(1.5), chiều cao ký tự khoảng 12 pixel (8*1.5).
    // Dòng 16 pixel có vẻ phù hợp cho setTextSize(1.5) như bạn đã dùng.
    int line_height = 16;


    // CÁC DÒNG HIỂN THỊ THÔNG TIN VĂN BẢN
    display.setCursor(text_start_x, 0 * line_height);
    display.print("Dist: ");
    display.println(currentNavData.distance);

    display.setCursor(text_start_x, 1 * line_height);
    display.println("Title: ");

    display.setTextSize(2);
    display.setCursor(text_start_x, 2 * line_height);
    display.println(currentNavData.title); // Đây là khoảng cách rẽ kế tiếp

    // Nếu bạn muốn hiển thị các thông tin khác từ nav object, hãy thêm vào đây
    // Ví dụ:
    // display.setCursor(text_start_x, 2 * line_height);
    // display.print("Dir: "); display.println(currentNavData.directions);
    // display.setCursor(text_start_x, 3 * line_height);
    // display.print("ETA: "); display.println(currentNavData.eta);


    display.display(); // Đẩy tất cả dữ liệu ra màn hình
}


void configCallback(Config config, uint32_t a, uint32_t b)
{   
    switch (config)
    {
    case CF_NAV_DATA:
        //Serial.print("Navigation state: ");
        //Serial.println(a ? "Active" : "Inactive");
        isNavigationActive = a; // Cập nhật trạng thái dẫn đường toàn cục

        if (isNavigationActive) // Nếu navigation active
        {
            currentNavData = watch.getNavigation(); // Lưu dữ liệu navigation vào biến toàn cục
            //Serial.println(currentNavData.directions);
            //Serial.println(currentNavData.eta);
            //Serial.println(currentNavData.duration);
            //Serial.println(currentNavData.distance);
            //Serial.println(currentNavData.title);
            //Serial.println(currentNavData.speed);
            //In thêm next_step_distance nếu thư viện của bạn có
            //Serial.println(currentNavData.next_step_distance);

            change = true; // Đặt cờ để biết cần cập nhật hiển thị OLED
        } else { // Nếu navigation không active
            change = true; // Đặt cờ để gọi updateNavigationDisplay() để hiển thị "Inactive"
        }
        break;

    case CF_NAV_ICON:
        //Serial.print("Navigation Icon data, position: ");
        //Serial.println(a);
        //Serial.print("Icon CRC: ");
        //Serial.printf("0x%04X\n", b);
        if (a == 2){ // Khi icon đã được truyền đầy đủ
            Navigation tempNav = watch.getNavigation(); // Lấy dữ liệu icon
            if (nav_crc != tempNav.iconCRC) // Chỉ cập nhật nếu CRC thay đổi
            {
                nav_crc = tempNav.iconCRC;
                currentNavData = tempNav; // Lưu dữ liệu icon vào biến toàn cục
                change = true; // Đặt cờ để cập nhật hiển thị OLED
            }
        }
        break;
    }
}

