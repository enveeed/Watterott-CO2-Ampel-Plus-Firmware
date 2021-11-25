#include "timer.h"
#include <TimeLib.h>
#include "time.h"
#include "DeviceConfig.h"
#include "LED.h"
#include "Buzzer.h"

// timer

// timer: init and load config values
void timer_init() {}

// timer: loop
void timer_loop() {
  if (time_status() == TIME_STATUS_SET) {
    Serial.println("timer loop bc time is set");
    device_config_t cfg = config_get_values();
    if(cfg.light_mode == TIMER_MODE_TIMER) {
      Serial.println("light mode is timer");
      int now = (hour()*60 + minute());
      Serial.print("time now = ");
      Serial.print(now);
      Serial.print("  time on = ");
      Serial.print(cfg.light_timer.time_on);
      Serial.print("  time off = ");
      Serial.println(cfg.light_timer.time_off);
      if (now > cfg.light_timer.time_on && now < cfg.light_timer.time_off) {
        // ensure enabled
        Serial.println("ensure on");
        led_enabled = true;
      } else {
        // ensure disabled
        Serial.println("ensure off");
        if(led_enabled) led_off();
        led_enabled = false;
      }
    }
    if(cfg.buzzer_mode == TIMER_MODE_TIMER) {
      Serial.println("buzzer mode is timer");
      if ((hour()*60 + minute()) > cfg.buzzer_timer.time_on && (hour()*60 + minute()) < cfg.buzzer_timer.time_off) {
        // ensure enabled
        buzzer_enabled = true;
      } else {
        // ensure disabled
        if(buzzer_enabled) buzzer_off();
        buzzer_enabled = false;
      }
    }
  }
}