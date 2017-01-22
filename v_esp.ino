// ****************************************************************************
//      - - - /   v_esp.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
//==================================================================================
void espOn()
// Try to switch on the ESP, do a reset and give feedback at the right eye
{
    unsigned long startMillis = millis();
    bool toggle = 0;
    espSwitchOn(1);
    do {
        esp.Process();  // you have to run this frequently to get callbacks from the ESP!

        if (toggle) {
            eye.setTextColor(WHITE, BLACK);
            toggle = 0;
        } else {
            eye.setTextColor(BLACK, WHITE);
            toggle = 1;
        }
        eye.setCursor(0, 32);
        eye.print("          ");
        eye.setCursor(0, 32);
        eye.print(wifiStatusTxt[wifiStatusFlag]);
        eye.display();
        delay(100);
    } while (wifiStatusFlag != STATION_GOT_IP & (startMillis + 10000 > millis()));

    eye.setTextColor(WHITE, BLACK);
    eye.setCursor(0, 32);
    eye.print("          ");
    eye.setCursor(0, 32);
    eye.print(wifiStatusTxt[wifiStatusFlag]);
    eye.display();

    SerialESP.print("Woody: ");
    SerialESP.println(wifiStatusTxt[wifiStatusFlag]);

    if(wifiStatusFlag == STATION_GOT_IP) {    // you only have to wait for the UNIX time if there is a connection
        delay(1000);                          // give the ESP at least one second for getting the UNIX Time from the WWW
        startMillis = millis();
        uint32_t timeEsp;
        do {
            timeEsp = cmd.GetTime();
            esp.Process();  // you have to run this frequently to get callbacks from the ESP!
            delay(300);
        } while ((timeEsp == 0) & (startMillis + 10000 > millis()));  // Wait no longer than 10 Seconds for the time
        SerialESP.print("ESP UNIX Time: ");
        SerialESP.println(timeEsp);
        eye.setCursor(0, 48);
        eye.print(timeEsp);
        eye.display();
    }
    SerialESP.print("Woody UNIX Time: ");
    SerialESP.println(RTC_GetUnixTime());
}
//==================================================================================
void espSwitchOn(bool eyeOutputFlag)
// switch on the ESP, do a reset, try to connect Woody with ESP via SerialESP
{
    if(eyeOutputFlag) {
        eye.select(eyeLeft);
        eye.setTextColor(WHITE, BLACK);  //overwrites old pixel in letters
        eye.clearBuffer();
        eye.setCursor(0, 0);
        eye.println("con. ESP");
        eye.display();
    }
// Set Pin for operation
    digitalWrite(Led13, HIGH);
    digitalWrite(WiEnable, HIGH);
    digitalWrite(WiPin2, HIGH);
    digitalWrite(WiPin15, LOW);
    digitalWriteFrank(WiProg, HIGH);
// RESET
    delay(10);
    digitalWrite(WiRst, LOW);
    delay(10);
    digitalWrite(WiRst, HIGH);
    delay(10);
    espStatusFlag = 1;
    SerialESP.begin(115200);
// Sync-up with esp-link, this is required at the start of any sketch and initializes the
// callbacks to the wifi status change callback. The callback gets called with the initial
// status right after Sync() below completes.
    esp.wifiCb.attach(wifiCallBack); // wifi status change callback, optional (delete if not desired)
    bool ok;
    int failCounter = 0;
    do {
        ok = esp.Sync();      // sync Woody with esp-link, blocks for up to 2 seconds
        if (!ok) {
            ++failCounter ;
            if(eyeOutputFlag) {
                eye.setCursor(0, 16);
                eye.println("sync trial");
                eye.println(failCounter);
                eye.display();
            }
        }
    } while(!ok && (failCounter < 5));
    if(ok) {
        SerialUSB.println("EL-Client synced!");
        if(eyeOutputFlag) {
            eye.setCursor(0, 16);
            eye.print("ESP Con OK");
            eye.display();
        }
    } else {
        if(eyeOutputFlag) {
            eye.println("ESP fail!");
            eye.display();
        }
    }
}
//==================================================================================
// Callback made from esp-link to notify of wifi status changes
// Here we just print something out for grins
void wifiCallBack(void* response)
// wifiStatusFlag is global available to check the status of the Wifi
{
    ELClientResponse *res = (ELClientResponse*)response;
    if (res->argc() == 1) {
        res->popArg(&wifiStatusFlag, 1);

        if(wifiStatusFlag == STATION_GOT_IP) {
            SerialUSB.println("WIFI Got IP");
        } else {
            SerialUSB.print("WIFI NOT READY: ");
            SerialUSB.println(wifiStatusFlag);
        }
    }
}
//==================================================================================
void espProg()
{
    // Set Pin for operation
    digitalWrite(Led13, HIGH);
    digitalWrite(WiEnable, HIGH);
    digitalWrite(WiPin2, HIGH);
    digitalWrite(WiPin15, LOW);
    digitalWriteFrank(WiProg, LOW);  // ready for programming
    // RESET
    delay(10);
    digitalWrite(WiRst, LOW);
    delay(10);
    digitalWrite(WiRst, HIGH);
    espStatusFlag = 2;

    SerialESP.end();

    pinMode(14, INPUT);
    pinMode(15, INPUT);

    //  while (SerialESP.available()) {    // empty the buffer
    //SerialESP.read();
    //  }
}
//==================================================================================
void espOff(bool eyeOutputFlag)
{
    digitalWrite(Led13, LOW);
    digitalWrite(WiEnable, LOW);
    espStatusFlag = 0;
    if(eyeOutputFlag) {
        eye.select(eyeLeft);
        eye.setTextColor(WHITE, BLACK);  //overwrites old pixel in letters
        eye.clearBuffer();
        eye.setCursor(0, 0);
        eye.println("ESP");
        eye.println("   is OFF!");
        eye.display();
    }
}
//==================================================================================
void espDisplayStatus()
{
// Display general esp Status info on the left eye
    eye.setCursor(0, 0);
    if (espStatusFlag == 0)
        eye.print("ESP  OFF");
    else
        eye.print("ESP  ON");
    eye.setCursor(0, 32);
    eye.println("WIFIstatus");
    eye.print(wifiStatusTxt[wifiStatusFlag]);

}
//==================================================================================
//==================================================================================


