int incomingByte = 0;

void setup() {
    Serial.begin(2400);  
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print((char)incomingByte);
  }
}
