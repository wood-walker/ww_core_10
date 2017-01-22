// ****************************************************************************
//      - - - /   n_neopixel.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
// Slightly different, this makes the rainbow equally distributed throughout
void neoRainbow(uint8_t wait)
{
    static uint16_t j;
    uint16_t i;

    j++;
    if (j > 255) j = 0;
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
}
//=======================================================================================
void neoNight(uint8_t wait)
{
    static uint16_t j, value = 0;
    uint16_t i;

    j++;
    if (j < 80) value++;
    else value--;
    if (j > 160) {
        j = 0;
        value = 10;
    }
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, value , 0, 0);
    }
    strip.show();
    delay(wait);
}
//=======================================================================================
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}
//=======================================================================================
void neoRandom()
{
    int rm = 50; // random max

    //strip.setPixelColor(n, red, green, blue);
    strip.setPixelColor(0, random(rm), random(rm), random(rm));
    strip.setPixelColor(1, random(rm), random(rm), random(rm));
    strip.setPixelColor(2, random(rm), random(rm), random(rm));
    strip.show();   // push the data to the pixels
}
//=======================================================================================
void neoOff()
{
    strip.begin();
    strip.show();   // activate the begin()
    strip.clear();  // switch all pixels off
    strip.show();   // activate the switch off
}
//=======================================================================================
//=======================================================================================