#include "microphone.h"
//#include <driver/i2s.h>

void mic_init(void)
{
    // Cấu hình I2S cho microphone
    // i2s_config_t i2s_mic_config = {
    //     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    //     .sample_rate = 16000, // Sample rate for microphone
    //     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    //     .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // Mono input
    //     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    //     .intr_alloc_flags = 0,
    //     .dma_buf_count = 4,
    //     .dma_buf_len = 256,
    //     .use_apll = false,
    //     .tx_desc_auto_clear = false,
    //     .fixed_mclk = 0
    // };

    // i2s_pin_config_t i2s_mic_pin_config = {
    //     .bck_io_num = 15,   // Bit clock pin
    //     .ws_io_num = 14,    // Word select (LRCK) pin
    //     .data_out_num = I2S_PIN_NO_CHANGE,
    //     .data_in_num = 13   // Data input pin
    // };

    // // Cài đặt và khởi động driver I2S cho microphone
    // i2s_driver_install(I2S_NUM_0, &i2s_mic_config, 0, NULL);
    // i2s_set_pin(I2S_NUM_0, &i2s_mic_pin_config);
}