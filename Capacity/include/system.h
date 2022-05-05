#ifndef SYSTEM_H
#define SYSTEM_H

// custom lib
#include "ble.h"
#include "mqtt.h"
#include "lcd.h"

class system_manager
{
public:
    static system_manager *get_instance();
    static int range;
    static int max_device;
    static SemaphoreHandle_t ble_sem;

    system_manager();
    ~system_manager();

    /** @brief BLE Task Manager
     */
    static void ble_task(void *argp)
    {
        // SETUP BLE
        ble_scanner_setup();

        // Take the ble semaphore
        xSemaphoreTake(system_manager::ble_sem);
        Serial.println("Start ble activity");
        for (;;)
        {
            // SCANNER LOOP
            ble_scanner_loop();
        }
    }

    /** @brief MQTT Task Manager
     */
    static void mqtt_task(void *argp)
    {
        connectToWiFi();
        setup_mqtt();

        Serial.println("WiFi setup intitiating..");
        for (;;)
        {
            mqtt_loop();
        }
    }

    /** @brief Alarm Task Manager
    /*  @TODO: get the max_device variable, if the beacon_list index is bigger
     *         than the max_device, trigger the alarm.
     */
    static void alarm_task(void *argp)
    {
        Serial.println("Starting alarm task...");
        for (;;)
        {
        }
    }

    /** @brief LCD Task Manager
    /*
     */
    static void lcd_task(void *argp)
    {
        Serial.println("Starting LCD task...");
        lcd_manager screen;

        for (;;)
        {
            for (int i = 0; i < command_queue.size(); i++)
            {
                Serial.println("Execute command...");
                command_queue[i]->execute(screen);
            }
            command_queue.clear();
        }
    }

    static void add_command_queue(Command *cmd)
    {
        command_queue.push_back(cmd);
    }

private:
    BaseType_t rc;
    static system_manager *instance;
    static std::vector<Command *> command_queue;
};
#endif
