#ifndef __CLOCK_H
#define __CLOCK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <TimeLib.h>
#include <WiFiUdp.h> 

typedef struct 
{
  int Hour;
  int Minute;
  int Second;
  uint8_t Flag;
}Clock_t;

extern Clock_t Clock;
extern WiFiUDP Udp;


time_t getNtpTime(void);

void sendNTPpacket(IPAddress &address);
void ClockInit(void);
#endif
