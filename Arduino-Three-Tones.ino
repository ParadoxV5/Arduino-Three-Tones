/* Array order: 245Hz (`tone()`), 490Hz (PWM), 980Hz (PWM)
  Specific for 490Hz and 980Hz, see which pins are available at
  https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/.
  According to that page, Uno, Nano, Micro, Mini, Mega, Yún,
  Leonardo and 101 are the boards that PWM at 490/980Hz.
  It also says that, except for Mega,
  `tone()` interferes with the PWM on pins 3 and 11. Avoid using those two.
*/
const byte
  PINS_OUT[] = {12,  9, 6},
  PINS_IN [] = {13, 10, 7};
/*
 * 0 = released & muted
 * 1 =  pressed & muted
 * 2 = released & playing
 * 3 =  pressed & playing
 */
byte state[] = {0, 0, 0};

void setup() {
  for(byte pin: PINS_OUT)
    pinMode(pin, OUTPUT);
  for(byte pin: PINS_IN)
    pinMode(pin, INPUT);
}

void loop() {
  for(byte i = 0; i < 3; ++i) {
    if(digitalRead(PINS_IN[i]) == LOW)
      state[i] &= ~1; // Clear pressed state
    // HIGH: Toggle on rising-edge
    else if(!(state[i] & 1)) { // Not already holding
      if(state[i] >> 1) { // Currently playing
        if(i) // PWM
          digitalWrite(PINS_OUT[i], LOW);
        else // 245Hz
          noTone(PINS_OUT[0]);
      } else { // Currently muted
        if(i) // PWM
          analogWrite(PINS_OUT[i], /*floor(255/2)*/ 127);
        else // 245Hz
          tone(PINS_OUT[0], 245);
      }
      state[i] = (state[i] | 1) ^ 2; // Set to pressed and invert playing state
    }
  }
}