#pragma once

#define WORD_LEDS_PIN 21
#define TOP_LEFT_LED_PIN 19
#define TOP_RIGHT_LED_PIN 18
#define BOTTOM_LEFT_LED_PIN 17
#define BOTTOM_RIGHT_LED_PIN 16

CRGB _leds[130];

void setAllLEDs(CRGB c, CRGB* strip, uint16_t numLeds) {
    for (uint16_t i = 0; i < numLeds; ++i) {
        strip[i] = c;
    }
}

void setAllLEDs(CHSV c, CRGB* strip, uint16_t numLeds) {
    for (uint16_t i = 0; i < numLeds; ++i) {
        strip[i] = c;
    }
}

void it(CRGB c = CRGB::White) {
    _leds[0] = c;
    _leds[1] = c;
}

void is(CRGB c = CRGB::White) {
    _leds[3] = c;
    _leds[4] = c;
}

void tenM(CRGB c = CRGB::White) {
    _leds[6] = c;
    _leds[7] = c;
    _leds[8] = c;
}

void half(CRGB c = CRGB::White) {
    _leds[9] = c;
    _leds[10] = c;
    _leds[11] = c;
    _leds[12] = c;
}

void quarter(CRGB c = CRGB::White) {
    _leds[13] = c;
    _leds[14] = c;
    _leds[15] = c;
    _leds[16] = c;
    _leds[17] = c;
    _leds[18] = c;
    _leds[19] = c;
}

void twenty(CRGB c = CRGB::White) {
    _leds[20] = c;
    _leds[21] = c;
    _leds[22] = c;
    _leds[23] = c;
    _leds[24] = c;
    _leds[25] = c;
}

void fiveM(CRGB c = CRGB::White) {
    _leds[26] = c;
    _leds[27] = c;
    _leds[28] = c;
    _leds[29] = c;
}

void minutes(CRGB c = CRGB::White) {
    _leds[31] = c;
    _leds[32] = c;
    _leds[33] = c;
    _leds[34] = c;
    _leds[35] = c;
    _leds[36] = c;
    _leds[37] = c;
}

void past(CRGB c = CRGB::White) {
    _leds[46] = c;
    _leds[47] = c;
    _leds[48] = c;
    _leds[49] = c;
}

void to(CRGB c = CRGB::White) {
    _leds[50] = c;
    _leds[51] = c;
}

void three(CRGB c = CRGB::White) {
    _leds[60] = c;
    _leds[61] = c;
    _leds[62] = c;
    _leds[63] = c;
    _leds[64] = c;
}

void eleven(CRGB c = CRGB::White) {
    _leds[65] = c;
    _leds[66] = c;
    _leds[67] = c;
    _leds[68] = c;
    _leds[69] = c;
    _leds[70] = c;
}

void four(CRGB c = CRGB::White) {
    _leds[71] = c;
    _leds[72] = c;
    _leds[73] = c;
    _leds[74] = c;
}

void one(CRGB c = CRGB::White) {
    _leds[75] = c;
    _leds[76] = c;
    _leds[77] = c;
}

void two(CRGB c = CRGB::White) {
    _leds[78] = c;
    _leds[79] = c;
    _leds[80] = c;
}

void eight(CRGB c = CRGB::White) {
    _leds[86] = c;
    _leds[87] = c;
    _leds[88] = c;
    _leds[89] = c;
    _leds[90] = c;
}

void nine(CRGB c = CRGB::White) {
    _leds[91] = c;
    _leds[92] = c;
    _leds[93] = c;
    _leds[94] = c;
}

void seven(CRGB c = CRGB::White) {
    _leds[95] = c;
    _leds[96] = c;
    _leds[97] = c;
    _leds[98] = c;
    _leds[99] = c;
}

void five(CRGB c = CRGB::White) {
    _leds[100] = c;
    _leds[101] = c;
    _leds[102] = c;
    _leds[103] = c;
}

void six(CRGB c = CRGB::White) {
    _leds[104] = c;
    _leds[105] = c;
    _leds[106] = c;
}

void ten(CRGB c = CRGB::White) {
    _leds[114] = c;
    _leds[115] = c;
    _leds[116] = c;
}

void twelve(CRGB c = CRGB::White) {
    _leds[117] = c;
    _leds[118] = c;
    _leds[119] = c;
    _leds[120] = c;
    _leds[121] = c;
    _leds[122] = c;
}

void oclock(CRGB c = CRGB::White) {
    _leds[124] = c;
    _leds[125] = c;
    _leds[126] = c;
    _leds[127] = c;
    _leds[128] = c;
    _leds[129] = c;
}

void minute1(bool state=true) {
  digitalWrite(TOP_LEFT_LED_PIN, state);
}

void minute2() {
  digitalWrite(TOP_RIGHT_LED_PIN, HIGH);
}

void minute3() {
  digitalWrite(BOTTOM_LEFT_LED_PIN, HIGH);
}

void minute4() {
  digitalWrite(BOTTOM_RIGHT_LED_PIN, HIGH);
}

void clearClockFace() {
  setAllLEDs(CRGB::Black, _leds, 130);
  digitalWrite(TOP_LEFT_LED_PIN, LOW);
  digitalWrite(TOP_RIGHT_LED_PIN, LOW);
  digitalWrite(BOTTOM_LEFT_LED_PIN, LOW);
  digitalWrite(BOTTOM_RIGHT_LED_PIN, LOW);
}

uint16_t XY(uint8_t x, uint8_t y) {
  auto ret = x+(y*13);
  //Serial.print("x: "); Serial.print(x); Serial.print(" - y: "); Serial.print(y); Serial.print(" => XY: "); Serial.println(ret);
  return ret;
}
