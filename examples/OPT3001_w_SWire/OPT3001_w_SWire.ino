//Example to show SWire usage with the OPT3001 ALS
#include <SWire.h>

void setup() {
  Serial.begin(115200);
  //Setup my sensor
  SWire.begin(4, 5); // Use Pin 4 for SDA & Pin 5 for SCL
  SWire.beginTransmission(0x45);
  SWire.write(0x01);
  SWire.write(0xC6);
  SWire.write(0x10);
  Serial.println(SWire.endTransmission());
}

void loop() {
  //Read Sensor results
  uint16_t result = 0;
  SWire.beginTransmission(0x45);
  SWire.write(0x00);
  int error = SWire.endTransmission(false);
  if (error == 0) {
    SWire.requestFrom(0x45, 2);
    Serial.println(SWire.available());
    result = (uint16_t)SWire.read() << 8;
    result |= (uint16_t)SWire.read();
    uint16_t exponent = result >> 12;
    uint16_t fractionalResult = result & (uint16_t)0x0FFF;
    Serial.println(0.01*pow(2, exponent)*fractionalResult);
  }
  delay(100);
}
