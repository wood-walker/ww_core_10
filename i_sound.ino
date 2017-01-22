// ****************************************************************************
//      - - - /   i_sound.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
//==================================================================================
void playFile (char* fileName, int action)
{
    const int S = 2048;     // Number of samples to read in block 1024 or 2028
    const int T = 200;      // mSec of pre buffering.              100 or  200

    short buffer[S];        // wav data buffer
    int returnValue = 0;    // = 0 when the DAC buffer was filled again
    int loadCount = 0;      // counts the fill events to prevent the start "blop"
    int volume = 50;

    if (action > 1) {
        speedTarget = 200;
        speedNow = 100;
        moveflag = true;
        speedAcceleration = 50;
        feetMovement(feetGoForward); // go forward
    }
    if (SdOK == 1) {  // Nur spielen, wenn die SD OK ist - sonst gibt es Probleme
        SPI.setClockDivider(4);  // hi-speed SPI transfers
        Audio.begin(88200, T);
        File myFile = SD.open(fileName);     // open wave file from sdcard
        if (!myFile)
            return;
        digitalWrite(SpEnable, HIGH);  // switch on amplifier
        if (action > 0)
            volumePrepare(volume);
        while (myFile.available()) {
            myFile.read(buffer, sizeof(buffer));  // read from the file into buffer
            Audio.prepare(buffer, S, volume);     // Prepare samples
            returnValue = Audio.write(buffer, S);               // Feed samples to audio
            if (action > 0) {
                if (returnValue == 0) {
                    volume = volumeControl(volume);
                    if (volume == -1) break;
                }
            }
        }
        myFile.close();
        Audio.end();
        digitalWrite(SpEnable, LOW); // switch off amplifier
        if (action > 0) {
            blankEye(eyeLeft);           // clear the eye Display
            if (action > 1) {
                speedTarget = 0;
                moveflag = false;
                feetMovement(feetGoForward); // go forward
            }
            delay(500);                  //to give you some time to release the feeler
        }
    }
}
//=======================================================================================
void volumePrepare(int firstVolume)      // we have to do as many operations as possible outside the wav play loop
{
    eye.setTextSize(2);
    eye.setTextColor(WHITE, BLACK);

    eye.clearBuffer(); // clean the splash screen buffer
    eye.setCursor(0, 0);
    eye.print("Volume");
    eye.setCursor(0, 50);
    eye.print("-  STOP  +");
    volumeDisplay(firstVolume);
}
//=======================================================================================
int volumeControl(int volume)
{
    const int increment = 20;

    if (digitalRead(MFRight) == 0) {
        volume += increment;
        if (volume > 600) volume = 600;
        volumeDisplay(volume);
    }
    if (digitalRead(MFLeft) == 0) {
        volume -= increment;
        if (volume < 0) volume = 0;
        volumeDisplay(volume);
    }

    if (digitalRead(MFRight) == 0 && digitalRead(MFLeft) == 0)
        volume = -1;

    return volume;
}
//=======================================================================================
void volumeDisplay(int displayvolume)
{
    eye.setCursor(0, 20);
    eye.print("   ");
    eye.setCursor(0, 20);
    eye.print(displayvolume);

    eye.select(eyeLeft);
    eye.display();
}
//=======================================================================================
//==================================================================================