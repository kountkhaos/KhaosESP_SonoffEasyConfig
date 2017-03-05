/*

KhaosESP_SonoffEasyConfig

Karl "Kount-Khaos" Hoskin : 17th February 2017.

Based heavily on ideas and (a little) code from :
    John Lassen's ESP_WebConfig ( thanks John !)

This is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

*/

//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#include <WiFiClient.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
// #include <ArduinoJson.h>
// #include "ESP8266TrueRandom.h"

#include "global.h"
#include "time_sync.h"
#include "PAGE_Configuration.h"

void setup ( void ) {

    EEPROM.begin(512); // there is a 4K limit on this
    Serial.begin(115200);
    delay(500);

    Serial.println("");
    Serial.println("");
    Serial.println("***************");
    Serial.println("Starting ES8266");
    Serial.println("***************");

    pinMode( gpio13GreenLED, OUTPUT );
    pinMode( gpio12Relay, OUTPUT );
    pinMode( gpio0Button, INPUT_PULLUP );

    tkQtrSecond.attach_ms(250, Qtr_Second_Tick);

    if (!ReadConfig()) DefaultConfig();

    ConfigureWifiStartWeb();


}

void loop ( void ) {

    pollButtonSetLED();
    pollMQTTUpdate();

    server.handleClient();
    if (client.connected()) client.loop();

    // yield();
}


