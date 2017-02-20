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

String generate_config_page()
{

    Serial.println("getting web config page ...");

    HTMLPage = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
&nbsp;&nbsp;
<strong>Configuration</strong>
<hr>
Connect to Router with these settings:<br>
<form action="/cfg" method="POST">
    <table border="0" cellspacing="0" cellpadding="3" >
)=====";

    HTMLPage += "</td></tr><tr><td align='right'>Device Name:</td><td colspan='4'><input type='input' id='device_name' name='device_name' value='" + (String) config.DeviceName + "'></td></tr>";

    HTMLPage += "</td></tr><tr><td align='right'>Device Username:</td><td colspan='4'><input type='input' id='device_username' name='device_username' value='" + (String) config.DeviceUsername + "'></td></tr>";

    HTMLPage += "</td></tr><tr><td align='right'>Device Password:</td><td colspan='4'><input type='password' id='device_password' name='device_password' value=''></td></tr>";

    HTMLPage += "<tr><td align='right'>SSID:</td><td colspan='4'><select id='ssid' name='ssid' value="">";
    HTMLPage += get_wifi_networks_options() + "</select>";

    HTMLPage += "</td></tr><tr><td align='right'>SSID Password:</td><td colspan='4'><input type='password' id='ssid_password' name='ssid_password' value=''></td></tr>";

    HTMLPage += "<tr><td align='right'>DHCP:</td><td><input type='checkbox' id='dhcp' name='dhcp' ";
    if ( config.dhcp == true ) HTMLPage += "checked";

    HTMLPage += "></td></tr>";

    int i = 0 ;
    HTMLPage += "<tr><td align='right'>IP:</td>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='ip_" + (String) i +"' name='ip_" + (String) i +"' size='3' value='" + (String) config.IP[i]  + "'></td>";
    }
    HTMLPage += "</tr>";

    HTMLPage += "<tr><td align='right'>NetMask:</td>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='nm_" + (String) i +"' name='nm_" + (String) i +"' size='3' value='" + (String) config.Netmask[i]  + "'></td>";
    }
    HTMLPage += "</tr>";

    HTMLPage += "<tr><td align='right'>Gateway:</td>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='gw_" + (String) i +"' name='gw_" + (String) i +"' size='3' value='" + (String) config.Gateway[i]  + "'></td>";
    }
    HTMLPage += "</tr>";


    HTMLPage += "<tr><td align='right'>MQTT Broker IP:</td>";
    for ( i = 0 ; i < 4; i++ ){
        HTMLPage += "<td><input type='text' id='mqttip_" + (String) i +"' name='mqttip_" + (String) i +"' size='3' value='" + (String) config.MQTTBrokerIP[i]  + "'></td>";
    }
    HTMLPage += "</tr>";

    HTMLPage += "</td></tr><tr><td align='right'>MQTT Broker Port:</td><td colspan='4'><input type='input' id='mqtt_broker_port' name='mqtt_broker_port' value='"+ (String) config.MQTTBrokerPort + "'></td></tr>";

    HTMLPage += "</td></tr><tr><td align='right'>MQTT Broker Topic:</td><td colspan='4'><input type='input' id='mqtt_broker_topic' name='mqtt_broker_topic' value='" + (String) config.MQTTBrokerTopic + "'></td></tr>";

    HTMLPage += "</td></tr><tr><td align='right'>MQTT Broker Username:</td><td colspan='4'><input type='input' id='mqtt_broker_username' name='mqtt_broker_username' value='" + (String) config.MQTTBrokerUsername + "'></td></tr>";


    HTMLPage += "</td></tr><tr><td align='right'>MQTT Broker Password:</td><td colspan='4'><input type='password' id='mqtt_broker_password' name='mqtt_broker_password' value=''></td></tr>";

    if ( cfg_args_err != "" ){
        HTMLPage += "<tr><td colspan='5' align='center'>" + cfg_args_err + "</td></tr>";
    }

    HTMLPage += "<tr><td colspan='5' align='center'><input type='submit' value='Submit'></td></tr></table></form>";

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
        cfg_args_err = "";
        s_arg     = "";
        s_argName = "";

        // TODO a option will read in the DefaultConfig.
        // TODO a web option to ESP.restart();

        // if you are saving the config, its assumed you've selected a SSID to connect to
        // so the AccessPoint Mode will be disabled :
        config.AccessPointEnabled = false;
        config.defaultConfig      = false;
        config.dhcp               = false;

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
                else cfg_args_err = cfg_args_err + "{device_name : Invalid}";
            }

            if (s_argName == "device_username"){
                if ( checkStringStd(s_arg) ) config.DeviceUsername = s_arg;
                else cfg_args_err = cfg_args_err + "{device_username : Invalid}";
            }

            if (s_argName == "device_password"){
                if ( checkStringStd(s_arg)
                    && (s_arg.length() > 5 || s_arg.length() == 0)
                ){
                    if ( s_arg.length() > 0) config.DevicePassword = s_arg;
                }
                else {
                    cfg_args_err = cfg_args_err + "{device_password : Invalid}";
                }
            }

            if (s_argName == "ssid"){
                if ( checkSSID(s_arg) ) config.ssid = s_arg;
                else cfg_args_err = cfg_args_err + "{ssid : Invalid}";
            }

            if (s_argName == "ssid_password"){
                if ( s_arg.length() > 0 && checkStringStd(s_arg) )
                    config.ssid_password = s_arg;
                else {
                    cfg_args_err = cfg_args_err + "{ssid_password : Invalid}";
                }
            }

            if (s_argName == "mqtt_broker_topic"){
                if ( checkStringStd(s_arg) ) config.MQTTBrokerTopic = s_arg;
                else cfg_args_err = cfg_args_err + "{mqtt_broker_topic : Invalid}";
            }

            if (s_argName == "mqtt_broker_username"){
                if ( checkStringStd(s_arg) ) config.MQTTBrokerUsername = s_arg;
                else cfg_args_err = cfg_args_err + "{mqtt_broker_username : Invalid}";
            }

            if (s_argName == "mqtt_broker_password"){
                if ( s_arg.length() > 0 && checkStringStd(s_arg) )
                    config.MQTTBrokerPassword = s_arg;
                else {
                    cfg_args_err = cfg_args_err + "{mqtt_broker_password : Invalid}";
                }
            }

            if (s_argName == "mqtt_broker_port"){
                if ( check_uint16_t(s_arg) ) config.MQTTBrokerPort = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{mqtt_broker_port : Invalid}";
            }

            if (s_argName == "dhcp") config.dhcp = true;

            if (s_argName == "ip_0"){
                if( checkIPOctet(s_arg) ) config.IP[0] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{ip_0 : Invalid}";
            }
            if (s_argName == "ip_1"){
                if( checkIPOctet(s_arg) ) config.IP[1] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{ip_1 : Invalid}";
            }
            if (s_argName == "ip_2"){
                if( checkIPOctet(s_arg) ) config.IP[2] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{ip_2 : Invalid}";
            }
            if (s_argName == "ip_3"){
                if( checkIPOctet(s_arg) ) config.IP[3] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{ip_3 : Invalid}";
            }

            if (s_argName == "nm_0"){
                if( checkIPOctet(s_arg) ) config.Netmask[0] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{nm_0 : Invalid}";
            }
            if (s_argName == "nm_1"){
                if( checkIPOctet(s_arg) ) config.Netmask[1] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{nm_1 : Invalid}";
            }
            if (s_argName == "nm_2"){
                if( checkIPOctet(s_arg) ) config.Netmask[2] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{nm_2 : Invalid}";
            }
            if (s_argName == "nm_3"){
                if( checkIPOctet(s_arg) ) config.Netmask[3] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{nm_3 : Invalid}";
            }

            if (s_argName == "gw_0"){
                if( checkIPOctet(s_arg) ) config.Gateway[0] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{gw_0 : Invalid}";
            }
            if (s_argName == "gw_1"){
                if( checkIPOctet(s_arg) ) config.Gateway[1] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{gw_1 : Invalid}";
            }
            if (s_argName == "gw_2"){
                if( checkIPOctet(s_arg) ) config.Gateway[2] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{gw_2 : Invalid}";
            }
            if (s_argName == "gw_3"){
                if( checkIPOctet(s_arg) ) config.Gateway[3] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{gw_3 : Invalid}";
            }

            if (s_argName == "mqttip_0"){
                if( checkIPOctet(s_arg) ) config.MQTTBrokerIP[0] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{mqttip_0 : Invalid}";
            }
            if (s_argName == "mqttip_1"){
                if( checkIPOctet(s_arg) ) config.MQTTBrokerIP[1] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{mqttip_1 : Invalid}";
            }
            if (s_argName == "mqttip_2"){
                if( checkIPOctet(s_arg) ) config.MQTTBrokerIP[2] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{mqttip_2 : Invalid}";
            }
            if (s_argName == "mqttip_3"){
                if( checkIPOctet(s_arg) ) config.MQTTBrokerIP[3] = s_arg.toInt();
                else cfg_args_err = cfg_args_err + "{mqttip_3 : Invalid}";
            }

        }

        if ( config.DevicePassword == DEFAULT_DEVICE_PASSWORD ){
            cfg_args_err = cfg_args_err
                + "{device_password : Invalid : still '"+DEFAULT_DEVICE_PASSWORD+"'}";
        }

    } else {
        cfg_args_err = cfg_args_err + "{error : no-args}";
    }

    print_Config();


    if ( cfg_args_err != "" ){
        Serial.println(" config args invalid : " + cfg_args_err );

// TODO this is here until i get tempConfig copying for the config page :
        ReadConfig();
        print_Config();

    } else {

// TODO copy tempConfig into config

    }
}

void handlePOSTConfigPage()
{
    cfg_args_parse();

    if ( cfg_args_err == "" ){
        server.send ( 200, "text/html", PAGE_WaitAndReload );
        WriteConfig();
        ConfigureWifiStartWeb();
    } else {
        server.send ( 400, "text/html", generate_config_page() );
    }
}

#endif
