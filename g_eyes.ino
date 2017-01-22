// eye functions
//=======================================================================================
void initializeEyes() {
  delay(500);         // after a power on you have to wait a while to let the eyes start
  eye.select(eyeLeft);
  eye.stopscroll();
  eye.begin();
  eye.display();      // show an empty screen
  eye.select(eyeRight);
  eye.stopscroll();
  eye.begin();
  eye.display();      // show an empty screen
}

//=======================================================================================
void drawEye() {
  eye.clearBuffer(); // clean the splash screen buffer
  eye.fillCircle(eye.width() / 2, eye.height() / 2, 10, WHITE);
  for (int16_t i = 25; i < 30; i++) {
    eye.drawCircle(eye.width() / 2, eye.height() / 2, i, WHITE);
    eye.display();
  }
  eye.select(eyeRight);
  eye.display();
  eye.select(eyeLeft);
  eye.display();
}
//=======================================================================================
void scrollRight() {
  eye.select(eyeRight);
  eye.startscrollright(0x00, 0x07);
  eye.select(eyeLeft);
  eye.startscrollright(0x00, 0x07);
}

void scrollLeft() {
  eye.select(eyeRight);
  eye.startscrollleft(0x00, 0x07);
  eye.select(eyeLeft);
  eye.startscrollleft(0x00, 0x07);
}

void scrollStop() {
  eye.select(eyeLeft);
  eye.stopscroll();
  eye.select(eyeRight);
  eye.stopscroll();
}


//=======================================================================================
void displayIcons() {
  displayEye("ak01r.txt");
  displayEye("ak02r.txt");
  displayEye("ak03r.txt");
  displayEye("ak04r.txt");
  displayEye("ak05r.txt");
  displayEye("ak06r.txt");
  displayEye("ak07r.txt");

  displayEye("am01r.txt");
  displayEye("am02r.txt");
  displayEye("am03r.txt");

  displayEye("aw01r.txt");
  displayEye("aw02r.txt");
  displayEye("aw03r.txt");
  displayEye("aw04r.txt");
  displayEye("aw05r.txt");
  displayEye("aw06r.txt");
  displayEye("aw07r.txt");

  displayEye("az11.txt");
  displayEye("az12.txt");
  displayEye("az13.txt");
  displayEye("az14.txt");
}
//=======================================================================================
void displayEye(char *filename) {
  eye.select(eyeRight);
  eye.clearBuffer();
  eye.setCursor(0, 20);
  eye.print(filename);
  eye.display();
  eye.select(eyeLeft);
  eye.sd2Eye(filename); //we do not have to clear the buffer because we will fill all pixle
  eye.display();
  delay(500);
}

//=======================================================================================
void blankEye(int eyeThis) {
  eye.select(eyeThis);
  eye.dim(false);
  eye.stopscroll();
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.display();
}




