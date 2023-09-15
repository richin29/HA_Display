/**
 * @file wifi_mng.h
 * @author Richin Abraham
 * @brief Manages the WiFi asynchronously.
 */

#ifndef WIFI_MNG_H
#define WIFI_MNG_H

// INCLUDES

// DEFINES

// TYPES

/** @brief Enumeration of different WiFi states. */
typedef enum {
    /** @brief WiFi is disconnected. */
    WIFI_MNG_STATE_DISCONNECTED,
    /** @brief WiFi connection in-progress. */
    WIFI_MNG_STATE_CONNECTING,
    /** @brief WiFi is connected. */
    WIFI_MNG_STATE_CONNECTED,
    /** @brief Error connecting to WiFi. */
    WIFI_MNG_STATE_ERROR
} wifi_mng_state_t;

// GLOBAL VARIABLES

// FUNCTIONS PROTOTYPES

/**
 * @brief Function to initialise the WiFi manager.
 */
void wifi_mng_init(void);

/**
 * @brief Function to be called periodically to manage the WiFi.
 */
void wifi_mng_loop(void);

/**
 * @brief Function to get the current WiFi state.
 *
 * @return wifi_mng_state_t.
 */
wifi_mng_state_t wifi_mng_getstate(void);

// FUNCTIONS

#endif // WIFI_MNG_H
