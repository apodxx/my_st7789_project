#pragma once

#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif



#define TEST_LCD_BK_LIGHT_GPIO  42   // BLK背光 → GPIO42 ✔
#define TEST_LCD_RST_GPIO       45   // RES复位 → GPIO45 ✔
#define TEST_LCD_CS_GPIO        41   // CS片选 → GPIO41 ✔
#define TEST_LCD_DC_GPIO        40   // DC数据选择 → GPIO40 ✔
#define TEST_LCD_PCLK_GPIO      21   // SCL/SPI时钟 → GPIO21（原来8→改成21）
#define TEST_LCD_MOSI_GPIO      47   // SDA/SPI数据 → GPIO47（原来9→改成47）

#define TEST_LCD_PIXEL_CLOCK_HZ (30 * 1000 * 1000)

void st7789_init(uint16_t width, uint16_t height);
void st7789_deinit();
void st7789_DrawPixel(int16_t x, int16_t y, uint16_t color);
uint16_t st7789_GetPixel(int16_t x, int16_t y);
void st7789_Update();
void st7789_SetBL(uint8_t Value);
void st7789_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

uint16_t st7789_GetWidth();
uint16_t st7789_GetHeight();

void lcd_panel_test();

void GC9A01A_SleepMode(uint8_t Mode);
void GC9A01A_DisplayPower(uint8_t On);

#ifdef __cplusplus
}
#endif
