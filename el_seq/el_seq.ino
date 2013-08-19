/* EL AcroYoga Suit
 * Build this for Lilypad 328 to use it with the old Sparkfun EL Sequencer
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
#define LEFT_SHOULDER A2
#define RIGHT_SHOULDER A3
#define LEFT_HIP A4
#define RIGHT_HIP A5
#define LEFT_FOOT A6
#define RIGHT_FOOT A7


int el_pins[] = {EL_A, EL_B, EL_C, EL_D, EL_E, EL_F, EL_G, EL_H};
int el_count = 8;

int sensor_pins[] = {LEFT_SHOULDER,
                     RIGHT_SHOULDER,
                     LEFT_HIP,
                     RIGHT_HIP,
                     LEFT_FOOT,
                     RIGHT_FOOT};
int sensor_count = 6;

void setup() {
  // intialize EL wire pins
  for (int i = 0; i < el_count; ++i) {
    pinMode(el_pins[i], OUTPUT);
    digitalWrite(el_pins[i], LOW);
    
    // initialize sensor pins too
    if (i < sensor_count) {
      pinMode(sensor_pins[i], INPUT);
      digitalWrite(sensor_pins[i], HIGH); // use the internal pull-up resistor
    }
  }


}


int lastHighBit = 0;
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
      digitalWrite(el_pins[highBit], HIGH);
      digitalWrite(el_pins[lastHighBit], LOW);
      lastHighBit = highBit;
    }
    
    lastTime = now;
  }
}

// highest order bit function from S.O.
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
