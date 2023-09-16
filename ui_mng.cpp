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
#include "mqtt_mng.h"
#include "images/wifi_40x40.h"
#include "images/ha_32x32.h"

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
/** @brief Variable to store the home assistant frame index. */
uint8_t ha_frame_index;

char ha_data[50];
uint8_t ha_data_len;

// FUNCTIONS PROTOTYPES

static void ui_mng_load_notification(
    uint64_t current_time
);
static void ui_mng_refresh_notification(void);

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
    ha_frame_index = 0u;
    ha_data_len = 0u;
    ui_mng_notif_time_ref = millis();

    // Refresh full UI
    ui_mng_load_notification(ui_mng_notif_time_ref + UI_NOTIFICATION_REFRESH_PERIOD_MS);
}

void ui_mng_loop(void)
{
    uint64_t current_time = millis();

    background_sprite.fillSprite(TFT_BLACK);
    notification_sprite.fillSprite(TFT_BLACK);

    ui_mng_load_notification(current_time);

    background_sprite.drawString(ha_data, 0, 100, 4);

    lcd_PushColors(0, 0, UI_DISPLAY_WIDTH, UI_DISPLAY_HEIGHT, (uint16_t *)background_sprite.getPointer());
}

void ha_mqtt_cbk(char *topic, uint8_t *payload, uint32_t length)
{
    uint32_t last_index = (length < 50u) ? length: 50u;
    (void) strncpy(ha_data, (char *)payload, 50u);
    ha_data[last_index] = 0u;
    Serial.print("HA Message:");
    for (int i = 0; i < last_index; i++) {
        Serial.print((char) ha_data[i]);
    }
}

static void ui_mng_load_notification(
    uint64_t current_time
)
{
    // Update the notification panel sprite with the images
    notification_sprite.pushImage(164, 4, HA_32X32_FRAME_WIDTH, HA_32X32_FRAME_HEIGHT, &ha_32x32[ha_frame_index][0]);
    notification_sprite.pushImage(200, 0, WIFI_40X40_FRAME_WIDTH, WIFI_40X40_FRAME_HEIGHT, &wifi_40x40[wifi_frame_index][0]);
    notification_sprite.pushToSprite(&background_sprite, 0u, 0u);
    // Update the gif images after every refresh period
    if (current_time >= (ui_mng_notif_time_ref + UI_NOTIFICATION_REFRESH_PERIOD_MS)) {
        // Update the notification based on their status
        ui_mng_refresh_notification();
        // Update the reference time
        ui_mng_notif_time_ref = current_time;
    }
}

static void ui_mng_refresh_notification(void)
{
    // Display the frame according to WiFi connetion status
    if (wifi_mng_getstate() == WIFI_MNG_STATE_CONNECTED) {
        // Since WiFi is connected use the last frame
        wifi_frame_index = WIFI_40X40_NUM_FRAMES - 1u;
    }
    else {
        // update the gif frame index
        wifi_frame_index = (wifi_frame_index + 1u) < WIFI_40X40_NUM_FRAMES ? wifi_frame_index + 1u : 0u;
    }
    // Display the frame according to MQTT connection status
    if (mqtt_mng_getstate() == MQTT_MNG_STATE_CONNECTED) {
        ha_frame_index = 1u;
    }
    else {
        ha_frame_index = 0u;
    }
}
