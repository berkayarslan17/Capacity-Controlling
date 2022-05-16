#include "system.h"

Beacon beacon_list[BEACON_UNIT];
BLEScan *pBLEScan;
// TODO: Change the MAC Adresses
const char mac_addr_table[BEACON_UNIT][18] = {{"40:ed:98:ee:aa:32"},
                                              {"40:ed:98:ec:aa:32"},
                                              {"40:ed:98:ee:aa:32"},
                                              {"40:ed:98:ee:aa:32"},
                                              {"40:ed:98:ee:aa:32"}};

static bool check_beacon_list(const char *mac_addr)
{
    for (size_t i = 0; i < BEACON_UNIT; i++)
    {
        if(!strcmp(mac_addr_table[i], mac_addr))
        {
            return true;
        }
    }
    return false;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (check_beacon_list(advertisedDevice.getAddress().toString().c_str()))
    {
      int raw_rssi_val = advertisedDevice.getRSSI();
      static unsigned int index = 0;

      /* Search the beacon's MAC, if it is a unique one, save it to memory.
       * If returns false, the adress is unique, save the beacon and save it's RSSI value to buffer.
       * If returns true, the adress is not unique, find the right beacon object and save it's RSSI value to buffer.
       */

      int beacon_index = find_beacon(beacon_list, advertisedDevice.getAddress().toString().c_str());

      if (beacon_index == -1)
      {
        Serial.println("New beacon is found. Saving to the system...");
        // Save it's Device Address
        beacon_list[index].save_device_addr(advertisedDevice.getAddress().toString().c_str());
        // Save it's RSSI value
        beacon_list[index].save_rssi_val(raw_rssi_val);
        print_beacon(advertisedDevice.getAddress().toString().c_str(), raw_rssi_val, index);
        index++;
      }
      else
      {
        // Serial.println("The beacon is known. Saving its RSSI value...");
        beacon_list[beacon_index].save_rssi_val(raw_rssi_val);
        print_beacon(advertisedDevice.getAddress().toString().c_str(), raw_rssi_val, beacon_index);
        Serial.print("filtered rssi val ");
        Serial.println(beacon_list[beacon_index].filtered_rssi_val);
      }
    }
  }
};

void ble_scanner_setup(void)
{
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(1249); // less or equal setInterval value
}

void ble_scanner_loop(void)
{
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  //  delete pMAC_Address;
  //  pMAC_Address = NULL;
}
