#include "system.h"

WiFiClient ubidots;
PubSubClient mqttClient(ubidots);

void reconnectToTheBroker(void)
{
    int numberOfConnectionsTried = 0;
    while (!mqttClient.connected())
    {
        Serial.println("Reconnecting to MQTT Broker..");
        if (mqttClient.connect(CLIENT_ID, TOKEN, ""))
        {
            Serial.println("MQTT Broker Connected.");
            // subscribe to topic
            mqttClient.subscribe("/v1.6/devices/capacity/max-device/lv");
            mqttClient.subscribe("/v1.6/devices/capacity/range/lv");
            vTaskResume(system_manager::ble_task_handle);
            Serial.println("Resume ble task");
            // vTaskResume(system_manager::lcd_task_handle);
            // Serial.println("Resume ble task");
        }
        else
        {
            // MQTT Could not reconnect, wifi/esp32 error
            Serial.print("Connection failed, rc=");
            Serial.print(mqttClient.state());
            numberOfConnectionsTried++;
            if (numberOfConnectionsTried > 5)
            {
                Serial.print("Rebooting the device...");
                ESP.restart();
            }
        }
        delay(500);
    }
}

void connectToWiFi(void)
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.print("Connected to the WiFi.");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    char message_temp[length + 1];
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        message_temp[i] = payload[i];
    }
    message_temp[length] = '\0';
    Serial.print("\n");

    if (strcmp(topic, "/v1.6/devices/capacity/max-device/lv") == 0)
    {
        system_manager::max_device = atoi(message_temp);
        Serial.println(system_manager::max_device);

        // Refresh max device
        TrackingCommand *cmd = new TrackingCommand;
        system_manager::add_command_queue(cmd);
    }

    else if (strcmp(topic, "/v1.6/devices/capacity/range/lv") == 0)
    {
        system_manager::range = atoi(message_temp);
        system_manager::range *= -1;
        Serial.println(system_manager::range);
    }
}

void setup_mqtt(void)
{
    mqttClient.setServer(MQTT_SERVER_NAME, MQTT_PORT);
    // set the callback function
    mqttClient.setCallback(callback);
    // mqttClient.setKeepAlive(60);
}

void mqtt_loop(void)
{
    if (!mqttClient.connected())
    {
        Serial.println("Reconnecting to the broker..");
        reconnectToTheBroker();
    }

    // Serial.println("Publishing data to Ubidots Cloud");
    //    mqttClient.publish(topic, payload);
    mqttClient.loop();
    //    delay(1000);
}
