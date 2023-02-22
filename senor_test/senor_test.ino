// Potentiometer is connected to GPIO 34 (Analog ADC2_CH5) 
const int potPin = 12;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(40);
}
