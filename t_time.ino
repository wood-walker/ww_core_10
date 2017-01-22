// ****************************************************************************
//      - - - /    t_time.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
void displayTime()
{
    eye.select(eyeLeft);
    eye.clearBuffer();
    printTime();
    if (!pmc_osc_is_ready_32kxtal()) { // check if the external christal is running
        eye.print("no ex cris");
    } else {
        eye.print("ex cris OK");
    }
    eye.print(RTC_GetUnixTime());

    if (analogRead(LiFront) > 3500) {  //dim the display if we are in a dark environment
        eye.select(eyeRight);
        eye.dim(true);
        eye.select(eyeLeft);
        eye.dim(true);
    } else {
        eye.select(eyeRight);
        eye.dim(false);
        eye.select(eyeLeft);
        eye.dim(false);
    }
    //eye.println(analogRead(LiFront));
    eye.display();
}
// ================================================================================
void setAlarm()
{
    //RTC_SetAlarm(RTC, 23, 40,0);
}
// ================================================================================
uint32_t RTC_GetUnixTime()
{
    uint8_t daysInMonth [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t hour, minute, second;
    uint16_t year;
    uint8_t month, day, week;
    uint16_t days;

    RTC_GetDate( RTC, &year, &month, &day, &week);    //Get the actual date
    days = day-1;                          // we will count todays seconds via todays hour, minute, second
    for (int i = 1; i < month; ++i)        // look up the days per month and add
        days += daysInMonth[i - 1];        // days of the month of this year excluding actual month
    if (month > 2 && year % 4 == 0)        // every year which can be devided by 4 has a 29th of February to add (valid till 2100 ;-)
        ++days;
    year   = year - 1970;                  // Start counting the seconds from 1970
    days += 365 * year + (year + 1) / 4;   // Add extra days for the additional 29th of February in every 4th year.

    RTC_GetTime(RTC, &hour, &minute, &second); // get the actual time
    return ((days * 24 + hour) * 60 + minute) * 60 + second; // seconds from 01.01.1970 00:00:00
}
// ================================================================================
// Please remember the unixTime in general is calculated as UTC 0!!!
uint8_t RTC_SetByUnixTime(uint32_t unixTime, uint8_t UTC)
{
    uint8_t hour, minute, second;
    uint16_t year, years_Unix;
    uint8_t month, day, week;

    uint32_t days_Unix, days_LastYear, minutes_LastDay, secs_LastDay, leap;

    unixTime += UTC*60*60;                  // add or subtract the necessary seconds for the given UTC

    secs_LastDay = unixTime % (24*60*60);   // calculate how many seconds are left in the last day
    second = secs_LastDay % 60;             // calculate how many seconds are left in the last minute
    minutes_LastDay = secs_LastDay / 60;    // calculate how many full minutes are left in the last day
    minute = minutes_LastDay % 60;          // calculate how many minutes are left in the last hour
    hour = minutes_LastDay / 60;            // calculate how many full hours are left in the last day
    RTC_SetTime(RTC, hour, minute, second); //set the time to 05:04:50

    days_Unix = unixTime/(24 * 60 * 60);           // calculate how many full days fits into the given time
    years_Unix = (((days_Unix * 4) + 2) / 1461);   // calculate how many full years this full days make
    year = years_Unix + 70;                        // add this because of the Unix start in 1970

    leap = !(year & 3);
    days_Unix -= ((years_Unix * 1461) + 1) / 4;    // calculate how many full days are left in the last year
    days_Unix += (days_Unix > 58 + leap) ? ((leap) ? 1 : 2) : 0;
    month = ((days_Unix * 12) + 6)/367;            //generates month counting from 0 to 11
    day = days_Unix + 1 - ((month * 367) + 5)/12;
    month += 1;                                    // generate month counting from 1 to 12
    year = year + 1900;                            // add the missing 1900 years

    RTC_SetDate(RTC, year, month, day, 1);         // I do not care about the weeks here...

    return 0;
}
// ================================================================================
int timeEspSync()
{
    blankEye(eyeLeft);
    if (espStatusFlag == 0) {  // the Esp is switched off
        eye.println("ESP is OFF");
        eye.println("");
        eye.print("Switch ON!");
        eye.display();
        return 1;
    }
    if (wifiStatusFlag != STATION_GOT_IP) {  // the Esp is not connected to a network
        eye.println("ESP is ON!");
        eye.println("has no IP");
        eye.println("");
        eye.print("Check ESP");
        eye.display();
        return 2;
    }
    uint32_t timeEsp = cmd.GetTime();
    if (timeEsp == 0) {                      // the Esp has not got an actual UNIX time
        eye.println("IP is OK");
        eye.println("no time!");
        eye.println("");
        eye.print("Check ESP");
        eye.display();
        return 3;
    }
    eye.println("UNIX");
    eye.println("   time");
    eye.println("      sycn");
    eye.display();
    delay(1000);
    RTC_SetByUnixTime(timeEsp,0);            // UTC = 0 because the ESP calculates it to my UTC already
    return 0;
}
// ================================================================================
void setTime()
{
    byte hour, minute, second;
    byte digit[6];
    byte digitMax[] = {2, 0, 5, 9, 5, 9}; //digitMax[1] will be set according to the value of digit 0!
    byte selected = 0;
    byte delayTime = 0;

    RTC_GetTime(RTC, &hour, &minute, &second);

    digit[0] = hour / 10;
    digit[1] = hour % 10;
    digit[2] = minute / 10;
    digit[3] = minute % 10;
    digit[4] = second / 10;
    digit[5] = second % 10;

    while (selected < 6) {
        timeSetBlink(digit[0], digit[1], digit[2], digit[3], digit[4], digit[5], selected);
        delay(delayTime); //delay for debouncing if we got a feeler action
        delayTime = 0;
        if (feelerListenAction() == 1) { // count one up
            if (digit[selected] < digitMax[selected]) {
                digit[selected]++;
            } else {
                digit[selected] = 0;
            }
            delayTime = 200;
        }
        if (feelerListenAction() == 2) { // next digit or after last save into RTC
            if (selected == 0) { //set the max value of the hours second digit according to the hours first digit
                if (digit[0] == 2) { //we can have only 20, 21, 22 and 23
                    digitMax[1] = 3;
                } else {
                    digitMax[1] = 9;
                }
            }
            selected++;
            delayTime = 200;
        }
    }
    hour = digit[0] * 10 + digit[1];
    minute = digit[2] * 10 + digit[3];
    second = digit[4] * 10 + digit[5];
    RTC_SetTime(RTC, hour, minute, second);
}
// ================================================================================
void timeSetBlink(byte h1, byte h2, byte m1, byte m2, byte s1, byte s2, byte selected)
{
    byte i;
    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.print(h1);
    eye.print(h2);
    eye.print(":");
    eye.print(m1);
    eye.print(m2);
    eye.print(":");
    eye.print(s1);
    eye.println(s2);
    for (i = 0; i < 6; i++) {
        if (i == selected) {
            eye.print("^");
        } else {
            eye.print(" ");
        }
        if (i % 2 != 0) { // one more to hop over the ':'
            eye.print(" ");
        }
    }
    eye.display();
}
// ================================================================================
byte feelerListenAction()
{
    if (digitalRead(MFRight) == 0)  return 1;
    if (digitalRead(MFLeft) == 0)   return 2;
    return 0;
}
// ================================================================================
void printTime()
{
    uint8_t hour, minute, second;
    RTC_GetTime(RTC, &hour, &minute, &second);
    eye.setCursor(10, 16);
    if (hour < 10)   eye.print("0");
    eye.print(hour);
    eye.print(":");
    if (minute < 10)   eye.print("0");
    eye.print(minute);
    eye.print(":");
    if (second < 10)   eye.print("0");
    eye.println(second);
}
// ================================================================================
void oldClock()
{
    static byte secondOld = 0;
    byte hour, minute, second;
    int i;
    char file[3];
    RTC_GetTime(RTC, &hour, &minute, &second);

    if (second == 0) {           // procedure to flip over from 59 to 0
        if (secondOld > 59) {
            secondOld = 0;
        }
    }
    if (second > secondOld) {                   // update only every second
        eye.select(eyeRight);
        eye.sd2Eye(itoa(hour, file, 10));
        eye.display();
        eye.select(eyeLeft);
        eye.sd2Eye(itoa(minute, file, 10));
        eye.display();
        secondOld = second + 1;

        for (i = 10; i < 50; i++) {
            strip.setPixelColor(1, i,    0,   0);
            strip.show();   // push the data to the pixels
            delay(10);
        }
        for (; i >= 0; i--) {
            strip.setPixelColor(1,   i,    0,   0);
            strip.show();   // push the data to the pixels
            delay(2);
        }
        strip.setPixelColor(1,   0,    0,   0);
        strip.show();
    }
}
// ================================================================================
void oldClockLowPrepare()
{
    pmc_disable_udpck(); //switch off USB clock
    pmc_disable_periph_clk(3);   // real-time timer
    pmc_disable_periph_clk(4);   // watchdog timer

    pmc_disable_periph_clk(21);  // HSMCI (SD/MMC ctrl, N/C)
    pmc_disable_periph_clk(22);  // TWI/I2C bus 0 (i.MX6 controlling)
    //pmc_disable_periph_clk(23);  // TWI/I2C bus 1                         für Augen und SD Card
    //pmc_disable_periph_clk(24);  // SPI0                                  für Augen und SD Card
    pmc_disable_periph_clk(25);  // SPI1
    pmc_disable_periph_clk(26);  // SSC (I2S digital audio, N/C)

    pmc_disable_periph_clk(27);  // timer/counter 0
    pmc_disable_periph_clk(28);  // timer/counter 1
    pmc_disable_periph_clk(29);  // timer/counter 2
    pmc_disable_periph_clk(30);  // timer/counter 3
    pmc_disable_periph_clk(31);  // timer/counter 4
    pmc_disable_periph_clk(32);  // timer/counter 5
    pmc_disable_periph_clk(33);  // timer/counter 6
    pmc_disable_periph_clk(34);  // timer/counter 7
    pmc_disable_periph_clk(35);  // timer/counter 8
    pmc_disable_periph_clk(36);  // PWM

    pmc_disable_periph_clk(38);  // DAC ctrl
    pmc_disable_periph_clk(39);  // DMA ctrl
    pmc_disable_periph_clk(40);  // USB OTG high-speed ctrl
    pmc_disable_periph_clk(41);  // random number generator
    pmc_disable_periph_clk(42);  // ethernet MAC - N/C
    pmc_disable_periph_clk(43);  // CAN controller 0
    pmc_disable_periph_clk(44);  // CAN controller 1
}
// ================================================================================
void oldClockLowOff()
{
    // switch back to regular operating
    pmc_mck_set_prescaler(PMC_PCK_PRES_CLK_2);
    pmc_switch_mainck_to_xtal(PMC_OSC_XTAL);
    pmc_enable_udpck();  //switch on USB clock
    pmc_enable_periph_clk(3);   // real-time timer
    pmc_enable_periph_clk(4);   // watchdog timer

    pmc_enable_periph_clk(21);  // HSMCI (SD/MMC ctrl, N/C)
    pmc_enable_periph_clk(22);  // TWI/I2C bus 0 (i.MX6 controlling)
    pmc_enable_periph_clk(23);  // TWI/I2C bus 1                         für Augen und SD Card
    pmc_enable_periph_clk(24);  // SPI0                                  für Augen und SD Card
    pmc_enable_periph_clk(25);  // SPI1
    pmc_enable_periph_clk(26);  // SSC (I2S digital audio, N/C)

    pmc_enable_periph_clk(27);  // timer/counter 0
    pmc_enable_periph_clk(28);  // timer/counter 1
    pmc_enable_periph_clk(29);  // timer/counter 2
    pmc_enable_periph_clk(30);  // timer/counter 3
    pmc_enable_periph_clk(31);  // timer/counter 4
    pmc_enable_periph_clk(32);  // timer/counter 5
    pmc_enable_periph_clk(33);  // timer/counter 6
    pmc_enable_periph_clk(34);  // timer/counter 7
    pmc_enable_periph_clk(35);  // timer/counter 8
    pmc_enable_periph_clk(36);  // PWM

    pmc_enable_periph_clk(38);  // DAC ctrl
    pmc_enable_periph_clk(39);  // DMA ctrl
    pmc_enable_periph_clk(40);  // USB OTG high-speed ctrl
    pmc_enable_periph_clk(41);  // random number generator
    pmc_enable_periph_clk(42);  // ethernet MAC - N/C
    pmc_enable_periph_clk(43);  // CAN controller 0
    pmc_enable_periph_clk(44);  // CAN controller 1

}
// ================================================================================
void oldClockLow()
{
    static byte minuteOld = 0;
    byte hour, minute, second;
    int i;
    char file[3];

    RTC_GetTime(RTC, &hour, &minute, &second);

    if (minute == 0) {           // procedure to flip over from 59 to 0
        if (minuteOld > 59) {
            minuteOld = 0;
        }
    }
    if (minute > minuteOld) {                   // update only every minute
        // switch back to regular operating
        pmc_mck_set_prescaler(PMC_PCK_PRES_CLK_2);
        pmc_switch_mainck_to_xtal(PMC_OSC_XTAL);
        //pmc_enable_periph_clk(23);  // TWI/I2C bus 1                         für Augen und SD Card
        //pmc_enable_periph_clk(24);  // SPI0                                  für Augen und SD Card

        eye.select(eyeRight);
        eye.sd2Eye(itoa(hour, file, 10));
        eye.display();
        eye.select(eyeLeft);
        eye.sd2Eye(itoa(minute, file, 10));
        eye.display();
        minuteOld = minute;
        //pmc_disable_periph_clk(23);  // TWI/I2C bus 1                         für Augen und SD Card
        //pmc_disable_periph_clk(24);  // SPI0                                  für Augen und SD Card
        pmc_mck_set_prescaler(PMC_PCK_PRES_CLK_64); // master clock prescaler set to 64
        pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_4_MHz);
    }
}
// ================================================================================
// ================================================================================
