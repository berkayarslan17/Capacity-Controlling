#include "system.h"

//std::vector<Beacon> beacon_list;
Beacon beacon_list[BEACON_UNIT];
BLEScan *pBLEScan;
//BLEAddress *pMAC_Address;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    //Beacon *Indicator;
    //pMAC_Address = new BLEAddress(advertisedDevice.getAddress());

    if (strcmp(advertisedDevice.getAddress().toString().c_str(), "ff:06:22:b0:03:f6") == 0)
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
        Serial.println("New beacon is founded. Saving to the system...");
        // Save it's Device Address
        beacon_list[index].save_device_addr(advertisedDevice.getAddress().toString().c_str());
        // Save it's RSSI value
        beacon_list[index].save_rssi_val(raw_rssi_val);
        print_beacon(advertisedDevice.getAddress().toString().c_str(), raw_rssi_val, index);
        index++;
      }
      else
      {
        //Serial.println("The beacon is known. Saving its RSSI value...");
        beacon_list[beacon_index].save_rssi_val(raw_rssi_val);
        print_beacon(advertisedDevice.getAddress().toString().c_str(), raw_rssi_val, beacon_index);
      }
    }
  }
};

void ble_scanner_setup(void)
{
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void ble_scanner_loop(void)
{
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
//  delete pMAC_Address;
//  pMAC_Address = NULL;
}
