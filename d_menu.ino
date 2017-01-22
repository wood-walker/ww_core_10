// ****************************************************************************
//      - - - /    d_menu.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************

/*
struct menuvariables {
byte contentact;      // active item in each level of the actual menu chain
byte listcount;    // # of items in each level of the actual menu chain
byte list[10];     // list of the numbers of all menu entries in the actual display
byte listact;  // active item in the menu.list collection
};
menuvariables menu;
*/

//==================================================================================
struct menustructure {
    int id;
    byte level;
    char text[9];
    byte action;
};
//==================================================================================
#define menuitems 46   // total number of menu items

// menu definition
static const menustructure menucontent[] {
    //               ID, level, Text, action
    menustructure {mWalker,     0, "Walker",       m_deeper},
    menustructure {mSleepMode, 1, "Sleep",         m_action},

    menustructure {mMotorTest, 1,   "test run",    m_action},
    menustructure {mBump, 1,   "bump run",         m_action},
    //  menustructure   {1300, 1,   "Settings",    m_deeper},
    //  menustructure   {1310, 2,     "maxspeed",  m_action},
    //  menustructure   {1320, 2,     "behavior",  m_action},
    //  menustrucure   {1330, 2,     "RETURN",     m_return},
    menustructure {1400, 1,   "RETURN",            m_return},

    menustructure {mRTC, 0, "RTC",                 m_deeper},
    //  menustructure   {2100, 1,   "Set Date",    m_action},
    menustructure {mRTCDisplayTime, 1, "TIME",     m_action},
    menustructure {mOldClock, 1,   "OldClock",     m_action},
    menustructure {mOldClockLow, 1, "OldLowEn",    m_action},
    menustructure {mRTCSetTime, 1, "Set TIME",     m_action},
    menustructure {mRTCEspSync, 1,   "ESP Sync",   m_action},
    //menustructure   {2220, 2,     "RETURN",      m_return},
    menustructure {0, 1,                "RETURN",  m_return},

    menustructure {mEye, 0,      "Display",     m_deeper},
    menustructure {mEyeCircle, 1,  "Circle",    m_action},
    menustructure {mEyeIcon, 1,    "eye icon",  m_action},
    menustructure {mEyeLScroll, 1,  "R Scroll", m_action},
    menustructure {mEyeRScroll, 1,  "L Scroll", m_action},
    //menustructure {mEyeNoScroll, 1, "NoScroll", m_action},
    menustructure {mEyeDim, 1,      "dim",      m_action},
    //menustructure {mEyeBright, 1,   "no dim",   m_action},
    menustructure {0, 1,            "RETURN",   m_return},

    menustructure {mMon, 0,        "Monitor",   m_deeper},
    menustructure {mMonEnergy, 1,    "Energy",  m_action},
    menustructure {mMonIR, 1,        "IR-sens", m_action},
    menustructure {mMonLight, 1,     "Li-feel", m_action},
    menustructure {mMonKnock, 1,     "Knock",   m_action},
    menustructure {mMonTemp, 1,      "Temp.",   m_action},
    menustructure {mMonEncoder, 1,   "Encoder", m_action},
    menustructure {mMonGiro, 1,      "Giro",    m_action},
    menustructure {0, 1,             "RETURN",  m_return},

    menustructure {mNeo, 0,        "NeoPix",    m_deeper},
    menustructure {mNeoRainbow, 1,   "Rainbow", m_action},
    menustructure {mNeoRandom,  1,   "Random",  m_action},
    menustructure {mNeoNight,   1,   "Night",   m_action},
    menustructure {mNeoOff, 1,       "Off",     m_action},
    menustructure {0, 1,             "RETURN",  m_return},

    menustructure {mSound, 0,     "Sound",      m_deeper},
    menustructure {mPlay1, 1,       "Walkuer",  m_action},
    menustructure {mMicAClap, 1,    "MicAclap", m_action},
    menustructure {0, 1,            "RETURN",   m_return},
    //menustructure   {5100, 1,   "voice",     m_action},

    //menustructure {mBT, 0,        "Bluetoo.",   m_deeper},
    //menustructure {mBTPio10, 1,     "PIO10",    m_action},
    //menustructure {mBTPio11, 1,     "PIO11",    m_action},
    //menustructure {mBTInfo, 1,      "Info",     m_action},
    //menustructure {0, 1,            "RETURN",   m_return},

    menustructure {mESP, 0,       "Wlan",       m_deeper},
    menustructure {mESPOn, 1,       "ESP on",   m_action},
    menustructure {mESPProg, 1,     "ESP prog", m_action},
    menustructure {mESPOff, 1,      "ESP off",  m_action},
    //menustructure {mESPInfo, 1,     "Info",     m_action},
    menustructure {mEspMqtt, 1,     "MQTT",     m_action},
    menustructure {0, 1,            "RETURN",   m_return},

    menustructure {mOff, 0,       "Off",        m_deeper},
    menustructure {mOffYes, 1,      "Off ?",    m_action},
    menustructure {0, 1,            "RETURN",   m_return}
};

//menustructure   {mSleepMode, 1, "Sleep",     m_action},
//menustructure   {mUSBOff, 1,    "USB Off",     m_action},
//menustructure   {mUSBOn, 1,     "USN On",     m_action},

//=======================================================================================
void feelerListen()
{
    // is one of the feeler pressed?
    if (digitalRead(MFLeft) == 0) {
        if (activeAction == mOldClockLowLoop) oldClockLowOff();
        buttonDown();
        delay(200);     // debounce the button
        buttonPressed = 1;
    }
    if (digitalRead(MFRight) == 0) {
        if (activeAction == mOldClockLowLoop) oldClockLowOff();
        buttonSelect();
        delay(200);     // debounce the button
        buttonPressed = 2;
    }

}
//=======================================================================================
void buttonDown()
{
    menu.listact ++ ;
    if (menu.listact > menu.listcount) {  // repeat from the beginning because we don't have 3 buttons for up/down/select
        menu.listact = 0;
    }
    menuShow();
    activeAction = mFeelerListen; // if we go back to the previous menu we can switch off all previouse actions
}
//=======================================================================================
void buttonSelect()
{
    int start, menuItem;
    byte i;

    switch (menucontent[menu.list[menu.listact]].action) { // look into the menuStructure what to do now = actionIndicator
    // ----------------------------------------------
    case m_deeper:                                        //we should jump into the next deeper level
        menu.itemactlevel[menu.levelact] = menu.listact;  // remember the actual item in this level before going+ deeper
        menu.levelact++;                                  // we switch to the next deeper level
        menu.startact = menu.list[menu.listact] + 1;      // the first item of the new sub menu is located beneath the selected item
        menu.listact = 0;                                 // make the first item in the list the active icon
        menuCollect();
        menuShow();
        leftEyeShow();
        break;
    // ----------------------------------------------
    case m_return:             //we should return back to the next higher level
        menu.levelact--;
        if (menu.levelact > 0) {
            for (i = menu.startact - 1; i >= 0; i--) { // lock upwards in the menu structure
                if (menucontent[i].level < menu.levelact) {
                    i++;
                    break;
                }
            }
        } else {
            i = 0;
        }
        menu.startact = i;  // register the first item in the new list
        menu.listact = menu.itemactlevel[menu.levelact];    // call back the actual item for the new level
        menuCollect();
        menuShow();
        activeAction = mFeelerListen; // if we go back to the previous menu we can switch off all previouse actions
        blankEye(eyeLeft);
        break;
    // ----------------------------------------------
    case m_action:
        onlyonce = 1;
        activeAction = menucontent[menu.list[menu.listact]].id;
        break;
    }
}
//=======================================================================================
void menuCollect()
{
    byte j = -1;

    for (byte i = menu.startact; i < menuitems; i++) {
        if (menucontent[i].level == menu.levelact) {      // search for new menu items in this level
            j++;
            menu.list[j] = i;                          //store the item id in the menu.list
        } else if (menucontent[i].level < menu.levelact) { //if the next item in the list is from an higher level (lower number) we collected all items already
            break;
        }
    }
    menu.listcount = j;
}
//=======================================================================================
void menuShow()
{
    byte fromItem;
    byte toItem;

    eye.select(eyeRight);
    eye.dim(false);
    eye.stopscroll();
    eye.setTextSize(2);
    eye.setTextColor(WHITE,BLACK);
    eye.clearBuffer(); // clean the splash screen buffer
    eye.setCursor(0, 0);

    if (menu.listact > 3) {         // we have to start the list at a later point - menu counts 0,1,2,3
        fromItem = menu.listact - 3;
    } else {
        fromItem = 0;
    }

    if (menu.listcount < 3) {  // we have equal or less than 3 items in the list
        toItem = menu.listcount;
    } else {
        toItem = fromItem + 3;
    }
    //Menuebene Ausgeben
    for (byte i = fromItem; i <= toItem; i++) {  //produces 4 lines: 0,1,2,3
        if ( i == menu.listact) {
            eye.print("-");
            //eye.setTextColor(BLACK, WHITE); // 'inverted' text
        } else {
            eye.print(" ");
            //eye.setTextColor(WHITE);
        }
        eye.println(menucontent[menu.list[i]].text);
    }
    eye.display();
}
//=======================================================================================
void leftEyeShow()
{
    eye.select(eyeLeft);
    eye.setTextSize(2);
    eye.setTextColor(WHITE,BLACK);
    eye.clearBuffer(); // clean the splash screen buffer
    eye.setCursor(0, 0);

    switch (menucontent[menu.startact].id) { // use the ID of the first item of the actual menu to deside what to do
    case mESPOn:                // first item of the ESp menu
        espDisplayStatus();
        break;
    }
    eye.display();
}
//=======================================================================================
//=======================================================================================
