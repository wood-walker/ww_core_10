// ****************************************************************************
//      - - - /    c_loop.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
// loop() function only

unsigned long currentMillis = millis();
unsigned long lastMillis;


int neoRandomWait = 0;
int onlyonce = 0;

//==================================================================================
void loop()
{
    esp.Process();  // you have to run this frequently to get callbacks from the ESP!
// ---------------------------------------------------------------------------------------
    switch (activeAction) {
    case mFeelerListen: //        ==== just the menu without any action
        feelerListen();
        buttonPressed = 0;
        break;
    case mMotorTest:  //          ==== Walker ====
        motorTest(); // will perform the Motor test without the ability to any interaction
        menuShow();
        activeAction = mFeelerListen;
        break;
    case mBump:
        bumpnRunPrepare();
        activeAction = mBumpLoop;
        break;
    case mBumpLoop:
        bumpLoop();     // The feeler can not be used as activation for the menue again... You have to reset the W-W
        break;
    case mRTCSetTime: //           ==== RTC ====
        setTime();
        activeAction = mRTCDisplayTime;
        break;
    case mRTCEspSync: //           ==== RTC ====
        if(timeEspSync()>0)   // >0: no sync beacuse of technical problems 
            delay(3000);
        activeAction = mRTCDisplayTime;
        break;
    case mRTCDisplayTime:
        displayTime();
        feelerListen();
        break;
    case mOldClock:  // --------------------------------------------------
        buttonPressed = 0;
        activeAction = mOldClockLoop;
        break;
    case mOldClockLoop:
        oldClock();
        feelerListen();
        if (buttonPressed > 0) { // we are leaving this wonderfull action
            activeAction = mFeelerListen;
            buttonPressed = 0;
        }
        break;
    case mOldClockLow:  // --------------------------------------------------
        buttonPressed = 0;
        oldClockLowPrepare();
        activeAction = mOldClockLowLoop;
        break;
    case mOldClockLowLoop:
        oldClockLow();
        feelerListen();
        if (buttonPressed > 0) { // we are leaving this wonderfull action
            activeAction = mFeelerListen;
            buttonPressed = 0;
        }
        break;
    case mSleepMode:
        //attachInterrupt(VibSwitch, knockDetect, FALLING);

        /*
        attachInterrupt(VibSwitch, wakeISR, FALLING);


        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        //pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_4_MHz);

        //pmc_enable_sleepmode(0);
        __WFI();



        //pmc_enable_waitmode();
        //

        detachInterrupt(VibSwitch);
        */
        feelerListen();
        break;
    case mEyeBright: //             ==== Display ====
        eye.dim(false);
        eye.select(eyeLeft);
        eye.dim(false);
        eye.select(eyeRight);
        activeAction = mFeelerListen;
        break;
    case mEyeDim:
        eye.dim(true);
        eye.select(eyeLeft);
        eye.dim(true);
        eye.select(eyeRight);
        activeAction = mFeelerListen;
        break;
    case mEyeRScroll:
        scrollRight();
        activeAction = mFeelerListen;
        break;
    case mEyeLScroll:
        scrollLeft();
        activeAction = mFeelerListen;
        break;
    case mEyeNoScroll:
        scrollStop();
        activeAction = mFeelerListen;
        break;
    case mEyeCircle:
        drawEye();
        activeAction = mEyeRScroll;
        break;
    case mEyeIcon:
        displayIcons();
        menuShow();
        activeAction = mFeelerListen;
        break;
    case mMonEnergy: //              ==== Monitor ====
        monEnergy();
        feelerListen();
        break;
    case  mMonIR:
        monIR();
        feelerListen();
        break;
    case  mMonLight:
        monLight();
        feelerListen();
        break;
    case  mMonKnock:
        attachInterrupt(VibSwitch, knockDetect, FALLING);
        activeAction = mMonKnockLoop;
        break;
    case  mMonKnockLoop:
        monKnock();
        feelerListen();
        if (activeAction != mMonKnockLoop) { // we are leaving this wonderfull action
            detachInterrupt(VibSwitch);
        }
        break;
    case  mMonTemp:
        monTempOn();
        activeAction = mMonTempLoop;
        break;
    case  mMonTempLoop:
        monTemp();
        feelerListen();
        if (activeAction != mMonTempLoop) { // we are leaving this wonderfull action
            monTempOff();
        }
        break;
    case  mMonEncoder:
        monEncoderPrepare();
        activeAction = mMonEncLoop;
        break;
    case  mMonEncLoop:
        monEncoderLoop();
        feelerListen();
        if (activeAction != mMonEncLoop) { // we are leaving this wonderfull action
            monEncoderOff();
        }
        break;

    case  mMonGiro:
        monGiroPrepare();
        activeAction = mMonGiroLoop;
        lastMillis = millis() - 4000;
        break;
    case  mMonGiroLoop:
        if (millis() > lastMillis + 3000) {
            monGiroLoop();
        }
        feelerListen();
        if (activeAction != mMonEncLoop) { // we are leaving this wonderfull action
            monGiroOff();
        }
        break;
    case mEspMqtt:

        EspPrepareMqtt();


        //activeAction = mEspMqttLoop;

        activeAction = mFeelerListen;


        break;
    case mEspMqttLoop:
        esp.Process();
        feelerListen();

        break;
    case mNeoRainbow: //Rainbow      ==== NeoPixle ====
        feelerListen();
        neoRainbow(20);
        break;
    case mNeoNight: //Rainbow
        drawEye();
        activeAction = mNeoNightLoop;
        break;
    case mNeoNightLoop: //Rainbow
        neoNight(30);
        feelerListen();
        break;
    case mNeoRandom: //neo random
        feelerListen();
        neoRandomWait++;
        delay(10);
        if (neoRandomWait > 100) {
            neoRandom();
            neoRandomWait = 0;
        }
        break;
    case mNeoOff: //Off
        neoOff();
        feelerListen();
        break;
    case mPlay1: //                 ==== Sound ====
        playFile("walle01.wav", 2);
        activeAction = mFeelerListen;
        break;
    case mMicAClap:
        micOpen();
        activeAction = mMicAClapLoop;
        break;
    case mMicAClapLoop:
        micClapAnalog();
        feelerListen();
        if (activeAction != mMicAClapLoop) { // we are leaving this wonderfull action
            micClose();
        }
        break;
    case mBTPio10: //               ==== Bluetooth ====
        BTPio10();
        activeAction = mFeelerListen;
        break;
    case mBTPio11:
        BTPio11();
        activeAction = mFeelerListen;
        break;
    case mBTInfo:
        BTInfo();
        activeAction = mFeelerListen;
        break;
    case mESPOn: //                ==== Wlan ====
        espOn();
        activeAction = mFeelerListen;
        break;
    case mESPProg:
        espProg();
        activeAction = mFeelerListen;
        break;
    case mESPOff:
        espOff(1);
        activeAction = mFeelerListen;
        break;
    case mESPInfo:
        //espInfo();
        activeAction = mFeelerListen;
        break;
    case mOffYes:
        offYes();
        break;
    default: //                     ==== default ====
        feelerListen();
        buttonPressed = 0;
        break;
    }
}
//==================================================================================
//==================================================================================