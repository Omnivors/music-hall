/*
 *  MusicHall - arduino side
 *
 *  created Apr 2018
 *  by Francesco Cretti and Luca Morino
 *  in Turin, Italy
 *
 */

/******* GLOBAL VARIABLES *********/
// SONAR PINS - 0-5 analog pins - 2,3 pwm pins
/*
 * sensorPin[0] - group 0 (trigger pin 4) - analog in A0 - 5V
 * sensorPin[1] - group 0 (trigger pin 4) - digital pin 8 - 5V
 * sensorPin[2] - group 1 (trigger pin 5) - digital pin 2 - 3.3V
 * sensorPin[3] - group 2 (trigger pin 6) - digital pin 3 - 3.3V
 * sensorPin[4] - group 1 (trigger pin 5) - analog pin A2 - 3.3V
 * sensorPin[5] - group 2 (trigger pin 6) - analog pin A3 - 3.3V
 * sensorPin[6] - group 1 (trigger pin 5) - analog pin A4 - 3.3V
 * sensorPin[7] - not used
 * 
 */
const int sensorPin[8] = {0, 8, 2, 3, 2, 3, 4, 3};
// TRIGGER PINS - 3 triggers used
const int triggerPin[3] = {4, 5, 6};
// SONAR READINGS
unsigned int rangeCm[8];
// SMOOTHING METHOD - exponentially decaying moving average - equivalent window = 10
const float tiny=1-(1/10.0);


/******* SETUP *********/
void setup () {
  Serial.begin(9600);
  pinMode(triggerPin[0],OUTPUT);
  pinMode(triggerPin[1],OUTPUT);
  //pinMode(triggerPin[2],OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

/******* LOOP *********/
void loop () {
  
  readGroup0();
  
  /*readGroup1();
  
  readGroup0();
  
  readGroup2();*/
  
}

void readGroup0 () {
  // ****** Trigger sensor 0 and 1 (analog) ******
  start_sensor_group_0();
  // ****** Sensor 0 analog reading ******
  readSensor(0); 
  //printData(0);
  send12bitData(0);
  // ****** Sensor 1 PWM reading  ******
  readSensor(1);
  //printData(1);
  send12bitData(1);
  delay(100);
}

void readGroup1 () {
  // ***** Trigger sensor 2 (PWM) 4 and 6 (analog) ******
  start_sensor_group_1();
  // ***** Sensor 2 PWM reading ******
  //readSensor(2); 
  //printData(2);
  //send12bitData(2);
  //delay(100);
  // ***** Sensor 4 analog reading ******
  //readSensor(4);
  //printData(4);
  //send12bitData(4);
  // ***** Sensor 6 analog reading ******
  readSensor(6);
  printData(6);
  //send12bitData(6);
  delay(100);
}

void readGroup2 () {
  // ***** Trigger sensor 3 (PWM) 5 (analog) ******
  start_sensor_group_2();
  // ***** Sensor 2 PWM reading ******
  //readSensor(3); 
  //printData(3);
  //send12bitData(3);
  //delay(100);
  // ***** Sensor 5 analog reading ******
  //readSensor(5);
  //printData(5);
  //send12bitData(5);
  delay(100);
}

void readSensor (int sensor_number) {
  if (sensor_number == 1 || sensor_number == 2 || sensor_number == 3) {
    // pwm reading for sensors 2 and 3
    unsigned int pwmVal = pulseIn(sensorPin[sensor_number], HIGH);
    rangeCm[sensor_number] = pwmVal/58;
    delay(10);
  } else {
    // analog reading 
    unsigned int analog = analogRead(sensorPin[sensor_number]);
    if (analog < 1024) {
      float volt = analog * (5.0 / 1023.0);
      unsigned int cm = volt / 0.005 * 2.0;
      rangeCm[sensor_number] = cm;
      // exp smooth
      //rangeCm[sensor_number] = tiny * rangeCm[sensor_number] + (1 - tiny) * cm;
      }
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

void start_sensor_group_0() {
  digitalWrite(triggerPin[0],HIGH);
  delay(1);
  digitalWrite(triggerPin[0],LOW);
}

void start_sensor_group_1() {
  digitalWrite(triggerPin[1],HIGH);
  delay(1);
  digitalWrite(triggerPin[1],LOW);
}

void start_sensor_group_2() {
  digitalWrite(triggerPin[2],HIGH);
  delay(1);
  digitalWrite(triggerPin[2],LOW);
}
