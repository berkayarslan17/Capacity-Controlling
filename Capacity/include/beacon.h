#ifndef BEACON_H
#define BEACON_H

#include "filter.h"
#include "lcd.h"

#define BEACON_UNIT 5

typedef enum beacon_signals
{
  /** @brief BEACON_SIG_IN_RANGE   -> Signal that indicates the stretcher is inside the range .
   */
  BEACON_SIG_IN_RANGE,

  /** @brief BEACON_SIG_OUT_RANGE  -> Signal that indicates the beacon is outside the range.
   */
  BEACON_SIG_OUT_RANGE,
} beacon_sig_t;

typedef enum beacon_states
{
  /** @brief INITIAL_STATE -> When the beacon initialized, it's state will begin here.
   *
   */
  BEACON_STATE_INITIAL,

  /** @brief IN_STATE      ->  If the beacon in the range, switch to IN_STATE
   *
   */
  BEACON_STATE_IN,

  /** @brief OUT_STATE     -> If the beacon is not in the range, switch to OUT_STATE
   *
   */
  BEACON_STATE_OUT,
} beacon_state_t;

typedef struct event_tag
{
  beacon_sig_t sig;
} event_t;

class Beacon
{
public:
  char mac_addr[17];
  int rssi_buffer[4];
  int filtered_rssi_val;
  int rssi_val;
  beacon_state_t state;
  bool write_lcd;

  // Constructors and destructor
  Beacon();                     // Default constructor
  Beacon(const char *mac_addr); // Default constructor

  void save_rssi_val(int raw_rssi_val);
  void save_device_addr(const char *device_addr);
  void beacon_dispatch(event_t *evt);
  void check_beacon_range(void);
};

int find_beacon(Beacon beacon_list[], const char *device_addr);
void print_beacon(const char *device_addr, int rssi_val, unsigned int index);
#endif
