#pragma once

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

/**
 * 初始化背光、SPI 总线、ST7789 面板及 LVGL 显示驱动。
 * 成功时通过 out 参数返回句柄；失败时由 ESP_ERROR_CHECK 触发重启。
 */
void display_init(esp_lcd_panel_io_handle_t *out_io,
                  esp_lcd_panel_handle_t *out_panel,
                  lv_display_t **out_disp);
