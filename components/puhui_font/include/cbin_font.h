#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

lv_font_t *cbin_font_create(uint8_t *bin_addr);
void cbin_font_delete(lv_font_t *font);

#ifdef __cplusplus
}
#endif
