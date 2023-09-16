/**
 * @file mqtt_mng.h
 * @author Richin Abraham
 * @brief Manages the MQTT asynchronously.
 */

#ifndef MQTT_MNG_H
#define MQTT_MNG_H

// INCLUDES

// DEFINES

// TYPES

/** @brief Enumeration of different MQTT states. */
typedef enum {
    /** @brief MQTT is disconnected. */
    MQTT_MNG_STATE_DISCONNECTED,
    /** @brief MQTT connection in-progress. */
    MQTT_MNG_STATE_CONNECTING,
    /** @brief MQTT is connected. */
    MQTT_MNG_STATE_CONNECTED,
    /** @brief Error connecting to MQTT. */
    MQTT_MNG_STATE_ERROR
} mqtt_mng_state_t;

// GLOBAL VARIABLES

// FUNCTIONS PROTOTYPES

/**
 * @brief Function to initialise the MQTT manager.
 */
void mqtt_mng_init(void);

/**
 * @brief Function to be called periodically to manage the MQTT.
 */
void mqtt_mng_loop(void);

/**
 * @brief Function to get the current MQTT state.
 *
 * @return mqtt_mng_state_t.
 */
mqtt_mng_state_t mqtt_mng_getstate(void);

// FUNCTIONS

#endif // MQTT_MNG_H
