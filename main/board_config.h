#pragma once

/* SPI 引脚 */
#define BOARD_PIN_SCLK      21
#define BOARD_PIN_MOSI      47
#define BOARD_PIN_MISO      (-1)

/* LCD 控制引脚 */
#define BOARD_PIN_LCD_RST   45
#define BOARD_PIN_LCD_DC    40
#define BOARD_PIN_LCD_CS    41
#define BOARD_PIN_BK_LIGHT  42

/* 屏幕参数（ST7789 240×240） */
#define BOARD_LCD_H_RES             240
#define BOARD_LCD_V_RES             240
#define BOARD_LCD_PIXEL_CLOCK_HZ    (20 * 1000 * 1000)
#define BOARD_LCD_CMD_BITS          8
#define BOARD_LCD_PARAM_BITS        8
#define BOARD_LCD_SPI_HOST          SPI2_HOST
