/**
 * @file mqtt_mng.cpp
 * @author Richin Abraham
 * @brief Manages the MQTT asynchronously.
 */

// INCLUDES

#include "mqtt_mng.h"
#include "mqtt_config.h"
#include <PubSubClient.h>
#include "wifi_mng.h"

// DEFINES

// TYPES

// GLOBAL VARIABLES

/** @brief Instance of WiFi client. */
WiFiClient wifi_client;
/** @brief Instance of MQTT client. */
PubSubClient mqtt_client(wifi_client);
/** @brief Variable to store the state of the MQTT. */
mqtt_mng_state_t mqtt_mng_state = MQTT_MNG_STATE_DISCONNECTED;
/** @brief Variable to store the number of errors in MQTT connection. */
uint8_t mqtt_mng_error_count;
/** @brief Configuration structure for subscribers. */
extern mqtt_sub_t mqtt_sub_config[MQTT_NUM_SUBSCRIBERS];

// FUNCTIONS PROTOTYPES

/**
 * @brief Callback function for receiving MQTT messages.
 *
 * @param topic Topic of the message.
 * @param payload Data of the message.
 * @param length Length of the message.
 */
static void mqtt_mng_callback(
    char *topic,
    uint8_t *payload,
    uint32_t length
);

/**
 * @brief Function to subscribe all configured topics.
 */
static void mqtt_mng_subscribe(void);

// FUNCTIONS

void mqtt_mng_init(void)
{
    // Configure the MQTT
    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(mqtt_mng_callback);
    // Reset the variables
    mqtt_mng_error_count = 0u;
    mqtt_mng_state = MQTT_MNG_STATE_DISCONNECTED;
}

void mqtt_mng_loop(void)
{
    bool connected;
    wifi_mng_state_t wifi_state;
    const String client_id = "t3-amoled-" + String(WiFi.macAddress());

    // Run the library
    mqtt_client.loop();
    // Get the updated status
    connected = mqtt_client.connected();

    switch (mqtt_mng_state) {
    case MQTT_MNG_STATE_DISCONNECTED:
        wifi_state = wifi_mng_getstate();

        // Wait for WiFi to be connected before starting MQTT server connection
        if (wifi_state == WIFI_MNG_STATE_CONNECTED) {
            // Start connecting to the MQTT server
            mqtt_mng_state = MQTT_MNG_STATE_CONNECTING;
        }
        break;

    case MQTT_MNG_STATE_CONNECTING:
        if (connected == true) {
            // Connection persisted
            Serial.println("MQTT connection reused");
            mqtt_mng_state = MQTT_MNG_STATE_CONNECTED;
        }
        else if (mqtt_client.connect(client_id.c_str(), MQTT_USER, MQTT_PASS) == true) {
            Serial.println("MQTT connected");
            // Configure all the subsribers
            mqtt_mng_subscribe();
            mqtt_mng_error_count = 0u;
            mqtt_mng_state = MQTT_MNG_STATE_CONNECTED;
        }
        else {
            mqtt_mng_error_count++;

            if (mqtt_mng_error_count >= MQTT_MAX_ERROR_COUNT) {
                // Abort the process as error count exceeded the limit
                Serial.print("MQTT error, aborted! State: ");
                Serial.println(mqtt_client.state());
                mqtt_mng_state = MQTT_MNG_STATE_ERROR;
            }
        }
        break;

    case MQTT_MNG_STATE_CONNECTED:
        // Monitor for disconnections
        if (connected != true) {
            Serial.println("MQTT connection lost");
            mqtt_mng_state = MQTT_MNG_STATE_DISCONNECTED;
        }
        break;

    case MQTT_MNG_STATE_ERROR:
    default:
        // Nothing to do as of now
        break;
    }
}

mqtt_mng_state_t mqtt_mng_getstate(void)
{
    return mqtt_mng_state;
}

static void mqtt_mng_callback(
    char *topic,
    uint8_t *payload,
    uint32_t length
)
{
    uint16_t sub_index;
    uint32_t data_index;
    Serial.print("MQTT topic:");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    for (sub_index = 0u; sub_index < MQTT_NUM_SUBSCRIBERS; sub_index++) {
        if (strcmp(mqtt_sub_config[sub_index].topic, topic) == 0) {
            mqtt_sub_config[sub_index].callback(topic, payload, length);
            break;
        }
    }
}

static void mqtt_mng_subscribe(void)
{
    uint16_t i;

    for (i = 0u; i < MQTT_NUM_SUBSCRIBERS; i++) {
        mqtt_client.subscribe(mqtt_sub_config[i].topic);
    }
}
