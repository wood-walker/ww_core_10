// b_setup.h
//
// setup() function only
//==================================================================================
void setup()
{
    // pin input/output definitions **************************************************
    pinMode(SysCur,   INPUT); // Current Sense
    pinMode(RawVcc,   INPUT); // Battery Voltage
    pinMode(MiInput,  INPUT); // mic
    pinMode(MiEnable, INPUT);
    pinMode(MoEnable, OUTPUT); // motor pins -------------------------------
    pinMode(MoRi1 ,   OUTPUT);
    pinMode(MoRi2 ,   OUTPUT);
    pinMode(MoRiPwm,  OUTPUT);
    pinMode(MoLe1,    OUTPUT);
    pinMode(MoLe2,    OUTPUT);
    pinMode(MoLePwm,  OUTPUT);
    pinMode(EnEnable, OUTPUT);
    digitalWrite(EnEnable, LOW);          // motor encoder -----------------------

    pinMode(EnRight, INPUT_PULLUP);
    pinMode(EnLeft, INPUT_PULLUP);
    pinMode(MFRight, INPUT_PULLUP);    // mechanical feeler ------------------
    pinMode(MFLeft,  INPUT_PULLUP);

    pinModeFrank(IrEnLeft, INPUT);    // IR feeler ---------------------------
    pinModeFrank(IrEnRight, INPUT);
    pinModeFrank(LiEnable, INPUT);    // light sensor front enable
    pinMode(LiFront,   INPUT);         // light sensor front analog input signal
    pinMode(LiBackEnable, INPUT);
    pinMode(LiBack,   INPUT);          // light sensor back  analog input signal
    pinMode(VibSwitch, INPUT_PULLUP);  // vibration switch

    pinMode(SpEnable, OUTPUT);         // Audio Pin
    digitalWrite(SpEnable, LOW);       // disable amplifier
    pinModeFrank(BtReset, OUTPUT);     // Bluetooth-----------------------------
    digitalWriteFrank(BtReset, LOW);   // HIGH = operation
    delay(10);
    digitalWriteFrank(BtReset, HIGH);   // HIGH = operation
    // pinModeFrank(BtEnable, OUTPUT);
    // digitalWriteFrank(BtEnable, HIGH); // LOW = switch on the BT ?????  I think it will change the mode
    //serialBT.begin(serialBTBaud);
    pinMode(WiEnable, OUTPUT);         // ESP Pins ---------------------
    digitalWrite(WiEnable, LOW);       // shut off the ESP
    pinMode(WiPin2, OUTPUT);
    pinMode(WiPin15, OUTPUT);
    pinMode(WiRst, OUTPUT);
    pinModeFrank(WiProg, OUTPUT);
    espStatusFlag = 0;                // Global available flag for ESP Status on/off

    pinMode(Led13, OUTPUT);
    digitalWrite(Led13, LOW);
    pinMode(WiEnable, OUTPUT);         // Wireless = ESP 8266 ------------------
    digitalWrite(WiEnable, LOW); // Shut off the WiFi Module
    pinMode(WiPin2, OUTPUT);
    pinMode(WiPin15, OUTPUT);
    pinMode(WiRst, OUTPUT);
    pinModeFrank(WiProg, OUTPUT);

    // variable settings ******************************************************************
    menu.startact = 0;      //Menu stuff intialization
    menu.listact = 6;       // here you can preset a menue item as selected - good for testing special menue items 0 = firt item in menu list
    menu.levelact = 0;      // here you can specify the actual level in the actual menue - 0 = first menu level
    menu.itemactlevel[0] = 0;
    menuCollect();
    activeAction = mFeelerListen; // set menu state

    // RTC preparation  *******************************************************************
    pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL); // switch RTC to external cristal
    RTC_SetHourMode(RTC, 0);   // 0 = 24-hour mode, 1 = 12-hour mode

    // setup via functiion calls ***********************************************************
    neoOff();          // switch off all neopixels
    Wire.begin();      // Necessary for eyes and SD-card
    initializeEyes();  // must be done once at the start of each program
    SD.begin(SdCS);
    SPI.setClockDivider(SdCS);
    pinMode(SdDetect, INPUT_PULLUP); // card switch will pull it to ground if card is inserted
    testSD();          // test if there is a functional SD card and display result on OLED
    analogReadResolution(12);

    // Play a sound if SD available
    if (SdOK == 1) {
        // hi-speed SPI transfers
        playFile("frog4.wav", 0);    // You should have this on your SD-Card or remove the SD Card
    } else { // give the user some time to read the SD-card info when no sound is played!
        delay(800);
    }
    delay(1000);
    menuShow();
}



