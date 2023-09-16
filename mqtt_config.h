
/**
 * @file mqtt_config.h
 * @author Richin Abraham
 * @brief MQTT configuration.
 */

#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

// INCLUDES

#include <stdint.h>
#include "secrets.h"

// DEFINES

/** @brief Specify the broker for MQTT connection. */
#define MQTT_BROKER             "192.168.0.13"

/** @brief Specify the port for MQTT connection. */
#define MQTT_PORT               1883

/** @brief Specify the SSID of the MQTT broker. */
#define MQTT_USER               SECRET_MQTT_USER

/** @brief Specify the password of the MQTT broker. */
#define MQTT_PASS               SECRET_MQTT_PASS

/** @brief Specify the maximum number of errors to be detected before aborting the process. */
#define MQTT_MAX_ERROR_COUNT    10u

/** @brief Specify the maximum length of the MQTT topic. */
#define MQTT_MAX_TOPIC_LEN      255u

/** @brief Specify the number of subscribers. */
#define MQTT_NUM_SUBSCRIBERS    1

// TYPES

/** @brief Type of MQTT callback function. */
typedef void (*mqtt_sub_cbk_t)(char *topic, uint8_t *payload, uint32_t length);

/** @brief Structure type for MQTT subscriber configuration. */
typedef struct {
    /** @brief MQTT subscriber topic. */
    char topic[MQTT_MAX_TOPIC_LEN];
    /** @brief MQTT subscriber callback function. */
    mqtt_sub_cbk_t callback;
} mqtt_sub_t;

#endif // MQTT_CONFIG_H
