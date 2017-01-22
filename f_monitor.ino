// ****************************************************************************
//      - - - /   f_monitor.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
void monEnergy()
{
    int reading, current;
    float voltage;

    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);

    reading = analogRead(RawVcc); // RAW Bat voltage
    voltage = float(reading) / 985;
    eye.print(voltage);      // should be something between 3.3 and 4.2
    eye.println(" RAW");

    /*
      reading = analogRead(SysVcc); // System voltage
      voltage = float(reading) / 990;
      eye.print(voltage);
      eye.println(" Syst");      // should be 3.2 to 3.3
    */

    reading = 0;
    for (int i = 1; i < 301; i++) {
        reading += analogRead(SysCur);
        delay(1);
    }
    reading /= 300;

    current = float(reading) * 0.158;
    eye.print(current);
    eye.print(" mA");
    eye.display();
}
//=======================================================================================
void monIR()
{
    int cornerFrontLeft, cornerFrontRight;

    pinModeFrank(IrEnLeft, OUTPUT);    // light sensor front enable
    pinModeFrank(IrEnRight, OUTPUT);
    digitalWriteFrank(IrEnLeft, HIGH);
    digitalWriteFrank(IrEnRight, HIGH);

    cornerFrontRight = analogRead(IrRight);
    cornerFrontLeft = analogRead(IrLeft);
    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Right Left");
    eye.println(cornerFrontRight);
    eye.print("      ");
    eye.print(cornerFrontLeft);
    eye.display();
    delay (100);
}
//=======================================================================================
void monLight()
{
    boolean static toggle = false;
    static int ambientLightOldFront;
    static int ambientLightOldBack;

    int ambientLightFront = 0;
    int ambientLightBack = 0;

    pinModeFrank(LiEnable, OUTPUT);    // light sensor front enable
    pinMode(LiBackEnable, OUTPUT);
    digitalWriteFrank(LiEnable, HIGH);
    digitalWrite(LiBackEnable, HIGH);

    for (int i = 1; i < 101; i++) {
        ambientLightFront += analogRead(LiFront);
        ambientLightBack += analogRead(LiBack);
        delay(2);
    }
    ambientLightFront /= 100;
    ambientLightBack /= 100;

    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    if (toggle == true) {
        eye.println("Li-feeler");
    } else {
        eye.println("Li feeler");
    }
    eye.println("     delta");
    eye.print(ambientLightBack);
    eye.setCursor(90, 32);
    eye.println(abs(ambientLightOldBack - ambientLightBack));

    eye.print(ambientLightFront);
    eye.setCursor(90, 48);
    eye.print(abs(ambientLightOldFront - ambientLightFront));

    eye.display();
    toggle = !toggle;
    ambientLightOldFront = ambientLightFront;
    ambientLightOldBack = ambientLightBack;
    delay(50);
}
//=======================================================================================
void monKnock()
{
    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Knockknock");

    if (knockKnock == 1) {
        knockKnock = 0;
        eye.println(" we got");
        eye.print(" a knock");
        eye.display();

        switch (random(5)) {
        case 1:
            playFile("frog3.wav", 0);
            break;

        case 2:
            playFile("frog4.wav", 0);
            break;

        case 3:
            playFile("frog5.wav0", 0);
            break;

        case 4:
            playFile("donttouc.wav", 0);
            break;

        case 5:
            playFile("frog6.wav", 0);
            break;
        }
        delay(500);
    } else {
        if (toggle == true) {
            eye.println("       .");
        }
        eye.display();
        toggle = !toggle;
    }
}
//=======================================================================================
// Interrupt routins *********************************************************************
void knockDetect()
{
    knockKnock = 1;
}
//=======================================================================================
void monTempOn()
{
    //switch on the temp sensor
    pinModeFrank(TeEnable, OUTPUT);
    digitalWriteFrank(TeEnable, HIGH);
}
void monTempOff()
{
    //switch on the temp sensor
    pinModeFrank(TeEnable, INPUT);
}
void monTemp()
{
    float temp;                         // original temperature reading
    char dataString[7];                 // temperature as string: (-xx)x.x
    temp = getAmbientTemperature();
    //dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
    // = standard avr-libc function
//  dtostrf(temp, 5, 1, dataString);

    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Temp.");
    eye.println("");
    eye.print(" ");
    eye.print(dataString);
    eye.println(" C");
    if (toggle == true) {
        eye.print("        .");
    }
    eye.display();
    toggle = !toggle;
}
// ============================================================================
double getAmbientTemperature()
{
    int8_t msb;
    int8_t lsb;
    int8_t msb1;
    Wire.beginTransmission(0x48);
    Wire.write(0);  //0 = Temp, 2 = lower limit, 3 = upper limit
    Wire.endTransmission();
    Wire.beginTransmission(0x48);
    Wire.requestFrom(0x48, 2);
    if (Wire.available()) {
        msb1 = Wire.read();
        msb = msb1 << 1; // Vorzeichenbit entfernen, verbliebener Wert ist nun doppelt so groß
        lsb = Wire.read();
    }
    // höchstes bit von lsb sagt aus, ob 0,5 Grad dazu addiert werden sollen
    lsb = (lsb & 0x80 ) >> 7; // nun ist lsb = 0 oder 1
    Wire.endTransmission();
    if (msb1 < 0x80) {                    // positive value?
        return double(msb + lsb) / 2;       // positive value
    } else {
        return double(msb + lsb) / 2 - 128; // negativer value
    }
}
//=======================================================================================
void monEncoderPrepare()
{
    digitalWrite(EnEnable, HIGH);
}
//=======================================================================================
void monEncoderLoop()
{
#ifdef v4
    int encoder_right = digitalReadFrank(EnRight); //1 = solid disk, 0 = encoder hole
    int encoder_left = digitalReadFrank(EnLeft);
#else
    int encoder_right = digitalRead(EnRight); //1 = solid disk, 0 = encoder hole
    int encoder_left = digitalRead(EnLeft);
#endif
    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Encoder");
    if (toggle == true) {
        eye.println("left");
    } else {
        eye.println("left    .");
    }
    eye.print(encoder_left);
    eye.display();

    eye.select(eyeRight);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Encoder");
    eye.println("right");
    eye.print(encoder_right);
    eye.display();

    toggle = !toggle;
}
//=======================================================================================
void monEncoderOff()
{
    pinMode(EnEnable, INPUT);
}
//=======================================================================================
//=======================================================================================