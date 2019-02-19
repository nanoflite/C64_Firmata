#include <SoftwareSerial.h>

SoftwareSerial ser(10, 11); // RX, TX

void setup() {
    Serial.begin(2400);
    ser.begin(9600);
    // ser.println("Monitor ready");  
    // Serial.println("ready");
}

void loop() {
  // if (Serial.available() > 0) {
  //  char ch = Serial.read();
  //  // Serial.write(ch);
  //  ser.write(ch);
  // }
  // Serial.println("Hello World!");
  for(int i=0; i<256; i++) {
    Serial.write((unsigned char)i);
  }
  delay(500);
}
