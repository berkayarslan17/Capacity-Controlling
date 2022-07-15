# Capacity Controlling System

## Introduction

![resim](https://user-images.githubusercontent.com/44584158/179208041-d5ff46cb-3f74-4fef-917b-4b0be096608b.png)

The subject of this project is to make a system that measures how many people are in the room. The ESP32 microcontroller card will observe the bluetooth signals in the room and record the data coming from the bluetooth devices to its system and try to determine how many bluetooth signal generating devices are in the room. Since the devices producing the Bluetooth signal will be above the people, the number of people in the room will also be measured. This project was conceived to prevent the spread of the recently emerged COVID-19 virus.

## System Design
![resim](https://user-images.githubusercontent.com/44584158/179205679-12510eec-4b8c-4bad-ae55-5f278cba7f41.png)
## Schematic Design
![resim](https://user-images.githubusercontent.com/44584158/179205905-2d5d62c7-217c-4beb-8b2e-97a1effae64a.png)
## LCD Screen
![resim](https://user-images.githubusercontent.com/44584158/179205989-35dd2d09-36c6-442a-bace-87f2795c4ea1.png)

The steps can be listed as follows:

1) Activate the IDLE Screen.
2) Connect to the Internet.
3) Activate the tracking screen.
4) Start the Bluetooth scan.
5) Increase the counter when the device enters the room.
6) Decrease the counter when the device leaves the room.
7) Activate the alarm screen when the counter reaches the maximum capacity value.
8) Activate the buzzer.
9) Switch to the tracking screen when the device leaves the room.
10) Decrease the counter.
11) Turn off the buzzer.

## Beacon's Finite State Machine
![resim](https://user-images.githubusercontent.com/44584158/179206454-4ec41d5a-7681-4840-a3e6-5b91d16a7431.png)

The steps can be listed as follows:

1) When the beacon is detected, initialize it.
2) Compare the RSSI value from the beacon with the value from the internet, if the system decides that the beacon is inside the room, assign it to the input status.
3) Compare the RSSI value from the beacon with the value from the internet, if the system decides that the beacon is outside the room, assign it to the output status.

## Weighted Moving Average Filter
In order for the system to understand the exit from the room faster, a weighted moving average filter is used instead of the normal filter. In this way, the calculation was made in such a way that the weight of the latest data received was the highest, and the response speed of the system against possible situations was increased.

![resim](https://user-images.githubusercontent.com/44584158/179206718-2ac20603-3d1a-49b6-9b85-d5c80d504338.png)

## Using IoT
The capacity control system can be controlled remotely regarding to the data it receives from the internet. It can provide room control according to different room size and capacity management needs. There are two different types of data it receives from the Internet. This data is reached under two headings. These topics can be explained as follows:

### Maximum Device Topic
Maximum device data is the data that takes a value between [1,5] and tells how many beacons the system should give an alarm when it detects in the room.

### Range Topic
The range data takes a value between [-50, -100] and compares the signal from the beacon with its own data and allows the system to decide whether the beacon is in the room or not.

![resim](https://user-images.githubusercontent.com/44584158/179207204-3ff210f9-aa0e-424e-b11e-c60fc9add0a0.png)

![resim](https://user-images.githubusercontent.com/44584158/179207254-84890502-7e95-4466-8a5b-ec1a3d31589a.png)

## Conclusion
The Capacity Control System project has been successfully completed as a system that can monitor RSSI-based devices in the room in real time, can be controlled via the website or mobile application, and can give an alarm when the maximum human capacity is reached in the room. In order to realize these, Bluetooth and WiFi modules of the ESP32 microcontroller card were used. Since it has different tasks from each other, task division has been made on the system by using the FreeRTOS operating system, code readability and the speed of executing commands of the device have been increased. By using a weighted average filter on RSSI signals, the noise on the signal is reduced and the device is able to make more accurate decisions about room tracking. Finally, on the internet connection side, the device can be controlled over the internet with the MQTT Broker provided by Ubidots.


