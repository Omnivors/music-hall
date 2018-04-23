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
// Trigger
const int triggerPin = 2;

/******* SETUP *********/
void setup () {
  Serial.begin(9600);
  pinMode(triggerPin,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(300);
}

/******* LOOP *********/
void loop () {
  start_sensor();
  
  readSensor(0);
  readSensor(1);
  readSensor(2); 
  
  sendData(0);
  sendData(1);
  sendData(2);
  
  delay(300);
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
  uint8_t LSB0 = rangeCm[0];
  uint8_t MSB0 = rangeCm[0] >> 8;
  uint8_t LSB1 = rangeCm[1];
  uint8_t MSB1 = rangeCm[1] >> 8;
  uint8_t LSB2 = rangeCm[2];
  uint8_t MSB2 = rangeCm[2] >> 8;
  //Serial.write(MSB);
  //Serial.write(LSB);
  Serial.print("sensor 0: ");
  Serial.println(rangeCm[0]);
  Serial.print("sensor 1: ");
  Serial.println(rangeCm[1]);
  Serial.print("sensor 2: ");
  Serial.println(rangeCm[2]);
  // Serial.write(rangeCm[0]);
}

void sendData (int sensor) {
  // Send sensor 1 data
  uint8_t LSB0 = rangeCm[sensor];
  uint8_t MSB0 = rangeCm[sensor] >> 8;
  //Serial.write(MSB);
  //Serial.write(LSB);
  Serial.print("sensor ");
  Serial.print(sensor);
  Serial.print(": ");
  Serial.println(rangeCm[sensor]);
}

void start_sensor() {
  digitalWrite(triggerPin,HIGH);
  delay(1);
  digitalWrite(triggerPin,LOW);
}
