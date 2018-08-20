#define PIN_A 6
#define PIN_B 7
#define PIN_C 8

void setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
}

void loop() {
  digitalWrite(PIN_A, HIGH);
  delay(250);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_A, LOW);
  delay(250);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_B, LOW);
  delay(250);
  digitalWrite(PIN_C, LOW);
}
