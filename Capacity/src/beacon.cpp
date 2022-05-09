#include <Arduino.h>
#include "beacon.h"
#include "system.h"

Beacon::Beacon()
{
  // mac_addr = "";
  rssi_val = 0;
  filtered_rssi_val = 0;
  memset(rssi_buffer, 0, sizeof(rssi_buffer) / sizeof(rssi_buffer[0]));
  state = BEACON_STATE_INITIAL;
}

Beacon::Beacon(const char *device_addr)
{
  strncpy(mac_addr, device_addr, strlen(device_addr) + 1);
  memset(rssi_buffer, 0, sizeof(rssi_buffer) / sizeof(rssi_buffer[0]));
}

// Beacon::~Beacon()
//{
// }

void Beacon::save_rssi_val(int raw_rssi_val)
{
  static int circ_idx, norm_idx;

  rssi_val = raw_rssi_val;

  // Save the value and increase the index
  rssi_buffer[circ_idx] = raw_rssi_val;

  circ_idx = (circ_idx + 1) % 5;
  norm_idx++;

  if (norm_idx <= 5)
  {
    /*** MOVING AVERAGE FILTER ***/
    filtered_rssi_val = mov_average_filter(rssi_buffer, norm_idx);
  }

  else
  {
    filtered_rssi_val = mov_average_filter(rssi_buffer, BEACON_UNIT);
  }

  check_beacon_range();

  // Check the filtered rssi value, if it is in the range, make the counter
  // up for indicating in the LCD screen
}

void Beacon::save_device_addr(const char *device_addr)
{
  // Save the address
  strncpy(mac_addr, device_addr, strlen(device_addr) + 1);
}

/* Find the index of the beacon according to its device_addr
 * and return that index. If it doesn't exist, return -1
 */
int find_beacon(Beacon beacon_list[], const char *device_addr)
{
  Serial.print("device_addr: ");
  Serial.println(device_addr);
  for (int index = 0; index < BEACON_UNIT; index++)
  {
    Serial.print("beacon_list[index], ");
    Serial.print(index);
    Serial.print("mac_addr: ");
    Serial.print(beacon_list[index].mac_addr);
    if (!strcmp(beacon_list[index].mac_addr, device_addr))
    {
      return index;
    }
  }
  return -1;
}

void Beacon::beacon_dispatch(event_t *evt)
{
  switch (state)
  {
  case BEACON_STATE_INITIAL:
  {
    switch (evt->sig)
    {
    // If the beacon is inside, switch to IN_RANGE state
    case BEACON_SIG_IN_RANGE:
    {
      state = BEACON_STATE_IN;
      CntUpCommand *cmd = new CntUpCommand;
      system_manager::add_command_queue(cmd);
      Serial.println("State Initial - In Range");
      break;
    }

    // If the beacon is outside, switch to OUT_RANGE state
    case BEACON_SIG_OUT_RANGE:
    {
      state = BEACON_STATE_OUT;
      Serial.println("State Initial - Out Range");
      break;
    }
    default:
    {
      Serial.println("Unknown event");
      break;
    }
    }
    break;
  }

  case BEACON_STATE_IN:
  {
    switch (evt->sig)
    {
    // If the beacon is inside, switch to IN_RANGE state
    case BEACON_SIG_IN_RANGE:
    {
      // Do nothing
      break;
    }

    // If the beacon is outside, switch to OUT_RANGE state
    case BEACON_SIG_OUT_RANGE:
    {
      state = BEACON_STATE_OUT;
      // Make LCD counter down
      CntDownCommand *cmd = new CntDownCommand;
      system_manager::add_command_queue(cmd);
      Serial.println("State IN - Out Range");
      break;
    }

    default:
    {
      // printk("Unknown event: %u", evt->sig);
      break;
    }
    }
    break;
  }

  case BEACON_STATE_OUT:
  {
    switch (evt->sig)
    {
    // If the beacon is inside, switch to IN_RANGE state
    case BEACON_SIG_IN_RANGE:
    {
      state = BEACON_STATE_IN;
      // Make LCD counter up
      CntUpCommand *cmd = new CntUpCommand;
      system_manager::add_command_queue(cmd);
      Serial.println("State OUT - IN Range");
      break;
    }

    // If the beacon is outside, switch to OUT_RANGE state
    case BEACON_SIG_OUT_RANGE:
    {
      // Do nothing
      break;
    }

    default:
    {
      Serial.println("Unknown event");
      break;
    }
    }
    break;
  }
  }
}

void Beacon::check_beacon_range(void)
{
  if (filtered_rssi_val > system_manager::range)
  {
    // Inside the range
    event_t evt = {.sig = BEACON_SIG_IN_RANGE};
    beacon_dispatch(&evt);
  }

  else
  {
    // Outside the range
    event_t evt = {.sig = BEACON_SIG_OUT_RANGE};
    beacon_dispatch(&evt);
  }
}

void print_beacon(const char *device_addr, int rssi_val, unsigned int index)
{
  Serial.print("Device ");
  Serial.print(index);
  Serial.print(", Address: ");
  Serial.print(device_addr);
  Serial.print(", RSSI Val: ");
  Serial.print(rssi_val);
  Serial.println();
}
