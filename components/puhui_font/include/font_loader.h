#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 从固件内嵌的普惠体常用字库（约 7000 字）创建 LVGL 字体。 */
const lv_font_t *font_loader_init(void);

/** 获取已加载字体，未初始化时返回 NULL。 */
const lv_font_t *font_loader_get(void);

#ifdef __cplusplus
}
#endif
