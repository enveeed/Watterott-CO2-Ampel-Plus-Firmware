// time stuff
#include "time.h"
#include <TimeLib.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include "LED.h"

// === TIME ===

bool time_initialized = false;

// time: setup NTP and enable regular sync
void time_init() {
    ntp_init();
    time_initialized = true;
    
    // setup ntp sync
    setSyncProvider(ntp_get_time);
    setSyncInterval(300);
}

// time: time status
int time_status() {
    if(!time_initialized) {
        return TIME_STATUS_UNSET;
    } else {
        // todo: values may have shifted if timeNeedsSync is true
        if(!timeNotSet) return TIME_STATUS_SET;
        else return TIME_STATUS_INITIALIZED;
    }
}

// time: print time to serial
void time_print() {
    if(time_status() == TIME_STATUS_SET) {
        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
    } else {
        Serial.print("(unknown time)");
    }
}

// === NTP ===

// UDP connection to interface with the NTP server
WiFiUDP udp;
// local port to listen for UDP packets
unsigned int udp_local = 8888;

// NTP server name
static const char NTP_SERVER_NAME[] = "europe.pool.ntp.org";
// NTP time is in the first 48 bytes of message
static const int NTP_PACKET_SIZE = 48;

//buffer to hold packets
byte packetBuffer[NTP_PACKET_SIZE];

static const int time_zone = 1;     // Central European Time

//

// ntp: open port
void ntp_init() {
    udp.begin(udp_local);
}

// ntp: send request and obtain time
time_t ntp_get_time() {

    IPAddress ntp_server_ip; // NTP server's ip address

    while (udp.parsePacket() > 0) ; // discard any previously received packets

    Serial.println("Transmit NTP Request");

    // get a random server from the pool
    WiFi.hostByName(NTP_SERVER_NAME, ntp_server_ip);

    Serial.print(NTP_SERVER_NAME);
    Serial.print("was resolved to ");
    Serial.println(ntp_server_ip);

    ntp_send_packet(ntp_server_ip);

    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
        int size = udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
            Serial.println("Receive NTP Response");
            udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + time_zone * SECS_PER_HOUR;
        }
    }

    Serial.println("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

// ntp: build and send request packet
void ntp_send_packet(IPAddress &address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udp.beginPacket(address, 123); //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
}