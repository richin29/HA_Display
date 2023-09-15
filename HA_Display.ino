/**
 * @file      HA_Display.ino
 * @author    Richin Abraham
 */

#include "ui_mng.h"
#include "wifi_mng.h"
#include <WiFi.h> // For MQTT
#include <PubSubClient.h>
#include "board_validation.h"
#include "secrets.h"
#include "config.h"

// DEFINES


// GLOBAL VARIABLES
/** @brief Instance of WiFi client. */
WiFiClient wifi_client;
/** @brief Instance of MQTT client. */
PubSubClient mqtt_client(wifi_client);
/** @brief Variable to store the main loop count. */
uint64_t loop_count;

// FUNCTIONS

static void connect_wifi(void)
{
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}

static void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

static void connect_mqtt(void)
{
    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(callback);
    while (!mqtt_client.connected()) {
        String client_id = "t3-amoled-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the HA mqtt broker\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), MQTT_USER, MQTT_PASS)) {
            Serial.println("HA mqtt broker connected");
        }
        else {
            Serial.print("failed with state ");
            Serial.print(mqtt_client.state());
            delay(2000);
        }
    }

    mqtt_client.subscribe(MQTT_HA_TOPIC);
}

void setup()
{
    // Use TFT_eSPI Sprite made by framebuffer , unnecessary calling during use tft.xxxx function
    Serial.begin(115200);
    ui_mng_init();
    wifi_mng_init();
    //connect_mqtt();
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
    //mqtt_client.loop();
}