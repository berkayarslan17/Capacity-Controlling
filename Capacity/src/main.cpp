#include <Arduino.h>
#include <Wire.h>
#include "system.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Capacity Control System...");
  
  // Creates a new singleton, and returns a pointer to it
  system_manager *sys = system_manager::get_instance();
}

void loop()
{
  // Delete loop task.
  vTaskDelete(nullptr);
}