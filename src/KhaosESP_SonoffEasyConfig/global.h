#ifndef GLOBAL_H
#define GLOBAL_H

// TODO const uint8_t for the below pin settings ?

ESP8266WebServer server(80); // The Webserver

int LED_FLASH_QTR_SEC_COUNT = 0;

int gpio13GreenLED          = 13; // Sonoff green led is on 13. indicator led gpio pin.
// Sonoff Red LED is only for the 433MHz add on board.

int gpio12Relay              = 12; // Sonoff Relay is on 12.

int gpio14                   = 14; // Sonoff spare pin at the end of where the FTDI serial pins are.

int gpio3RXD                 = 3; //  Sonoff This is also the UART RXD pin
int gpio1TXD                 = 1; //  Sonoff This is also the UART TXD pin

// TODO rename this gpio0Button
int gpio0Button        = 0;  // Sonoff button is on 0. reset to AP mode button gpio pin.

int Button_PressCount = 0;  // 1/4 second count of how long its been pressed.
boolean Button_HasToggledRelay       = false;
boolean Button_HasToggledWPS         = false; //Not yet implemented.
boolean Button_HasAccessPointEnabled = false;
boolean Button_HasDefaultConfiged    = false;

boolean Refresh              = false; // For Main Loop, to refresh things like GPIO / WS2812
Ticker  tkQtrSecond;                  // Quarter Second ticker. For LED flashing

#define WIFI_CONNECT_DELAY   500
#define WIFI_CONNECT_TIMEOUT_COUNT 12
int WifiConnectWaitCount = 0;

String HTMLPage = ""; // used by page generating subs.

// used by configuration page
boolean hasErrCfg = false;
String  s_arg     = "";
String  s_argName = "";

struct strConfig {

        boolean AccessPointEnabled;
        boolean dhcp;
        boolean ds18b20_therm;
        boolean defaultConfig;

        byte  IP[4];
        byte  Netmask[4];
        byte  Gateway[4];
        byte  MQTTBrokerIP[4];

        uint32_t MQTTUpdateQtrSecs;

        uint16_t MQTTBrokerPort;

        String ssid;
        String ssid_password;

        String MQTTBrokerTopic;
        String MQTTBrokerUsername;
        String MQTTBrokerPassword;
        String DeviceName;
        String DeviceUsername;
        String DevicePassword;

} config, tempConfig;

struct strErrorConfig {

        String IP;
        String Netmask;
        String Gateway;
        String MQTTBrokerIP;

        String MQTTUpdateQtrSecs;

        String MQTTBrokerPort;

        String ssid;
        String ssid_password;

        String MQTTBrokerTopic;
        String MQTTBrokerUsername;
        String MQTTBrokerPassword;
        String DeviceName;
        String DeviceUsername;
        String DevicePassword;

} errConfig;

void clearErrorConfig(){

    errConfig.IP                 = "";
    errConfig.Netmask            = "";
    errConfig.Gateway            = "";
    errConfig.MQTTBrokerIP       = "";

    errConfig.MQTTBrokerPort     = "";

    errConfig.MQTTUpdateQtrSecs  = "";

    errConfig.ssid               = "";
    errConfig.ssid_password      = "";

    errConfig.MQTTBrokerTopic    = "";
    errConfig.MQTTBrokerUsername = "";
    errConfig.MQTTBrokerPassword = "";
    errConfig.DeviceName         = "";
    errConfig.DeviceUsername     = "";
    errConfig.DevicePassword     = "";

}

WiFiClient espClient;
PubSubClient client(espClient);


// TODO tempConfig not yet in use.

String DEFAULT_DEVICE_PASSWORD = "password";

// ESP8266 Access Point :
String  ACCESS_POINT_NAME    = "ESP8266";
#define ACCESS_POINT_PASSWORD  "password"

// Default Access Point to connect to :
String  DEFAULT_SSID         = "MYSSID";
#define DEFAULT_SSID_PASSWORD  "MYPASSWORD"

// EEPROM Address constants
#define EPA_M_PORT           48
#define EPA_WF_SSID          52  // 32 bytes
#define EPA_WF_PASS          86
#define EPA_M_TOPIC          136
#define EPA_M_USER           186
#define EPA_M_PASS           236
#define EPA_DEVICE_NAME      286
#define EPA_DEVICE_USERNAME  336
#define EPA_DEVICE_PASSWORD  386 // 48 bytes
#define EPA_MQTT_UPDATE_SECS 436 // 4  bytes


int EP_32_LEN      = 32;
int EP_STD_STR_LEN = 48;

String EP_STD_TOO_LONG = "Too long ( > " + String(EP_STD_STR_LEN) + " )";

//
#define BUTTON_WPS_PRESS_COUNT            16  // 1/4 of seconds. So 16 / 4 = 4 secs.
#define BUTTON_AP_PRESS_COUNT             32  // 1/4 of seconds. So 32 / 4 = 8 secs.
#define BUTTON_DEFAULT_CONFIG_PRESS_COUNT 48  // 1/4 of seconds. So 48 / 4 = 12 secs.

#define WIFI_AP_LED_BLINK                 12  // 1/4 sec x 12 = 3 secs between led blinks

uint32_t MQTT_Update_Count = 0;  // 1/4 second count of how long since last update.
#define MQTT_UPDATE_MAX_SECS 1000000000

void stopWebserver()
{
    server.close();
}

void handleRoot() {
    Serial.println("handling web : root ");
    server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handlePageNotFound() {
    Serial.println("handling web : Page Not Found");
    server.send(404, "text/html", "<h1>Page Not Found</h1>");
}

// predeclare from PAGE_NetworkConfiguration.h :
// void   send_network_configuration_html();
// String generate_config_page();
void handleGETConfigPage();
void handlePOSTConfigPage();

void startWebserver(){

    server.on("/",     handleRoot );

//    server.on("/cfg", handleGETConfigPage );

    server.on("/cfg", HTTP_GET, [](){
        handleGETConfigPage();
    });

    server.on("/cfg", HTTP_POST, [](){
        handlePOSTConfigPage();
    });

    server.onNotFound ( []() {
        handlePageNotFound();
    });

    server.begin();

    Serial.println( "HTTP server started" );
}

boolean check_uint16_t(String Value) {

    // TODO also return false if value can't be cast to an int.

    if (Value.toInt() < 0 || Value.toInt() > 65535) {
        return false;
    } else {
        return true;
    }
}

boolean check_mqtt_update_secs(String Value) {

    // uint32_t of 1/4 secs.
    // TODO also return false if value can't be cast to an int.
    if (Value.toInt() < 0 || Value.toInt() > MQTT_UPDATE_MAX_SECS ) {
        return false;
    } else {
        return true;
    }
}

boolean checkIPOctet(String Value) {
    // TODO also return false if value can't be cast to an int.

    if (Value.toInt() < 0 || Value.toInt() > 255) {
        return false;
    } else {
        return true;
    }
}

boolean checkStringStd(String text ) {
    if (text.length() > EP_STD_STR_LEN) {
        return false;
    } else {
        return true;
    }
}

boolean checkSSID(String text) {
    if (text.length() > EP_32_LEN) {
        return false;
    } else {
        return true;
    }
    // TODO also needs to check more than just the length of SSID.

}

void WriteStringToEEPROM(int beginaddress, String string)
{
    char  charBuf[string.length()+1];
    string.toCharArray(charBuf, string.length()+1);
    for (int t=0 ; t < sizeof(charBuf) ; t++)
    {
        EEPROM.write(beginaddress + t,charBuf[t]);
    }
}


String  ReadStringFromEEPROM(int beginaddress, int s_len) {
    byte counter=0;
    char rChar;
    String retString = "";
    while (1) {
        rChar = EEPROM.read(beginaddress + counter);
        if (rChar == 0) break;
        if (counter +1 > s_len ) break;
        counter++;
        retString.concat(rChar);
    }
    // TODO is this going to return a null terminated string ?
    return retString;
}

void EEPROMWrite_uint16_t(int address, uint16_t value) {
    byte two = (value & 0xFF);
    byte one = ((value >> 8) & 0xFF);

    //Write the 2 bytes into the eeprom memory.
    EEPROM.write(address , two);
    EEPROM.write(address + 1, one);
}

uint16_t EEPROMRead_uint16_t(int address) {
    //Read the 2 bytes from the eeprom memory.
    uint16_t two = EEPROM.read(address);
    uint16_t one = EEPROM.read(address + 1);

    //Return the recomposed long by using bitshift.
    return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF) ;
}

void EEPROMWrite_uint32_t(int address, uint32_t value) {
    byte four  = (value & 0xFF);
    byte three = ((value >> 8) & 0xFF);
    byte two   = ((value >> 16) & 0xFF);
    byte one   = ((value >> 24) & 0xFF);

    //Write the 4 bytes into the eeprom memory.
    EEPROM.write(address, four);
    EEPROM.write(address + 1, three);
    EEPROM.write(address + 2, two);
    EEPROM.write(address + 3, one);
}

uint32_t EEPROMRead_uint32_t(int address) {
    //Read the 4 bytes from the eeprom memory.
    long four  = EEPROM.read(address);
    long three = EEPROM.read(address + 1);
    long two   = EEPROM.read(address + 2);
    long one   = EEPROM.read(address + 3);

    //Return the recomposed long by using bitshift.
    return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

String GetMacAddress() {
    uint8_t mac[6];
    char macStr[18] = {0};
    WiFi.macAddress(mac);
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
     char c;
     String ret = "";

     for(byte t=0;t<input.length();t++) {
         c = input[t];
         if (c == '+') c = ' ';
         if (c == '%') {
            t++;
            c = input[t];
            t++;
            c = (h2int(c) << 4) | h2int(input[t]);
         }

         ret.concat(c);
     }
     return ret;
}

void print_Config_IP(String indent ){
    Serial.println( indent + "config.IP                 :"
        +     (String) config.IP[0]
        +"."+ (String) config.IP[1]
        +"."+ (String) config.IP[2]
        +"."+ (String) config.IP[3]);

    Serial.println( indent + "config.Netmask            :"
        +     (String) config.Netmask[0]
        +"."+ (String) config.Netmask[1]
        +"."+ (String) config.Netmask[2]
        +"."+ (String) config.Netmask[3]);

    Serial.println( indent + "config.Gateway            :"
        +     (String) config.Gateway[0]
        +"."+ (String) config.Gateway[1]
        +"."+ (String) config.Gateway[2]
        +"."+ (String) config.Gateway[3]);
}

void print_Config() {

    Serial.println("Configurarion is :");
    Serial.println("    config.AccessPointEnabled :" + (String) config.AccessPointEnabled);

    Serial.println("    config.dhcp               :" + (String) config.dhcp);
    Serial.println("    config.ds18b20_therm(io14):" + (String) config.ds18b20_therm);
    print_Config_IP("    ");

    Serial.println("    config.MQTTBrokerIP       :"
        +     (String) config.MQTTBrokerIP[0]
        +"."+ (String) config.MQTTBrokerIP[1]
        +"."+ (String) config.MQTTBrokerIP[2]
        +"."+ (String) config.MQTTBrokerIP[3]);

    Serial.println("    config.MQTTBrokerPort     :" + (String) config.MQTTBrokerPort);

// TODO passwords should probably not be shown in the serial terminal ...

    Serial.println("    config.ssid               :" + (String) config.ssid);
    Serial.println("    config.ssid_password      :" + (String) config.ssid_password);

    Serial.println("    config.MQTTBrokerTopic    :" + (String) config.MQTTBrokerTopic);
    Serial.println("    config.MQTTBrokerUsername :" + (String) config.MQTTBrokerUsername);
    Serial.println("    config.MQTTBrokerPassword :" + (String) config.MQTTBrokerPassword);

    Serial.println("    config.MQTTUpdateQtrSecs  :" + (String) config.MQTTUpdateQtrSecs);

    Serial.println("    config.DeviceName         :" + (String) config.DeviceName);
    Serial.println("    config.DeviceUsername     :" + (String) config.DeviceUsername);
    Serial.println("    config.DevicePassword     :" + (String) config.DevicePassword);
}

void WiFiConnectedWait ()
{

    WifiConnectWaitCount = 0;

    Serial.println("Waiting for Wifi connection ...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CONNECT_DELAY);
        Serial.print(".");
        WifiConnectWaitCount++;

        if ( WifiConnectWaitCount > WIFI_CONNECT_TIMEOUT_COUNT ) break;
    }

    if ( WiFi.status() != WL_CONNECTED ) {
        Serial.println("    Can't connect to Wifi");
    } else {
        Serial.println("    CONNECTED !");
    }

}

void ConfigureWifiStartWeb() {

    Serial.println("Configuring Wifi");

    if (!strncmp(ESP.getSdkVersion(),"1.5.3",5)) {
        Serial.println("  Wifi: Patch issue 2186");
        WiFi.mode(WIFI_OFF); // See https://github.com/esp8266/Arduino/issues/2186
    }

    stopWebserver();
    WiFi.disconnect();

    if (config.AccessPointEnabled) {
        Serial.println("  in Access Point mode");
        WiFi.mode(WIFI_AP_STA);
        //WiFi.softAPmacAddress(mac); ## TODO use on the end of the SSID name
        WiFi.softAP( ACCESS_POINT_NAME.c_str() , ACCESS_POINT_PASSWORD);
        delay(500);

        // TODO is there anyway to tell if the softAP is fully started and ready for the webserver to be attached ?

        IPAddress ip = WiFi.softAPIP();
        Serial.println(
            "  Access Point IP = "
             + String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3])
        );

        startWebserver();

    } else {
        Serial.println("  in Client mode : connect to SSID : " + config.ssid );
        WiFi.mode(WIFI_STA);

        if ( ! config.dhcp ){
            Serial.println("    : using fixed IP :");
            print_Config_IP("        ");
            WiFi.config(
                IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),
                IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ),
                IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] )
            );
        }
        WiFi.begin (config.ssid.c_str(), config.ssid_password.c_str());

        WiFiConnectedWait();

        if (WiFi.status() != WL_CONNECTED){
            Serial.println("  wifi not yet connected");
        } else {
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            startWebserver();
        }
    }
}

void WriteConfig()
{
    Serial.println("Writing Config");
    EEPROM.write(0,'C');
    EEPROM.write(1,'F');
    EEPROM.write(2,'G');

    // TODO is this correct/best way to write booleans ?

    if ( config.ds18b20_therm == true ) EEPROM.write(28,1);
    else EEPROM.write(28, 0);

    if ( config.defaultConfig == true ) EEPROM.write(29,1);
    else EEPROM.write(29, 0);

    if ( config.AccessPointEnabled == true ) EEPROM.write(30,1);
    else EEPROM.write(30, 0);

    if ( config.dhcp == true ) EEPROM.write(31, 1);
    else EEPROM.write(31, 0);

    EEPROM.write(32, config.IP[0]);
    EEPROM.write(33, config.IP[1]);
    EEPROM.write(34, config.IP[2]);
    EEPROM.write(35, config.IP[3]);

    EEPROM.write(36, config.Netmask[0]);
    EEPROM.write(37, config.Netmask[1]);
    EEPROM.write(38, config.Netmask[2]);
    EEPROM.write(39, config.Netmask[3]);

    EEPROM.write(40, config.Gateway[0]);
    EEPROM.write(41, config.Gateway[1]);
    EEPROM.write(42, config.Gateway[2]);
    EEPROM.write(43, config.Gateway[3]);

    EEPROM.write(44, config.MQTTBrokerIP[0]);
    EEPROM.write(45, config.MQTTBrokerIP[1]);
    EEPROM.write(46, config.MQTTBrokerIP[2]);
    EEPROM.write(47, config.MQTTBrokerIP[3]);

    EEPROMWrite_uint16_t(EPA_M_PORT, config.MQTTBrokerPort);

    WriteStringToEEPROM(EPA_WF_SSID, config.ssid);
    WriteStringToEEPROM(EPA_WF_PASS, config.ssid_password);

    WriteStringToEEPROM(EPA_M_TOPIC, config.MQTTBrokerTopic);
    WriteStringToEEPROM(EPA_M_USER,  config.MQTTBrokerUsername);
    WriteStringToEEPROM(EPA_M_PASS,  config.MQTTBrokerPassword);

    WriteStringToEEPROM(EPA_DEVICE_NAME,     config.DeviceName);
    WriteStringToEEPROM(EPA_DEVICE_USERNAME, config.DeviceUsername);
    WriteStringToEEPROM(EPA_DEVICE_PASSWORD, config.DevicePassword);

    EEPROMWrite_uint32_t(EPA_MQTT_UPDATE_SECS, config.MQTTUpdateQtrSecs);

    EEPROM.commit();

    delay(100);
}

void DefaultConfig()
{
    // DEFAULT CONFIG
    config.ssid          = DEFAULT_SSID;
    config.ssid_password = DEFAULT_SSID_PASSWORD;

    config.defaultConfig      = true;
    config.AccessPointEnabled = true;
    config.ds18b20_therm      = false;
    config.dhcp  = true;
    config.IP[0] = 192;
    config.IP[1] = 168;
    config.IP[2] = 1;
    config.IP[3] = 100;
    config.Netmask[0] = 255;
    config.Netmask[1] = 255;
    config.Netmask[2] = 255;
    config.Netmask[3] = 0;
    config.Gateway[0] = 192;
    config.Gateway[1] = 168;
    config.Gateway[2] = 1;
    config.Gateway[3] = 1;

    config.MQTTBrokerIP[0] = 192;
    config.MQTTBrokerIP[1] = 168;
    config.MQTTBrokerIP[2] = 1;
    config.MQTTBrokerIP[3] = 101;

    config.MQTTBrokerPort  = 1883;

    config.MQTTBrokerTopic    = "sonoff";
    config.MQTTBrokerUsername = "";
    config.MQTTBrokerPassword = "";

    config.MQTTUpdateQtrSecs = 0;

    config.DeviceName     = "MySonoffDevice";
    config.DeviceUsername = "admin";
    config.DevicePassword = DEFAULT_DEVICE_PASSWORD;

    WriteConfig();
    Serial.println("Default Config applied");

}

boolean ReadConfig()
{
    Serial.println("Reading Configuration");
    if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' ){
        Serial.println("Configurarion Found");

        config.ds18b20_therm      = !! EEPROM.read(28);
        config.defaultConfig      = !! EEPROM.read(29);
        config.AccessPointEnabled = !! EEPROM.read(30);
        config.dhcp               = !! EEPROM.read(31);

        config.IP[0] = EEPROM.read(32);
        config.IP[1] = EEPROM.read(33);
        config.IP[2] = EEPROM.read(34);
        config.IP[3] = EEPROM.read(35);

        config.Netmask[0] = EEPROM.read(36);
        config.Netmask[1] = EEPROM.read(37);
        config.Netmask[2] = EEPROM.read(38);
        config.Netmask[3] = EEPROM.read(39);

        config.Gateway[0] = EEPROM.read(40);
        config.Gateway[1] = EEPROM.read(41);
        config.Gateway[2] = EEPROM.read(42);
        config.Gateway[3] = EEPROM.read(43);

        config.MQTTBrokerIP[0] = EEPROM.read(44);
        config.MQTTBrokerIP[1] = EEPROM.read(45);
        config.MQTTBrokerIP[2] = EEPROM.read(46);
        config.MQTTBrokerIP[3] = EEPROM.read(47);

        config.MQTTBrokerPort = EEPROMRead_uint16_t(EPA_M_PORT);

        config.ssid          = ReadStringFromEEPROM(EPA_WF_SSID, EP_32_LEN);
        config.ssid_password = ReadStringFromEEPROM(EPA_WF_PASS, EP_STD_STR_LEN);

        config.MQTTBrokerTopic    = ReadStringFromEEPROM(EPA_M_TOPIC, EP_STD_STR_LEN);
        config.MQTTBrokerUsername = ReadStringFromEEPROM(EPA_M_USER, EP_STD_STR_LEN);
        config.MQTTBrokerPassword = ReadStringFromEEPROM(EPA_M_PASS, EP_STD_STR_LEN);

        config.DeviceName     = ReadStringFromEEPROM(EPA_DEVICE_NAME,     EP_STD_STR_LEN);
        config.DeviceUsername = ReadStringFromEEPROM(EPA_DEVICE_USERNAME, EP_STD_STR_LEN);
        config.DevicePassword = ReadStringFromEEPROM(EPA_DEVICE_PASSWORD, EP_STD_STR_LEN);

        config.MQTTUpdateQtrSecs = EEPROMRead_uint32_t(EPA_MQTT_UPDATE_SECS);

        print_Config();
        return true;
    } else {
        Serial.println("Configurarion NOT FOUND!!!!");
        return false;
    }
}

void Qtr_Second_Tick() {
    LED_FLASH_QTR_SEC_COUNT++;
    Button_PressCount++;
    MQTT_Update_Count++;
}

void toggle_led(){
    if ( digitalRead(gpio13GreenLED) == LOW )
         digitalWrite(gpio13GreenLED, HIGH);
    else digitalWrite(gpio13GreenLED, LOW);
}

void toggle_led_every_qtr_sec (){
    if ( LED_FLASH_QTR_SEC_COUNT > 0 ) {
        LED_FLASH_QTR_SEC_COUNT = 0 ;
        toggle_led();
    }
}

void toggle_relay(){
    // TODO send an MQTT message if that's enabled.
    if ( digitalRead(gpio12Relay) == LOW ){
        Serial.println("Relay Toggled ON by button");
        digitalWrite(gpio12Relay, HIGH);
    } else {
        Serial.println("Relay Toggled OFF by button");
        digitalWrite(gpio12Relay, LOW);
    }
}

void SwitchTo_WIFI_AP_STA_MODE(){
    Serial.println("Switch to WIFI_AP_STA MODE (AccessPointEnabled)");
    config.ssid          = DEFAULT_SSID;
    config.ssid_password = DEFAULT_SSID_PASSWORD;
    config.AccessPointEnabled = true;
    WriteConfig();
    ConfigureWifiStartWeb();
}

void pollButtonSetLED(){

    if ( digitalRead( gpio0Button ) == LOW ) {
        // button is being pressed.

        // LED output indication :
        if ( Button_PressCount > BUTTON_DEFAULT_CONFIG_PRESS_COUNT){
            // > 12 secs
            digitalWrite(gpio13GreenLED, HIGH);

        } else if ( Button_PressCount > BUTTON_AP_PRESS_COUNT ){
            // > 8 secs
            toggle_led_every_qtr_sec();

        } else if ( Button_PressCount > BUTTON_WPS_PRESS_COUNT ){
            // > 4 secs
            digitalWrite(gpio13GreenLED, LOW);

        } else {
            // < 4 secs
            digitalWrite(gpio13GreenLED, HIGH);
        }

        // Do the action for the press duration :
        if ( Button_HasToggledRelay == false){
            toggle_relay();
            Button_HasToggledRelay = true;
        }

        if ( Button_PressCount > BUTTON_WPS_PRESS_COUNT
            && Button_HasToggledWPS == false
        ){
            Button_HasToggledWPS = true;
            Serial.println("WPS Mode NOT YET IMPLEMENTED. TODO");
        }

        if ( Button_PressCount > BUTTON_AP_PRESS_COUNT
            && Button_HasAccessPointEnabled  == false
        ){
            Button_HasAccessPointEnabled = true;
            SwitchTo_WIFI_AP_STA_MODE();
        }

        if ( Button_PressCount > BUTTON_DEFAULT_CONFIG_PRESS_COUNT
            && Button_HasDefaultConfiged == false
        ){
            Button_HasDefaultConfiged = true;
            DefaultConfig();
        }

    } else {
        Button_PressCount = 0;
        Button_HasToggledWPS         = false;
        Button_HasAccessPointEnabled = false;
        Button_HasDefaultConfiged    = false;
        Button_HasToggledRelay       = false;

        if ( config.AccessPointEnabled ){
            // blink led about every 3 seconds when in AP mode.

            if ( LED_FLASH_QTR_SEC_COUNT > WIFI_AP_LED_BLINK ){
                LED_FLASH_QTR_SEC_COUNT = 0;
                toggle_led();
            }

        } else {

            if ( WiFi.status() != WL_CONNECTED ) {
                // blink led every 1/4 second when trying to connect to a router.
                toggle_led_every_qtr_sec();

            } else {
                // so happily connected in WIFI_STA mode the LED is off.
                // TODO could here use LED to display the state of the relay ...
                digitalWrite(gpio13GreenLED, LOW);
            }
        }
    }
}

void pollMQTTUpdate() {

    if ( config.MQTTUpdateQtrSecs == 0 || config.MQTTUpdateQtrSecs > MQTT_Update_Count )
        return;

    MQTT_Update_Count = 0;

    Serial.println("MQTT periodic update . TODO needs implementing");

    // TODO : this will see if the ds18b20_therm is enabled. If it is it will be sent in the update.
    // will also send the state of the relay.


}


#endif
