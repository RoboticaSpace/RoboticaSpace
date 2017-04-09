#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);           // start serial for output
}

void loop() 
{
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(">");         // print the character
    Serial.println(c,HEX);         // print the character
  }
  
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c,HEX);         // print the character
    Serial.print(" ");         // print the character
  }
  Serial.println(" ");         // print the character
}
