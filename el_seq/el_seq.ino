/* EL AcroYoga Suit
 * Build this for Lilypad 328 to use it with the old Sparkfun EL Sequencer
 * (Which is what we're doing...)
 *
 * by Leslie Bienenfeld and Kevin Nelson 
 */
 
#define DEBUG

// oh arduino... there must be a better way.
// EL Driver labels and their corresponding digital pins
#define EL_A 2
#define EL_B 3
#define EL_C 4
#define EL_D 5
#define EL_E 6
#define EL_F 7
#define EL_G 8
#define EL_H 9

// ADC mapping, in case it needs to change later
#define ADC_LEFT_SHOULDER A2
#define ADC_RIGHT_SHOULDER A3
#define ADC_LEFT_HIP A4
#define ADC_RIGHT_HIP A5
#define ADC_LEFT_FOOT A6
#define ADC_RIGHT_FOOT A7

// you might also want to do EL mapping, i.e.:
// #define EL_LEFT_SHOULDER EL_D



// Array-ize the pins, so you can use for loops
int ELPins[] = {EL_A, EL_B, EL_C, EL_D, EL_E, EL_F, EL_G, EL_H};
int ELPinCount = 8;

int SensorPins[] = {ADC_LEFT_SHOULDER,
                    ADC_RIGHT_SHOULDER,
                    ADC_LEFT_HIP,
                    ADC_RIGHT_HIP,
                    ADC_LEFT_FOOT,
                    ADC_RIGHT_FOOT};
int SensorPinCount = 6;




void setup() {
  int i;
  // intialize EL wire pins
  for (i = 0; i < ELPinCount; ++i) {
    pinMode(ELPins[i], OUTPUT);
    digitalWrite(ELPins[i], LOW);
  }

  // initialize sensor pins
  for (i = 0; i < SensorPinCount; ++i) {
    pinMode(SensorPins[i], INPUT);
    digitalWrite(SensorPins[i], HIGH); // use the internal pull-up resistor
  }
}


byte lastHighBit = 0;
unsigned long lastTime = 0;
void loop() {
  unsigned long now = millis();
  if (now - lastTime > 300) {
    // 300 ms refresh rate

    int pressure = 1024 - analogRead(A7);
    
    // chop the last 2 bits
    byte scaledPressure = pressure >> 2;
    
    byte highBit = highest_order_bit(scaledPressure);

    if (highBit != lastHighBit) {
      digitalWrite(ELPins[highBit], HIGH);
      digitalWrite(ELPins[lastHighBit], LOW);
      lastHighBit = highBit;
    }
    
    
    lastTime = now;
  }
}

// highest order bit function stolen from stack overflow.
byte highest_order_bit(byte input) {
  if (!input) {
    return 0;
  }
 
  byte highest = 1;
  while (input >>= 1) {
    highest <<= 1;
  }
 
  return highest;
}
