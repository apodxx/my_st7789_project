idf_component_register(
    SRCS "ui.c" "display.c" "main.c"
    INCLUDE_DIRS "."
    REQUIRES esp_lcd driver esp_lvgl_port puhui_font
)