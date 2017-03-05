#ifndef TIME_SYNC_H
#define TIME_SYNC_H

/* most of this code is from :
 * https://github.com/PaulStoffregen/Time
 *
 * modified slightly for KhaosESP_SonoffEasyConfig
 *
 * TimeNTP_ESP8266WiFi.ino
 * Example showing time sync to NTP time source
 *
 * This sketch uses the ESP8266WiFi library
 */

WiFiUDP Udp;
unsigned int ntp_localPort = 8888; // local port to listen for UDP packets

// TODO FIXME , this could be configurable in the web conf :
#define SYNC_TIME_EVERY_SECS 30

bool time_has_synced = false;
int time_failed_sync_count = 0;

time_t getNtpTime();

String now_rfc3339_utc()
{
    if ( time_has_synced == false ) return "time not synced";

    char str[30];

    sprintf(str, "%04d-%02d-%02dT%02d:%02d:%02dZ",
        year(), month(), day(), hour(), minute(), second()
    );

    return String(str);
}

time_t now_if_synced()
{
    if ( time_has_synced == false ) return 0;
    return now();
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
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
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime()
{
  // no point trying if we don't have an ntpServerName.
  if ( config.ntpServerName == "") return 0;

  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(config.ntpServerName.c_str(), ntpServerIP);
  Serial.print(config.ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];

      time_has_synced = true;
      time_failed_sync_count = 0;

      return secsSince1900 - 2208988800UL ;
    }
  }

  Serial.println("No NTP Response :-(");
  time_failed_sync_count ++;

  return 0; // return 0 if unable to get the time
}

void setupNTPSync()
{

  Serial.println("Starting UDP for time-sync");
  Udp.begin(ntp_localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(SYNC_TIME_EVERY_SECS);
}

#endif
