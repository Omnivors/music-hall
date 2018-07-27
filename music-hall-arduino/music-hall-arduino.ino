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
const int sensorPin[6] = {0, 1, 2, 3, 4, 5};
// SONAR READINGS
unsigned int rangeCm[3];
// SMOOTHING METHOD - exponentially decaying moving average - equivalent window = 10
const float tiny=1-(1/10.0);
// Trigger
const int triggerPin[2] = {2, 4};

/******* SETUP *********/
void setup () {
  Serial.begin(9600);
  pinMode(triggerPin[0],OUTPUT);
  pinMode(triggerPin[1],OUTPUT);
  //pinMode(triggerPin[2],OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  delay(300);
}

/******* LOOP *********/
void loop () {
  start_sensor(0);
  readSensor(0);
  //printData(0);
  send12bitData(0);
  delay(100);
  start_sensor(1);
  readSensor(1);
  //printData(1);
  send12bitData(1);
  delay(100);
  start_sensor(2);
  readSensor(2);
  //printData(2);
  send12bitData(2);
  start_sensor(3);
  readSensor(3);
  //printData(3);
  send12bitData(3);
  delay(100);
  start_sensor(4);
  readSensor(4);
  //printData(4);
  send12bitData(4);
  start_sensor(5);
  readSensor(5);
  //printData(5);
  send12bitData(5);
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

void sendAllData () {
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
  uint8_t LSB = rangeCm[sensor];
  uint8_t MSB = rangeCm[sensor] >> 8;
  Serial.write(MSB);
  Serial.write(LSB);
}

void printData (int sensor) {
  Serial.print("sensor ");
  Serial.print(sensor);
  Serial.print(": ");
  Serial.println(rangeCm[sensor]);
}

void send14bitData (int sensor) {
  uint8_t low = rangeCm[sensor] & 0b01111111; // seven least significant bits (bits 0-6)
  uint8_t high = (rangeCm[sensor] >> 7) & 0b01111111; // bits 7-13
  Serial.write(0b10000000 | high); // set bit 7 to 1, to indicate that this is these are the seven high bits.
  Serial.write(low);
}

void send12bitData (int sensor) {
  uint8_t check = 0b10000000 | (16 * sensor); // choose sensor
  uint8_t low = rangeCm[sensor] & 0b01111111; // seven least significant bits (bits 0-6)
  uint8_t high = ((rangeCm[sensor] >> 7) & 0b00001111); // bits 7-11
          high = check | high;
  //Serial.print("sensor ");
  //Serial.println(sensor);
  //Serial.print("value int ");
  //Serial.println(rangeCm[sensor]);
  //Serial.print("high ");
  //Serial.println(high); // set bit 7 to 1, to indicate that this is these are the seven high bits.
  //Serial.print("low ");
  //Serial.println(low);
  Serial.write(high);
  Serial.write(low);
}

void start_sensor(int sensor) {
  int s = sensor > 0;
  digitalWrite(triggerPin[s],HIGH);
  delay(1);
  digitalWrite(triggerPin[s],LOW);
}
