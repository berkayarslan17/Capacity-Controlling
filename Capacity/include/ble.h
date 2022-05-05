// BLEScanner.h

#ifndef BLE_H
#define BLE_H

#include <memory>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>

#include "beacon.h"

#define SCAN_TIME 5

void ble_scanner_setup(void);
void ble_scanner_loop(void);

#endif
