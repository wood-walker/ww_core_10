//=======================================================================================
void testSD() {
  eye.select(eyeRight);
  eye.setTextSize(2);
  eye.setTextColor(WHITE);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("SD-Card");
  eye.println("");
  if (digitalRead(SdDetect) == HIGH) {
    eye.println("No card");
    SdOK = 0;
  }
  else {
    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, SdCS)) {
      eye.println("no");  // no SD Card function
      eye.print("function");
      SdOK = 0;
    }
    else {                         // SD Card function OK!
      eye.println("Type:");   // print the type of card
      switch (card.type()) {
        case SD_CARD_TYPE_SD1:
          eye.print("   SD 1");
          SdOK = 1;
          break;
        case SD_CARD_TYPE_SD2:
          eye.print("   SD 2");
          SdOK = 1;
          break;
        case SD_CARD_TYPE_SDHC:
          eye.print(" SDHC");
          SdOK = 1;
          break;
        default:
          eye.print("Unknown");
      }
    }
  }
  eye.display();
}
