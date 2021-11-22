#include <TimeLib.h>
#include <WiFi101.h>

// time

// time: status
static const int TIME_STATUS_UNSET          = -1;
static const int TIME_STATUS_INITIALIZED    = 0;
static const int TIME_STATUS_SET            = 1;

// time: setup NTP and enable regular sync
void time_init();
// time: time status
int time_status();
// time: print time to serial
void time_print();

// ntp

// ntp: open port
void ntp_init();
// ntp: send request and obtain time
time_t ntp_get_time();
// ntp: build and send request packet
void ntp_send_packet(IPAddress &address);