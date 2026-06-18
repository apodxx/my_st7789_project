#include "display.h"
extern const lv_font_t font_puhui_14_1;
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui.h"
#include "lv_demos.h"

static const char *TAG = "st7789_240x240";

void app_main(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    lv_display_t *disp = NULL;

    display_init(&io_handle, &panel_handle, &disp);

    lv_demo_benchmark();
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();
    // lv_demo_keypad_encoder();

    ESP_LOGI(TAG, "ST7789 240x240 初始化完成");

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
