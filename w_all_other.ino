// ****************************************************************************
//      - - - /   w_all_other.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
void offYes()
{
    eye.select(eyeLeft);
    eye.clearBuffer();
    eye.setCursor(0, 0);
    eye.println("Good By");
    eye.display();
    pinMode(ShutDown, OUTPUT);
    digitalWrite(ShutDown, LOW);

}
//==================================================================================

void wakeISR()
{
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
}
//==================================================================================
//==================================================================================