#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

/* WiFi setup Change Yours*/
#define WIFI_SSID "Androido"
#define WIFI_PASSWORD "rengarkedi"

/* You can change but you dont need to*/
#define MQTT_SERVER_NAME "things.ubidots.com"
#define MQTT_PORT 1883
#define TOKEN "BBFF-X2Os0zijDXBvbnqcPMIAFFgi0sZOj7"

/* ALWAYS CHANGE!!*/
#define CLIENT_ID "BerkayArslan"

void reconnectToTheBroker(void);
void connectToWiFi(void);
void callback(char *topic, byte *payload, unsigned int length);
void setup_mqtt(void);
void mqtt_loop(void);

#endif
