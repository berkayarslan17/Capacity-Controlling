#ifndef SYSTEM_H
#define SYSTEM_H

// custom lib
#include "ble.h"
#include "mqtt.h"
#include "lcd.h"
#include "buzzer.h"

class system_manager
{
public:
    static system_manager *get_instance();
    static int range;
    static int max_device;
    static TaskHandle_t ble_task_handle;
    static TaskHandle_t lcd_task_handle;
    static SemaphoreHandle_t lcd_sem;

    system_manager();
    ~system_manager();

    /** @brief BLE Task Manager
     */
    static void ble_task(void *argp)
    {
        // SETUP BLE
        ble_scanner_setup();

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

    /** @brief LCD Task Manager
    /*
     */
    static void lcd_task(void *argp)
    {
        Serial.println("Starting LCD task...");
        lcd_manager screen;

        for (;;)
        {
            xSemaphoreTake(system_manager::lcd_sem, portMAX_DELAY);
            Serial.println("Take the semaphore");

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
        xSemaphoreGive(system_manager::lcd_sem);
        Serial.println("Give the semaphore");
    }

private:
    BaseType_t rc;
    static system_manager *instance;
    static std::vector<Command *> command_queue;
};
#endif
