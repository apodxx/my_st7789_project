#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
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

    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, 1);

    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = { 0 };
    disp_cfg.io_handle = io_handle;
    disp_cfg.panel_handle = panel_handle;
    disp_cfg.buffer_size = EXAMPLE_LCD_H_RES * 40;
    disp_cfg.double_buffer = true;
    disp_cfg.hres = EXAMPLE_LCD_H_RES;
    disp_cfg.vres = EXAMPLE_LCD_V_RES;
    disp_cfg.monochrome = false;
    disp_cfg.color_format = LV_COLOR_FORMAT_RGB565;
    disp_cfg.rotation.swap_xy = false;
    disp_cfg.rotation.mirror_x = false;
    disp_cfg.rotation.mirror_y = false;
    disp_cfg.flags.buff_dma = true;
    disp_cfg.flags.swap_bytes = false;

    lv_display_t *disp = lvgl_port_add_disp(&disp_cfg);
    ESP_ERROR_CHECK(disp ? ESP_OK : ESP_FAIL);

    if (lvgl_port_lock(0)) {
        lv_obj_t *screen = lv_screen_active();
        lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
        lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

        lv_obj_t *label = lv_label_create(screen);
        lv_label_set_text(label, "\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe4\xb8\xad\xe6\x96\x87\xe6\x98\xbe\xe7\xa4\xba\nHello ESP32");
        lv_obj_set_style_text_font(label, &lv_font_source_han_sans_sc_16_cjk, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_width(label, EXAMPLE_LCD_H_RES - 20);
        lv_obj_center(label);

        lvgl_port_unlock();
    }

    ESP_LOGI(TAG, "ST7789 240x240 初始化完成！");

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}