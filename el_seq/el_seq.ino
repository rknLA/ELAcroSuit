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


int el_pins[] = {EL_A, EL_B, EL_C, EL_D, EL_F, EL_G, EL_H};
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
    
    // initialize sensor pins too
    if (i < sensor_count) {
      pinMode(sensor_pins[i], INPUT);
      digitalWrite(sensor_pins[i], HIGH); // use the internal pull-up resistor
    }
  }

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

// for now, just display some el blinkies.
byte lights = 0;
byte stage = 0;
unsigned long change_time = 0;
int last_highest = -1;
void loop() {
  /*
  unsigned long now = millis();
  if (now - change_time > 250) {
    stage = (stage + 1); //% el_count;

    write_lights(stage);
    change_time = now;
  }
  */
  
  int analogValue = analogRead(A7);
  // find highest order bit
  int highest = -1;
  for (int i = 0; i < 8; ++i) {
    if ((analogValue >> i) & 1) {
      highest = i;
    }
  }
  // compensate for 10 bit ADC
  highest -= 2;
  if (highest > 0) {
    if (highest != last_highest) {
      digitalWrite(el_pins[highest], HIGH);
      digitalWrite(el_pins[last_highest], LOW);
      last_highest = highest;
    }
  }      
  
  Serial.print("Sensor value: ");
  Serial.print(analogValue);
  Serial.print("\n");
}

void write_lights(byte light_mask) {
  // loop through the byte to write pins.. super inefficient. whatever
  for (int i = 0; i < el_count; ++i) {
    boolean state = light_mask & (1 << i);
    digitalWrite(el_pins[i], state);
  }
}
