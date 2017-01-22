
void micOpen() {
  pinModeFrank(MiEnable, OUTPUT );
  digitalWriteFrank(MiEnable, HIGH);
}
//=======================================================================================
void micClose() {
  pinModeFrank(MiEnable, INPUT); // saves Energy
}
//=======================================================================================
void micClapAnalog() {
  /****************************************
  Example Sound Level Sketch for the
  Adafruit Microphone Amplifier

  Changes for use with the Wood-Walker by Rainer Radow 06/2015
  ****************************************/
#define  sampleWindow 50  // Sample window width in mS (50 mS = 20Hz)
  static uint8_t hour, minute, second;
  static double lastVolts;
  unsigned int sample;
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 4096;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MiInput);
    //SerialUSB.println(sample);
    if (sample < 4096) { // toss out spurious readings
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 3.3) / 4096;  // convert to volts

  eye.select(eyeLeft);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println(volts);
  eye.println("");
  eye.println(lastVolts);
  if (volts > 0.5) {
    lastVolts = volts;
    RTC_GetTime(RTC, &hour, &minute, &second);
    printTime();
  }
  if (hour < 10)   eye.print("0");
  eye.print(hour);
  eye.print(":");
  if (minute < 10)   eye.print("0");
  eye.print(minute);
  eye.print(":");
  if (second < 10)   eye.print("0");
  eye.println(second);
  eye.display();
}

void micClapInterrupt() {
  double volts;

  eye.select(eyeLeft);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println(volts);
  eye.println("");
  eye.println("");
  eye.display();
}

