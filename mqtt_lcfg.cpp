/**
 * @file mqtt_config.cpp
 * @author Richin Abraham
 * @brief Link time configuration of MQTT.
 */

// INCLUDES

#include "mqtt_config.h"
#include "ui_mng.h"

// DEFINES

// TYPES

// GLOBAL VARIABLES

mqtt_sub_t mqtt_sub_config[MQTT_NUM_SUBSCRIBERS] = {
    // topic                 , callback
    {  "homeassistant/info/" , &ha_mqtt_cbk }
};

// FUNCTIONS PROTOTYPES

// FUNCTIONS
