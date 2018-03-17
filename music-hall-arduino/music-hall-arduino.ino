//HC RS04 Sensore ultrasuoni
const int triggerPort = 9;
const int echoPort = 10;
int freqVal;

void setup() {
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW );
  // invia un impulso di 10microsec su trigger
  digitalWrite( triggerPort, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPort, LOW );

  long durata = pulseIn( echoPort, HIGH );
  long distanza = 0.034 * durata / 2;

  //dopo 38ms è fuori dalla portata del sensore
  if (durata > 38000) {
    Serial.write(0);
  } else {
    freqVal = map(distanza, 3, 30, 255, 0);
    freqVal = constrain(freqVal, 0, 255);
    Serial.write(freqVal);
  }

  if (distanza < 10) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(60);
}
