#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H
#include <FlashStorage.h>

typedef struct {
  // minutes of the day (0 - 1440)
  int time_on;
  int time_off;
} device_config_timer_t;

typedef struct {
  int change_count;
  char wifi_ssid[40];
  char wifi_password[40];
  char ap_password[40];
  int mqtt_broker_port;
  char mqtt_broker_address[20];
  char mqtt_topic[20];
  char ampel_name[40];
  float temperature_offset;
  char mqtt_username[20];
  char mqtt_password[20];
  int mqtt_format;
  int light_mode;
  int buzzer_mode;
  device_config_timer_t light_timer;
  device_config_timer_t buzzer_timer;
} device_config_t;

void config_set_factory_defaults();
bool config_is_initialized();
device_config_t config_get_values();
void config_set_values(device_config_t new_config);

#endif
