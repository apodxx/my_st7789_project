#include "display.h"

#include "board_config.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lvgl_port.h"

static void init_backlight(void)
{
    gpio_config_t cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << BOARD_PIN_BK_LIGHT,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
    gpio_set_level(BOARD_PIN_BK_LIGHT, 0);
}

static void init_spi_bus(void)
{
    spi_bus_config_t buscfg = {
        .sclk_io_num     = BOARD_PIN_SCLK,
        .mosi_io_num     = BOARD_PIN_MOSI,
        .miso_io_num     = BOARD_PIN_MISO,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = BOARD_LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(BOARD_LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));
}

static esp_lcd_panel_io_handle_t create_panel_io(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num       = BOARD_PIN_LCD_DC,
        .cs_gpio_num       = BOARD_PIN_LCD_CS,
        .pclk_hz           = BOARD_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits      = BOARD_LCD_CMD_BITS,
        .lcd_param_bits    = BOARD_LCD_PARAM_BITS,
        .spi_mode          = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
        (esp_lcd_spi_bus_handle_t)BOARD_LCD_SPI_HOST, &io_config, &io_handle));
    return io_handle;
}

static esp_lcd_panel_handle_t create_panel(esp_lcd_panel_io_handle_t io_handle)
{
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BOARD_PIN_LCD_RST,
        .rgb_ele_order  = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 0, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    gpio_set_level(BOARD_PIN_BK_LIGHT, 1);
    return panel_handle;
}

static lv_display_t *create_lvgl_display(esp_lcd_panel_io_handle_t io_handle,
                                         esp_lcd_panel_handle_t panel_handle)
{
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    lvgl_port_display_cfg_t disp_cfg = { 0 };
    disp_cfg.io_handle = io_handle;
    disp_cfg.panel_handle = panel_handle;
    disp_cfg.buffer_size = BOARD_LCD_H_RES * 40;
    disp_cfg.double_buffer = true;
    disp_cfg.hres = BOARD_LCD_H_RES;
    disp_cfg.vres = BOARD_LCD_V_RES;
    disp_cfg.monochrome = false;
    disp_cfg.color_format = LV_COLOR_FORMAT_RGB565;
    disp_cfg.rotation.swap_xy = false;
    disp_cfg.rotation.mirror_x = false;
    disp_cfg.rotation.mirror_y = false;
    disp_cfg.flags.buff_dma = true;
    disp_cfg.flags.swap_bytes = false;

    lv_display_t *disp = lvgl_port_add_disp(&disp_cfg);
    ESP_ERROR_CHECK(disp ? ESP_OK : ESP_FAIL);
    return disp;
}

void display_init(esp_lcd_panel_io_handle_t *out_io,
                  esp_lcd_panel_handle_t *out_panel,
                  lv_display_t **out_disp)
{
    init_backlight();
    init_spi_bus();

    esp_lcd_panel_io_handle_t io_handle = create_panel_io();
    esp_lcd_panel_handle_t panel_handle = create_panel(io_handle);
    lv_display_t *disp = create_lvgl_display(io_handle, panel_handle);

    *out_io = io_handle;
    *out_panel = panel_handle;
    *out_disp = disp;
}
