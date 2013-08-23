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
#define ADC_LEFT_SHOULDER     A2
#define ADC_RIGHT_SHOULDER    A3
#define ADC_LEFT_HIP          A4
#define ADC_RIGHT_HIP         A5
#define ADC_LEFT_FOOT         A6
#define ADC_RIGHT_FOOT        A7

// ADC Thresholding
#define LEFT_SHOULDER_THRESHOLD   600
#define RIGHT_SHOULDER_THRESHOLD  600 
#define LEFT_HIP_THRESHOLD        600
#define RIGHT_HIP_THRESHOLD       600
#define LEFT_FOOT_THRESHOLD       600
#define RIGHT_FOOT_THRESHOLD      600

// take advantage of the consecutive analog pins to turn the sensor
// mapping into consecutive array indeces starting at 0
#define Sensor_Table_Index(sensor) ((sensor) - A2)

// you might also want to do EL mapping, i.e.:
// #define EL_LEFT_SHOULDER EL_D



// Array-ize the pins, so you can use for loops
#define ELPinCount (8)
int ELPins[] = {EL_A, EL_B, EL_C, EL_D, EL_E, EL_F, EL_G, EL_H};

#define SensorPinCount (6)
int SensorPins[] = {ADC_LEFT_SHOULDER,
                    ADC_RIGHT_SHOULDER,
                    ADC_LEFT_HIP,
                    ADC_RIGHT_HIP,
                    ADC_LEFT_FOOT,
                    ADC_RIGHT_FOOT};

int sensorTable[6];

// simple helper method, just cause it's less typing.
// usage: SensorThreshold(ADC_LEFT_SHOULDER);
int SensorThreshold(int sensorPin) {
  return sensorTable[Sensor_Table_Index(sensorPin)];
}


// arduino !


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

  // initialize the sensor threshold table.  you shouldn't need to change this
  // change the defines to change the thresholds.
  // hysteresis isn't supported in this version, sorry champ.
  sensorTable[Sensor_Table_Index(ADC_LEFT_SHOULDER)] = LEFT_SHOULDER_THRESHOLD;
  sensorTable[Sensor_Table_Index(ADC_RIGHT_SHOULDER)] = RIGHT_SHOULDER_THRESHOLD;
  sensorTable[Sensor_Table_Index(ADC_LEFT_HIP)] = LEFT_HIP_THRESHOLD;
  sensorTable[Sensor_Table_Index(ADC_RIGHT_HIP)] = RIGHT_HIP_THRESHOLD;
  sensorTable[Sensor_Table_Index(ADC_LEFT_FOOT)] = LEFT_FOOT_THRESHOLD;
  sensorTable[Sensor_Table_Index(ADC_RIGHT_FOOT)] = RIGHT_FOOT_THRESHOLD;

#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect, just in case?
  }
#endif
}




// loop related things


int lastAnalog[SensorPinCount];
int analogValues[SensorPinCount];

unsigned long lastNow = 0;
void loop() {
  unsigned long now = millis();
  int refreshRate = 100; //ms
  if (now - lastNow > refreshRate) {
    int i;

    // read from sensors
    for (i = 0; i < SensorPinCount; ++i ) {
      analogValues[i] = analogRead(SensorPins[i]);

      // for now, testing, just map SensorPins[i] to ELPins[i]
      if (analogValues[i] < SensorThreshold(SensorPins[i])) {
        // threshold crossed, turn on the light
        digitalWrite(ELPins[i], HIGH);
      } else {
        digitalWrite(ELPins[i], LOW);
      }
    }

    // get ready for next round
    lastNow = now;
    for (i = 0; i < SensorPinCount; ++i) {
      lastAnalog[i] = analogValues[i];
#ifdef DEBUG
      String sensorOutput = "Sensor ";
      sensorOutput = sensorOutput + i + " Value: " + analogValues[i] + "\n";
      Serial.print(sensorOutput);
#endif
    }
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
