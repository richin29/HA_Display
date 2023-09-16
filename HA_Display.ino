/**
 * @file      HA_Display.ino
 * @author    Richin Abraham
 */

#include "ui_mng.h"
#include "wifi_mng.h"
#include "mqtt_mng.h"
#include "board_validation.h"
#include "secrets.h"
#include "config.h"

// DEFINES


// GLOBAL VARIABLES

/** @brief Variable to store the main loop count. */
uint64_t loop_count;

// FUNCTIONS

void setup()
{
    Serial.begin(115200);
    ui_mng_init();
    wifi_mng_init();
    mqtt_mng_init();
}

void loop()
{
    loop_count++;
    //uint16_t x;
    //uint16_t y;
    //uint8_t colour_index = 0u;


    //for (y = 0u; y < DISPLAY_HEIGHT; y = y + SQUARE_SIDE + VERTICAL_SEPERATOR_HEIGHT) {
    //    for (x = 0u; x < DISPLAY_WIDTH; x = x + SQUARE_SIDE) {
    //        spr.fillRect(x, y, SQUARE_SIDE, SQUARE_SIDE, colour_table[colour_index]);
    //        colour_index = ((colour_index + 1u) >= colour_table_size) ? 0u : (colour_index + 1u);
    //    }
    //}

    //spr.fillRect(60, 0, SQUARE_SIDE, SQUARE_SIDE, TFT_GREEN);
    //spr.fillRect(120, 0, SQUARE_SIDE, SQUARE_SIDE, TFT_BLUE);
    //spr.fillRect(180, 0, SQUARE_SIDE, SQUARE_SIDE, TFT_YELLOW);

    ui_mng_loop();
    wifi_mng_loop();
    mqtt_mng_loop();
}