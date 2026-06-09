#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ── 引脚定义 ── */
#define EXAMPLE_PIN_NUM_SCLK        21   // SCL  时钟线
#define EXAMPLE_PIN_NUM_MOSI        47   // SDA  数据线
#define EXAMPLE_PIN_NUM_MISO        -1
#define EXAMPLE_PIN_NUM_LCD_RST     45   // RES  复位
#define EXAMPLE_PIN_NUM_LCD_DC      40   // DC   数据/命令选择
#define EXAMPLE_PIN_NUM_LCD_CS      41   // CS   片选
#define EXAMPLE_PIN_NUM_BK_LIGHT    42   // BLK  背光

/* ── 屏幕参数（240×240） ── */
#define EXAMPLE_LCD_H_RES           240
#define EXAMPLE_LCD_V_RES           240
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ  (20 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS        8
#define EXAMPLE_LCD_PARAM_BITS      8
#define LCD_HOST                    SPI2_HOST

static const char *TAG = "st7789_240x240";

void app_main(void)
{
    /* ── 1. 配置背光 GPIO ── */
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT,
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, 0); // 初始化期间关闭背光

    /* ── 2. 初始化 SPI 总线 ── */
    spi_bus_config_t buscfg = {
        .sclk_io_num     = EXAMPLE_PIN_NUM_SCLK,
        .mosi_io_num     = EXAMPLE_PIN_NUM_MOSI,
        .miso_io_num     = EXAMPLE_PIN_NUM_MISO,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    /* ── 3. 创建 LCD IO 句柄 ── */
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num       = EXAMPLE_PIN_NUM_LCD_DC,
        .cs_gpio_num       = EXAMPLE_PIN_NUM_LCD_CS,
        .pclk_hz           = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits      = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits    = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode          = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
        (esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    /* ── 4. 安装 ST7789 驱动 ── */
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_ele_order  = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    /* ── 5. 复位并初始化面板 ── */
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    /* ── 6. 240×240 屏幕特殊处理 ── */
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 0, 0));

    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));

    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

    /* ── 7. 打开显示并开启背光 ── */
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    static uint16_t line[EXAMPLE_LCD_H_RES];
    const uint16_t colors[] = {
        0xF800, // red
        0x07E0, // green
        0x001F, // blue
        0xFFFF, // white
    };

    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, 1);

    while (true) {
        for (int color_index = 0; color_index < 4; ++color_index) {
            for (int x = 0; x < EXAMPLE_LCD_H_RES; ++x) {
                line[x] = colors[color_index];
            }

            for (int y = 0; y < EXAMPLE_LCD_V_RES; ++y) {
                ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, EXAMPLE_LCD_H_RES, y + 1, line));
            }

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    ESP_LOGI(TAG, "ST7789 240x240 初始化完成！");
}