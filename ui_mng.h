/**
 * @file ui_mng.h
 * @author Richin Abraham
 * @brief Manages the user interface.
 */

#ifndef UI_MNG_H
#define UI_MNG_H

// INCLUDES

// DEFINES

// Values to be selected for UI_DISPLAY_ORIENTATION configuration
/** @brief The screen will in portait mode. */
#define UI_DISPLAY_ORIENTATION_PORTRAIT     0u
/** @brief The screen will in landscape mode. */
#define UI_DISPLAY_ORIENTATION_LANDSCAPE    1u

// TYPES

// GLOBAL VARIABLES

// FUNCTIONS PROTOTYPES

/**
 * @brief Function to initialise the user interface.
 */
void ui_mng_init(void);

/**
 * @brief Function to be called periodically to manage the user interface.
 */
void ui_mng_loop(void);

void ha_mqtt_cbk(char *topic, uint8_t *payload, uint32_t length);

// FUNCTIONS

#endif // UI_MNG_H
