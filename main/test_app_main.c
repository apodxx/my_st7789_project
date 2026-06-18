#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "display/dispcolor.h"
#include "display/fonts/font.h"
#include "st7789/st7789.h"
#include "encoder/encoder.h"

static char *TAG = "ENC";

uint32_t cntr = 0;
int32_t pos = 0;

void input_cb(sEncoderInfo event) {
    ESP_LOGI(TAG, "event %d, pos %ld", event.event, event.pos);
    pos = event.pos;
}

void app_main()
{
    dispcolor_Init(240, 240);
    dispcolor_ClearScreen();
    dispcolor_SetBrightness(255);

    encoder_init();
    encoder_set_cb(input_cb);
    encoder_start();

    while (1) {
        dispcolor_ClearScreen();

        dispcolor_DrawLine(0, 0, 170, 320, RED);
        dispcolor_DrawLine(170, 0, 0, 320, RED);

        dispcolor_DrawLine(0, 0, 170, 0, GREEN);
        dispcolor_DrawLine(0, 319, 169, 319, GREEN);

        dispcolor_DrawLine(0, 0, 0, 319, BLUE);
        dispcolor_DrawLine(169, 0, 169, 319, BLUE);

        dispcolor_printf(10, 10, FONTID_10X16F, WHITE, "Counter %d", cntr++);
        dispcolor_printf(10, 40, FONTID_10X16F, WHITE, "Pos %ld", pos);

        dispcolor_Update();
        vTaskDelay(20);
    }
}
