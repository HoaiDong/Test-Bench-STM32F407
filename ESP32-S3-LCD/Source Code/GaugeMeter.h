#ifndef GAUGE_METER_H
#define GAUGE_METER_H
#include <stdint.h>

void ui_throttle_meter_create(void);
void ui_throttle_meter_update(uint8_t throttle_value);

#endif