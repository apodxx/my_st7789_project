#include "display.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui.h"

static const char *TAG = "st7789_240x240";

void app_main(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    lv_display_t *disp = NULL;

    display_init(&io_handle, &panel_handle, &disp);
    ui_create_demo_screen();

    ESP_LOGI(TAG, "ST7789 240x240 初始化完成");

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
