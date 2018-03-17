//HC RS04 Sensore ultrasuoni
//Giuseppe Caccavale
const int triggerPort = 9;
const int echoPort = 10;
//const int led = 13;
int freqVal;

void setup() {

pinMode(triggerPort, OUTPUT);
pinMode(echoPort, INPUT);
pinMode(LED_BUILTIN, OUTPUT);
Serial.begin(9600);
// Serial.print( "Sensore Ultrasuoni: ");
}

void loop() {

//porta bassa l'uscita del trigger
digitalWrite( triggerPort, LOW );
//invia un impulso di 10microsec su trigger
digitalWrite( triggerPort, HIGH );
delayMicroseconds( 10 );
digitalWrite( triggerPort, LOW );

long durata = pulseIn( echoPort, HIGH );

long distanza = 0.034 * durata / 2;

// Serial.print("freq: ");

//dopo 38ms è fuori dalla portata del sensore
if( durata > 38000 ){
//Serial.println("Fuori portata   ");
Serial.write(0);
}
else{ 
//Serial.print(" cm: ");
//Serial.println(distanza); 

// Serial.write(distanza);
freqVal = map(distanza, 3, 30, 255, 0);
//Serial.print(distanza);
//Serial.println(" cm");
freqVal = constrain(freqVal, 0, 255);
//Serial.println(freqVal);
Serial.write(freqVal);
//Serial.println(" Hz     ");
}

if(distanza < 10){
 digitalWrite(LED_BUILTIN, HIGH);
}
else{
 digitalWrite(LED_BUILTIN, LOW);
}

//Aspetta 1000 microsecondi
delay(60);
}
