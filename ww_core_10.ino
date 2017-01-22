// ****************************************************************************
//      - - - /    ww_core_10.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
// ww_core_06  2015-10-24
// ww_core_08  2016-08-26  - läuft
// ww_core_08  2016-10-08  - läuft
// ww_core_09  2016-11-06  - MQTT rudimentär dazu gebaut
// ww_core_10  2017-01-18  - MQTT wieder entfernt
// ww_core_10              - neue wwEyes Benamsung dazu
// ww_core_10  2017-01-20  - UNIX Time aus ESP und Woody RTC ->OK
// ww_core_10  2017-01-21  - ESP und Wlan-Status ->OK


#define ProgRelease "10"
// 
//=================================================================================
#include <Wire.h>           // necessary for the I2C Communication
#include <Adafruit_GFX.h>

#include <wwEyes.h>      // Header with instructions for using the OLED displays
#include <wwFrankPin.h>       // Defines all pin names of the Frankenstein DUE

//#include <avr/dtostrf.h>    // used for temp string generation in f_monitor
#include <SPI.h>            // used for SD-card communication
#include <SD.h>             // SD-card commands

#include <wwAudio.h>          // DAC support for WAV file playback

#include <Adafruit_NeoPixel.h> // Neo Pixel support

#include <ELClient.h>
#include <ELClientCmd.h>
#include <ELClientMqtt.h>

/*
**Important** For at least the MQTT sketch to work you must turn off the UART debug log in esp-link (on
the Debug Log page). The reason is that otherwise esp-link prints too much to its uart and then
misses incoming characters.
*/

// for the Giro
#include <I2Cdev.h>        // I2C communication commands
#include <MPU6050_6Axis_MotionApps20.h>  //Giro Library

//====================================================== <Adafruit_NeoPixel.h> ============================
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NeoData, NEO_GRB + NEO_KHZ400);
//====================================================== <woody_eyes.h> ============================
// set up variables using the WoodyEyes.h:
wwEyes eye;
//====================================================== <SD.h> ============================
// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
//=================================================================================
//=================================================================================

