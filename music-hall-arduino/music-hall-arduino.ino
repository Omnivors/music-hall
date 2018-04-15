/*
 *  MusicHall - arduino side
 *  
 *  created Apr 2018
 *  by Francesco Cretti and Luca Morino
 *  in Turin, Italy
 * 
 */

/******* GLOBAL VARIABLES *********/
// SONAR ANALOG PINS
const int sensorPin[3] = {0, 1, 2};
// SONAR READINGS
unsigned int rangeCm[3];
// SMOOTHING METHOD - exponentially decaying moving average - equivalent window = 10
const float tiny=1-(1/10.0); 

/******* SETUP *********/
void setup () {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

/******* LOOP *********/
void loop () {
  readSensor(0);
  sendData();
  delay(100);
}

void readSensor (int sensor) {
  unsigned int analog = analogRead(sensorPin[sensor]);
  if (analog < 1024) {
    // TODO see if float is necessary for our needed resolution
    float volt = analog * (5.0 / 1023.0);
    unsigned int cm = volt / 0.005 * 2.0;
    // exp smooth
    rangeCm[sensor] = tiny * rangeCm[sensor] + (1 - tiny) * cm;
    }  
}

void sendData () {
  // Send sensor 1 data
  uint8_t LSB = rangeCm[0];
  uint8_t MSB = rangeCm[0] >> 8;
  Serial.write(MSB);
  Serial.write(LSB);
  //Serial.println(rangeCm[0]);
  // Serial.write(rangeCm[0]);
}
