#include "ui.h"

#include "board_config.h"
#include "esp_lvgl_port.h"
#include "font_loader.h"
#include "lvgl.h"

void ui_create_demo_screen(void)
{
    if (!lvgl_port_lock(0)) {
        return;
    }

    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    const lv_font_t *text_font = font_loader_get();
    if (!text_font) {
        lvgl_port_unlock();
        return;
    }

    lv_obj_set_style_text_font(screen, text_font, 0);

    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "你好，中文显示\nHello ESP32\n温度湿度设置");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(label, BOARD_LCD_H_RES - 20);
    lv_obj_center(label);

    lvgl_port_unlock();
}
