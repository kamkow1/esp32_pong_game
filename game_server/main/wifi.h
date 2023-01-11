#ifndef WIFI_H
#define WIFI_H

#include <stdint.h>
#include "esp_event.h"

#define TAG                 "wifi_station"
#define WIFI_MAX_RETRY      5
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void wifi_init_sta();
void start_wifi();

#endif
