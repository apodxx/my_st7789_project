#include "font_loader.h"

#include "cbin_font.h"
#include "esp_log.h"

static const char *TAG = "font_loader";

extern const uint8_t _binary_font_puhui_common_14_1_bin_start[];
extern const uint8_t _binary_font_puhui_common_14_1_bin_end[];

static lv_font_t *s_font = NULL;

const lv_font_t *font_loader_init(void)
{
    if (s_font) {
        return s_font;
    }

    size_t size = (size_t)(_binary_font_puhui_common_14_1_bin_end -
                           _binary_font_puhui_common_14_1_bin_start);
    ESP_LOGI(TAG, "加载普惠体常用字�? %u bytes", (unsigned)size);

    s_font = cbin_font_create((uint8_t *)_binary_font_puhui_common_14_1_bin_start);
    if (!s_font) {
        ESP_LOGE(TAG, "字库加载失败");
        return NULL;
    }

    return s_font;
}

const lv_font_t *font_loader_get(void)
{
    return s_font;
}
