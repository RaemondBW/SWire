#ifndef SWIRE_I2C
#define SWIRE_I2C

#include <Arduino.h>

class SoftWire {
  private:
    static const int BufferLength = 8;
    uint8_t clock_pin = 6;
    uint8_t data_pin = 7;
    uint8_t current_rw_address;
    bool transmitting = false;
    uint8_t rxBuffer[BufferLength];
    uint8_t rxBufferIndex = 0;
    uint8_t txBuffer[BufferLength];
    uint8_t txBufferIndex = 0;

    void clockPulse();
    uint8_t readBit();
    void sclHi();
    void sdaHi();
    void sclLo();
    void sdaLo();
    void i2cDelay();
    void sendAck();
    void sendNack();

  public:
    uint16_t delay_time_us = 1;

    SWire();
    void start();
    void stop();
    int writeByte(uint8_t data_byte);
    uint8_t readByte();
    uint8_t readAck();
    uint8_t readNack();
    uint8_t doStartWriteAckStop(uint8_t data_byte);
    uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    int available();
    int write(uint8_t);
    int write(uint16_t);
    int write(int);
    int read();
    void begin();
    void begin(uint8_t sdaPin, uint8_t sclPin);
};

extern SoftWire SWire;

#endif
