void BTPio10() {
  eye.select(eyeLeft);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("AT+PIO10");
  while (serialBT.available()) {    // empty the buffer
    serialBT.read();
  }  
  serialBT.print("AT+PIO10");
  delay(200);                       // not necessary -only for the visual effect ;-)
  while (serialBT.available()) {
    eye.print(char(serialBT.read()));
  }
  eye.display();
}

void BTPio11() {
  eye.select(eyeLeft);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("AT+PIO11");
  while (serialBT.available()) {    // empty the buffer
    serialBT.read();
  }  
  serialBT.print("AT+PIO11");
  delay(200);                       // not necessary -only for the visual effect ;-)
  while (serialBT.available()) {
    eye.print(char(serialBT.read()));
  }
  eye.display();
}

void BTInfo() {
  int i = 0;
  char BTAnswer;
  eye.select(eyeLeft);
  eye.clearBuffer();
  eye.setCursor(0, 0);
  eye.println("AT+NAME?12");
  while (serialBT.available()) {  // empty the buffer
    serialBT.read();
  }
  serialBT.print("AT+NAME?");
  delay(100);
  while (serialBT.available()) {
    BTAnswer = serialBT.read();
    i++;
    if ( i > 7) { // Skip the "OK+Get:" beginning of the answer
      eye.print(BTAnswer);
    }
  }
  i = 0;
  eye.println("\nAT+NAMB?");
    while (serialBT.available()) {   // empty the buffer
    serialBT.read();
  }
  serialBT.print("AT+NAMB?");
  delay(100);
  while (serialBT.available()) {
    BTAnswer = serialBT.read();
    i++;
    if ( i > 7) { // Skip the "OK+Get:" beginning of the answer
      eye.print(BTAnswer);
    }
  }
  eye.display();
}
