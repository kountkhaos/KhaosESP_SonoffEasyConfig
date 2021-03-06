#ifndef PAGE_Configuration_H
#define PAGE_Configuration_H

const char PAGE_WaitAndReload[] = R"=====( <meta http-equiv="refresh" content="5; URL=cfg"> Please Wait....Configuring and Restarting.)=====";


String get_wifi_networks_options()
{

    String Networks = "";

    int n = WiFi.scanNetworks();

    if ( n == 0 ){
        Networks = "<option value=\"" + DEFAULT_SSID + "\">No networks found!</option>";
    } else {

        for (int i = 0; i < n; ++i ) {
            int quality = 0;
            if(WiFi.RSSI(i) <= -100) {
                quality = 0;
            } else if(WiFi.RSSI(i) >= -50) {
                quality = 100;
            } else {
                quality = 2 * (WiFi.RSSI(i) + 100);
            }

            Networks += "<option value=\"" + String(WiFi.SSID(i)) + "\"";

            if ((String)WiFi.SSID() == String(WiFi.SSID(i)) )  Networks += "selected=\"selected\"";

            Networks += ">" +  String(WiFi.SSID(i)) +" (" +  String(quality) + " " +  String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*")  + ") </option>\n";
        }
    }

    return Networks;

}

//    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
//    <script src="https://ajax.googleapis.com/ajax/libs/jqueryui/1.12.1/jquery-ui.min.js"></script>
//    <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">

//String TABLE = "<table border='1' cellspacing='0' cellpadding='3' align='center'>";
//String TABLE_MID = "</table>" + TABLE;

String generate_config_page()
{

    Serial.println("getting web config page ...");

    HTMLPage = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
&nbsp;&nbsp;

<style>

td {
    font-size: 23px;
}

input {
    font-size: 18px;
}

select {
    font-size: 18px;
}

.err {
    color: white;
    background-color: red;
}

.fname {
    font-weight: bold;
}

.maintitle {
    background-color: lightblue;
    font-weight: bold;
}

</style>

<form action="/cfg" method="POST">
<table border='0' cellspacing='0' cellpadding='3' align='center'>

<tr><td class = 'maintitle' colspan='4'>Configuration</td></tr>
 
)=====";
    if ( hasErrCfg == true ){
        HTMLPage += "<tr><td class='err' colspan='4'>Config has errors. See below</td></tr>";
    }

    HTMLPage += "<tr><td class='fname' colspan='4'>Device Name:</td></tr><tr><td colspan='4'><input type='input' id='device_name' name='device_name' value='" + (String) config.DeviceName + "'></td></tr>";
    if (errConfig.DeviceName != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.DeviceName + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>Device Username:</td></tr><tr><td colspan='4'><input type='input' id='device_username' name='device_username' value='" + (String) config.DeviceUsername + "'></td></tr>";
    if (errConfig.DeviceUsername != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.DeviceUsername + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>Device Password:</td></tr><tr><td colspan='4'><input type='password' id='device_password' name='device_password' value=''></td></tr>";
    if (errConfig.DevicePassword != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.DevicePassword + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>SSID:</td></tr><tr><td colspan='4'><select id='ssid' name='ssid' value="">";
    HTMLPage += get_wifi_networks_options() + "</select>";
    if (errConfig.ssid != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.ssid + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>SSID Password:</td></tr><tr><td colspan='4'><input type='password' id='ssid_password' name='ssid_password' value=''></td></tr>";
    if (errConfig.ssid_password != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.ssid_password + "</td></tr>";

    HTMLPage += "<tr><td class='fname'>DHCP:</td><td><input type='checkbox' id='dhcp' name='dhcp' ";
    if ( config.dhcp == true ) HTMLPage += "checked";
    HTMLPage += "></td></tr>";

    int i = 0 ;
    HTMLPage += "<tr><td class='fname' colspan='4'>IP:</td></tr><tr>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='ip_" + (String) i +"' name='ip_" + (String) i +"' size='2' value='" + (String) config.IP[i]  + "'></td>";
    }
    HTMLPage += "</tr>";
    if (errConfig.IP != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.IP + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>NetMask:</td></tr><tr>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='nm_" + (String) i +"' name='nm_" + (String) i +"' size='2' value='" + (String) config.Netmask[i]  + "'></td>";
    }
    HTMLPage += "</tr>";
    if (errConfig.Netmask != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.Netmask + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>Gateway:</td></tr><tr>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='gw_" + (String) i +"' name='gw_" + (String) i +"' size='2' value='" + (String) config.Gateway[i]  + "'></td>";
    }
    HTMLPage += "</tr>";
    if (errConfig.Gateway != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.Gateway + "</td></tr>";


    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Broker IP:</td></tr><tr>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='mqttip_" + (String) i +"' name='mqttip_" + (String) i +"' size='2' value='" + (String) config.MQTTBrokerIP[i]  + "'></td>";
    }
    HTMLPage += "</tr>";
    if (errConfig.MQTTBrokerIP != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTBrokerIP + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Broker Port:</td></tr><tr><td colspan='4'><input type='input' id='mqtt_broker_port' name='mqtt_broker_port' value='"+ (String) config.MQTTBrokerPort + "'></td></tr>";
    if (errConfig.MQTTBrokerPort != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTBrokerPort + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Broker Topic:</td></tr><tr><td colspan='4'><input type='input' id='mqtt_broker_topic' name='mqtt_broker_topic' value='" + (String) config.MQTTBrokerTopic + "'></td></tr>";
    if (errConfig.MQTTBrokerTopic != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTBrokerTopic + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Broker Username:</td></tr><tr><td colspan='4'><input type='input' id='mqtt_broker_username' name='mqtt_broker_username' value='" + (String) config.MQTTBrokerUsername + "'></td></tr>";
    if (errConfig.MQTTBrokerUsername != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTBrokerUsername + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Broker Password:</td></tr><tr><td colspan='4'><input type='password' id='mqtt_broker_password' name='mqtt_broker_password' value=''></td></tr>";
    if (errConfig.MQTTBrokerPassword != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTBrokerPassword + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>MQTT Update Seconds:</td></tr><tr><td colspan='4'><input type='text' id='mqtt_update_secs' name='mqtt_update_secs' value='"+String( config.MQTTUpdateQtrSecs / 4 )+"'></td></tr>";
    if (errConfig.MQTTBrokerPassword != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.MQTTUpdateQtrSecs + "</td></tr>";

    HTMLPage += "<tr><td class='fname' colspan='4'>NTP Time Server:</td></tr><tr><td colspan='4'><input type='text' id='ntp_server_name' name='ntp_server_name' value='"+ (String) config.ntpServerName +"'></td></tr>";
    if (errConfig.ntpServerName != "") HTMLPage += "<tr><td colspan='4' class='err'>" + errConfig.ntpServerName + "</td></tr>";

    HTMLPage += "<tr><td colspan='4' class='fname'>DS18B20 Therm GPIO-14:</td><td><input type='checkbox' id='ds18b20_therm' name='ds18b20_therm' ";
    if ( config.ds18b20_therm == true ) HTMLPage += "checked";
    HTMLPage += "></td></tr>";


    HTMLPage += "<tr><td class='fname' colspan='4'><input type='submit' value='Submit'></td></tr></table></form>";

    return HTMLPage;
}

void handleGETConfigPage()
{


// TODO , get config copied to tempConfig for the webpage.

//memcpy (dest_struct, source_struct, sizeof (dest_struct));
//dest_struct->strptr = strdup (source_struct->strptr);

//    memcpy (tempConfig, config, sizeof (tempConfig));
//    tempConfig->strptr = strdup (config->strptr);

    server.send ( 200, "text/html", generate_config_page() );
}

void cfg_args_parse()
{
    Serial.println(" in cfg_args_parse ");
    Serial.println(__FUNCTION__);

    if (server.args() > 0 ) // Save Settings
    {
        hasErrCfg = false;
        clearErrorConfig();
        s_arg     = "";
        s_argName = "";

        // TODO a option will read in the DefaultConfig.
        // TODO a web option to ESP.restart();

        // if you are saving the config, its assumed you've selected a SSID to connect to
        // so the AccessPoint Mode will be disabled :
        config.AccessPointEnabled = false;
        config.defaultConfig      = false;
        config.dhcp               = false;
        config.ds18b20_therm      = false;

// TODO really need to get config.dhcp setting first.
// If dhcp is true then the IP, Netmask and Gateway don't really need to report errors.

// TODO only set the main config if the parse of args happens without errors.
// need a temp config struct to copy into .

// The alternative is if there is an error , just reload the config from the EEPROM

// another alternative is just update the EEPROM with the values that are okay .

        for ( uint8_t i = 0; i < server.args(); i++ ){

            s_arg     = urldecode(server.arg(i));
            s_argName = server.argName(i);

            if (s_argName == "device_name"){
                if ( checkStringStd(s_arg) ) config.DeviceName = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.DeviceName = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "device_username"){
                if ( checkStringStd(s_arg) ) config.DeviceUsername = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.DeviceUsername = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "device_password" && s_arg.length() > 0 ){
                if ( checkStringStd(s_arg) && s_arg.length() > 5 ) config.DevicePassword = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.DevicePassword = EP_STD_TOO_LONG + " or too short ( < 6 )";
                }
            }

            if (s_argName == "ssid"){
                if ( checkSSID(s_arg) ) config.ssid = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.ssid = "Too long ( > " + String(EP_32_LEN) + " ) or invalid SSID chars";
                }
            }

            if (s_argName == "ssid_password" && s_arg.length() > 0){
                if ( checkStringStd(s_arg) ) config.ssid_password = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.ssid_password = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "mqtt_update_secs"){
                if ( check_mqtt_update_secs(s_arg) ) config.MQTTUpdateQtrSecs = s_arg.toInt() * 4;
                else {
                    hasErrCfg = true;
                    errConfig.MQTTUpdateQtrSecs = "0 to " + String(MQTT_UPDATE_MAX_SECS);
                }
            }

            if (s_argName == "mqtt_broker_topic"){
                if ( checkStringStd(s_arg) ) config.MQTTBrokerTopic = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.MQTTBrokerTopic = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "mqtt_broker_username"){
                if ( checkStringStd(s_arg) ) config.MQTTBrokerUsername = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.MQTTBrokerUsername = EP_STD_TOO_LONG;
                }
            }

// TODO need some way of blanking an mqtt password. The below could be an issue.
            if (s_argName == "mqtt_broker_password" && s_arg.length() > 0 ){
                if ( checkStringStd(s_arg) ) config.MQTTBrokerPassword = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.MQTTBrokerPassword = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "mqtt_broker_port"){
                if ( check_uint16_t(s_arg) ) config.MQTTBrokerPort = s_arg.toInt();
                else {
                    hasErrCfg = true;
                    errConfig.MQTTBrokerPort = "Invalid, 0 to 65535 only";
                }
            }

            if (s_argName == "ntp_server_name"){
                if ( checkStringStd(s_arg) ) config.ntpServerName = s_arg;
                else {
                    hasErrCfg = true;
                    errConfig.ntpServerName = EP_STD_TOO_LONG;
                }
            }

            if (s_argName == "dhcp") config.dhcp = true;
            if (s_argName == "ds18b20_therm") config.ds18b20_therm = true;

            for ( uint8_t z = 0 ; z < 4; z++ ){
                if (s_argName == "ip_" + String(z) ){
                    if( checkIPOctet(s_arg) ) config.IP[z] = s_arg.toInt();
                    else {
                        hasErrCfg = true;
                        errConfig.IP = "0 to 255 only";
                    }
                }

                if (s_argName == "nm_" + String(z) ){
                    if( checkIPOctet(s_arg) ) config.Netmask[z] = s_arg.toInt();
                    else {
                        hasErrCfg = true;
                        errConfig.Netmask = "0 to 255 only";
                    }
                }

                if (s_argName == "gw_" + String(z) ){
                    if( checkIPOctet(s_arg) ) config.Gateway[z] = s_arg.toInt();
                    else {
                        hasErrCfg = true;
                        errConfig.Gateway = "0 to 255 only";
                    }
                }

                if (s_argName == "mqttip_" + String(z) ){
                    if( checkIPOctet(s_arg) ) config.MQTTBrokerIP[z] = s_arg.toInt();
                    else {
                        hasErrCfg = true;
                        errConfig.MQTTBrokerIP = "0 to 255 only";
                    }
                }
            }
        }

        if ( config.DevicePassword == DEFAULT_DEVICE_PASSWORD ){
            hasErrCfg = true;
            errConfig.DevicePassword = "Device Password is still '"+DEFAULT_DEVICE_PASSWORD+"'";
        }

    } else {
        hasErrCfg = true;
    }

    print_Config();

    if ( hasErrCfg == true ){
        Serial.println(" config args invalid ");

// TODO this is here until i get tempConfig copying for the config page :
        ReadConfig();

    } else {

// TODO copy tempConfig into config

    }
}

void handlePOSTConfigPage()
{
    cfg_args_parse();

    if ( hasErrCfg == false ){
        server.send ( 200, "text/html", PAGE_WaitAndReload );
        WriteConfig();

    // TODO only restart the wifi if the config for ssid/ssid_password , access point enabled has actually changed.
        ConfigureWifiStartWeb();
    } else {
        server.send ( 400, "text/html", generate_config_page() );
    }
}

#endif
