/**
 * @file ui_mng.cpp
 * @author Richin Abraham
 * @brief Manages the user interface.
 */

// INCLUDES

#include <TFT_eSPI.h>
#include "rm67162.h"
#include "ui_mng.h"
#include "ui_config.h"
#include "wifi_mng.h"
#include "images/wifi_small.h"

// DEFINES

// TYPES

// GLOBAL VARIABLES

/** @brief Instance of the TFT class. */
TFT_eSPI tft = TFT_eSPI();
/** @brief Instance of the sprite class for background. */
TFT_eSprite background_sprite = TFT_eSprite(&tft);
/** @brief Instance of the sprite class for notification. */
TFT_eSprite notification_sprite = TFT_eSprite(&tft);
/** @brief Variable to store the notification refresh time reference in milliseconds. */
uint64_t ui_mng_notif_time_ref;
/** @brief Variable to store the WiFi frame index. */
uint8_t wifi_frame_index;

// FUNCTIONS PROTOTYPES

static void ui_mng_refresh_notification(
    uint64_t current_time
);
static void ui_mng_refresh_wifi(
    uint64_t current_time
);

// FUNCTIONS

void ui_mng_init(void)
{
    rm67162_init();
    lcd_setRotation(UI_DISPLAY_ORIENTATION);
    background_sprite.createSprite(UI_DISPLAY_WIDTH, UI_DISPLAY_HEIGHT);
    background_sprite.setSwapBytes(true);
    notification_sprite.createSprite(UI_DISPLAY_WIDTH, UI_NOTIFICATION_HEIGHT);
    notification_sprite.setSwapBytes(true);
    // initialise the variables
    wifi_frame_index = 0u;
    ui_mng_notif_time_ref = millis();

    // Refresh full UI
    ui_mng_refresh_notification(ui_mng_notif_time_ref + UI_NOTIFICATION_REFRESH_PERIOD_MS);
}

void ui_mng_loop(void)
{
    uint64_t current_time = millis();

    background_sprite.fillSprite(TFT_BLACK);
    notification_sprite.fillSprite(TFT_BLACK);

    ui_mng_refresh_notification(current_time);

    lcd_PushColors(0, 0, UI_DISPLAY_WIDTH, UI_DISPLAY_HEIGHT, (uint16_t *)background_sprite.getPointer());
}

static void ui_mng_refresh_notification(
    uint64_t current_time
)
{
    // Update the notification panel every refresh period only
    notification_sprite.fillRect(0, 0, 60, 60, TFT_RED);
    ui_mng_refresh_wifi(current_time);
    notification_sprite.pushToSprite(&background_sprite, 0u, 0u, TFT_BLACK);
}

static void ui_mng_refresh_wifi(
    uint64_t current_time
)
{
    notification_sprite.pushImage(180, 0, WIFI_SMALL_FRAME_WIDTH, WIFI_SMALL_FRAME_HEIGHT, &wifi_small[wifi_frame_index][0]);
    // Display the frame according to WiFi connetion status
    if (wifi_mng_getstate() == WIFI_MNG_STATE_CONNECTED) {
        // Since WiFi is connected use the last frame
        wifi_frame_index = WIFI_SMALL_NUM_FRAMES - 1u;
    }
    else {
        if (current_time >= (ui_mng_notif_time_ref + UI_NOTIFICATION_REFRESH_PERIOD_MS)) {
            // update the gif frame index
            wifi_frame_index = (wifi_frame_index + 1u) < WIFI_SMALL_NUM_FRAMES ? wifi_frame_index + 1u : 0u;
            // Update the reference time
            ui_mng_notif_time_ref = current_time;
        }
    }
}
