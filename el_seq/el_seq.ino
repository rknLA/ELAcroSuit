/* EL AcroYoga Suit
 * Build this for Lilypad 328 to use it with the old Sparkfun EL Sequencer
 *
 * by Leslie Bienenfeld and Kevin Nelson 
 */


int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int elPin = 2;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(elPin, OUTPUT);
  digitalWrite(elPin, HIGH);

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
 /* 
  // turn the ledPin on
  delay(250);
  Serial.print("Sensor value: ");
  Serial.print(sensorValue);
  Serial.print("\n");
  
  if (sensorValue < 400) {
    digitalWrite(elPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(elPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  */
}
