KhaosESP_SonoffEasyConfig
-------------------------
Karl "Kount-Khaos" Hoskin : 17th February 2017.


This is not yet finished.

The idea
--------

The button :

    quick click : briefly pressing it will toggle the relay

    $ secs :  holding it down for about 4 secs will reset it into WPS mode.
    8 secs :  holding it down for about 8 secs will reset it into Wifi Access Point mode.
        Where it broadcasts on
            SSID : ESP8266
            password : password

    12 secs : it will restore the default config.


When in Access Point Mode.
-------------------------

Get a laptop/phone/tablet etc to connect to the above SSID and navigate to :

    http://192.168.4.1/cfg

It is assumed that the Access Point Mode is just for initial configuration.

So once the cfg form is submitted it will try to connect to the SSID/Password specified in the page in "Station" mode.

If there are obvious errors in the configuration it will stay in AP mode , showing the errors.




THIS IS STILL WORK IN PROGRESS ( and all the above isn't fully complete yet )

Eventually this will :

(-) Web config form show errors by highlighting the field.
(-) Web config form to store the unsaved changes in tempConfig,
    only get submitted when all entries are okay.

(-) have Web user login (sessions, random cookies) to stop anyone from changing things.
(-) send and receive mqtt messages. ( with encryption , if possible )
(-) Be able to read DS18b20 temperature sensors on one of the spare pins.
    ( and send regular mqtt updates )
(-) have the other spare pins doing something.


(-) hopefully have SSL on the webserver (if possible)

(-) (This is done apart from the code to go into WPS mode)
   WPS mode if button is held for 4 seconds,
    (led off, goes on when in WPS mode)
   AP mode if held for 8 seconds.
    (led flashing every 1/4 sec when in AP mode)
   Default Config if held for 12 seconds.
    (led permanently on)

(-) OTA updates (maybe)



https://github.com/bblanchon/ArduinoJson/tree/master/examples


