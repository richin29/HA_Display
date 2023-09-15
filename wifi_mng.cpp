/**
 * @file wifi_mng.cpp
 * @author Richin Abraham
 * @brief Manages the WiFi asynchronously.
 */

// INCLUDES

#include "wifi_mng.h"
#include "wifi_config.h"
#include <WiFi.h>
#include "secrets.h"

// DEFINES

// TYPES

// GLOBAL VARIABLES

/** @brief Variable to store the state of the WiFi. */
wifi_mng_state_t wifi_mng_state = WIFI_MNG_STATE_DISCONNECTED;
/** @brief Variable to store the time reference in milliseconds. */
uint64_t wifi_mng_time_ref;

// FUNCTIONS PROTOTYPES

// FUNCTIONS

void wifi_mng_init(void)
{
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    wifi_mng_time_ref = millis();
    wifi_mng_state = WIFI_MNG_STATE_CONNECTING;
    Serial.println("Connecting to WiFi..");
}

void wifi_mng_loop(void)
{
    wl_status_t status = WiFi.status();
    uint64_t current_time = millis();

    switch (wifi_mng_state) {
    case WIFI_MNG_STATE_DISCONNECTED:
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        wifi_mng_time_ref = current_time;
        wifi_mng_state = WIFI_MNG_STATE_CONNECTING;
        break;

    case WIFI_MNG_STATE_CONNECTING:
        if (status == WL_CONNECTED) {
            Serial.println("Connected to WiFi");
            wifi_mng_state = WIFI_MNG_STATE_CONNECTED;
        }
        else if ((status == WL_IDLE_STATUS) || (status == WL_DISCONNECTED)){
            if ((current_time - wifi_mng_time_ref) >= 500u) {
                Serial.println("Connecting to WiFi...");
                wifi_mng_time_ref = current_time;
            }
        }
        else {
            Serial.print("Connection to WiFi failed:");
            Serial.println(status, DEC);
            wifi_mng_state = WIFI_MNG_STATE_ERROR;
        }
        break;

    case WIFI_MNG_STATE_CONNECTED:
        if ((status == WL_CONNECTION_LOST) || (status == WL_DISCONNECTED)) {
            wifi_mng_state = WIFI_MNG_STATE_DISCONNECTED;
        }
        break;

    case WIFI_MNG_STATE_ERROR:
    default:
        // Nothing to do as of now
        break;
    }
}

wifi_mng_state_t wifi_mng_getstate(void)
{
    return wifi_mng_state;
}
