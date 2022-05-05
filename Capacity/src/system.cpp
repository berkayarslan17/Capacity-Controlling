#include "system.h"

// Initialize static class member instances
system_manager *system_manager::instance = nullptr;
std::vector<Command *> system_manager::command_queue;
int system_manager::max_device = 0;
int system_manager::range = 0;
SemaphoreHandle_t system_manager::ble_sem;

system_manager::system_manager()
{
    ble_sem = xSemaphoreCreateBinary();

    rc = xTaskCreatePinnedToCore(
        ble_task,
        "ble_task",
        10000, // Stack Size
        nullptr,
        2,       // Priortiy
        nullptr, // Task Handle
        1        // CPU
    );
    assert(rc == pdPASS);

    rc = xTaskCreatePinnedToCore(
        mqtt_task,
        "mqtt_task",
        10000, // Stack Size
        nullptr,
        2,       // Priority
        nullptr, // Task Handle
        1        // CPU
    );
    assert(rc == pdPASS);

    rc = xTaskCreatePinnedToCore(
        alarm_task,
        "alarm_task",
        1000, // Stack Size
        nullptr,
        1,       // Priortiy
        nullptr, // Task Handle
        1        // CPU
    );
    assert(rc == pdPASS);

    rc = xTaskCreatePinnedToCore(
        lcd_task,
        "lcd_task",
        5000, // Stack Size
        nullptr,
        2,       // Priortiy
        nullptr, // Task Handle
        1        // CPU
    );
    assert(rc == pdPASS);
}

system_manager::~system_manager()
{
}

system_manager *system_manager::get_instance()
{
    // Check if the static instance variable is null still
    // Referred to as "lazy instantiation" (don't create until needed)
    if (!instance)
    {
        // If not, create the first singleton instance ...
        instance = new system_manager();
    }

    // ... and return it
    return instance;
}
