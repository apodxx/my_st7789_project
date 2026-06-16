idf_component_register(
    SRCS "src/cbin_font.c" "src/font_loader.c"
    INCLUDE_DIRS "include"
    REQUIRES lvgl
)

target_add_binary_data(${COMPONENT_LIB} "cbin/font_puhui_common_14_1.bin" BINARY)

target_compile_definitions(${COMPONENT_LIB} PUBLIC LV_LVGL_H_INCLUDE_SIMPLE)
