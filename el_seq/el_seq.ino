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
    digitalWrite(el_pins[i], LOW);
    
    // initialize sensor pins too
    if (i < sensor_count) {
      pinMode(sensor_pins[i], INPUT);
      digitalWrite(sensor_pins[i], HIGH); // use the internal pull-up resistor
    }
  }

/*
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  */
}

unsigned int adcMax = 1024;
unsigned int adcMin = 0;
unsigned int adcRange = 1;
int lastElLit = -1;
void loop() {
  // sensor is active low
  int sensorValue = 1023 - analogRead(A7);
  
  // maintain extrema
  adcMax = max(adcMax, sensorValue);
  adcMin = min(adcMin, sensorValue);
  adcRange = adcMax - adcMin;
  
  // in case Bad Things happen
  if (adcRange > 1024) {
    adcRange = 0;
  }
  
  
  // scale input according to extrema
  int scaledValue = (sensorValue - adcMin) / adcRange;
  
  // map scaped value from 0-7
  int valueAsElIndex = scaledValue / 8;
  if (valueAsElIndex != lastElLit) {
    digitalWrite(el_pins[valueAsElIndex], HIGH);
    digitalWrite(el_pins[lastElLit], LOW);
    lastElLit = valueAsElIndex;
  }      
  
  /*
  Serial.print("Sensor value: ");
  Serial.print(analogValue);
  Serial.print("\n");
  */
}
