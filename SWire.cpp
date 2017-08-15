#include "SWire.h"

SoftWire::SoftWire() {
  
}

void SoftWire::i2cDelay() {
  delayMicroseconds(delay_time_us);
}

void SoftWire::sclHi() {
  pinMode(clock_pin, INPUT_PULLUP);
}

void SoftWire::sdaHi() {
  pinMode(data_pin, INPUT_PULLUP);
}

void SoftWire::sclLo() {
  digitalWrite(clock_pin, LOW);
  pinMode(clock_pin, OUTPUT);
}

void SoftWire::sdaLo() {
  digitalWrite(data_pin, LOW);
  pinMode(data_pin, OUTPUT);
}

void SoftWire::start() {
  sdaHi();
  sclHi();
  i2cDelay();
  sdaLo();
  i2cDelay();
  sclLo();
  i2cDelay();
}

void SoftWire::stop() {
  sdaLo();
  sclLo();
  i2cDelay();
  sclHi();
  i2cDelay();
  sdaHi();
  i2cDelay();
}

void SoftWire::begin() {
  sdaHi();
  sclHi();
}

void SoftWire::begin(uint8_t sdaPin, uint8_t sclPin) {
  data_pin = sdaPin;
  clock_pin = sclPin;
  sdaHi();
  sclHi();
}

void SoftWire::clockPulse() {
  sclHi();
  i2cDelay();
  sclLo();
}

int SoftWire::writeByte(uint8_t data_byte) {
  for (uint8_t i = 0; i < 8; i++) {
    if (bitRead(data_byte, 7 - i)) {
      sdaHi();
    } else {
      sdaLo();
    }
    i2cDelay();
    clockPulse();
  }
  return readAck();
}

uint8_t SoftWire::readBit() {
  uint8_t out_bit;

  sclHi();
  i2cDelay();
  out_bit = digitalRead(data_pin);
  sclLo();
  i2cDelay();
  return out_bit;
}

uint8_t SoftWire::readByte() {
  uint8_t out_byte = 0;

  sdaHi();
  i2cDelay();
  for (uint8_t i = 0; i < 8; i++) {
    bitWrite(out_byte, 7 - i, readBit());
  }

  return out_byte;
}

/**
 * Return 0 if ACK was received, else 1
 */
uint8_t SoftWire::readAck() {
  sdaHi();
  sclHi();
  i2cDelay();
  uint8_t result = digitalRead(data_pin);
  sclLo();
  sdaLo();
  i2cDelay();
  return result;
}

/**
 * Return 0 if NACK was received, else 1
 */
uint8_t SoftWire::readNack() {
  sdaHi();
  return readBit() == 1 ? 0 : 1;
}

void SoftWire::sendAck() {
  sdaLo();
  i2cDelay();
  clockPulse();
  i2cDelay();
}

void SoftWire::sendNack() {
  sdaHi();
  i2cDelay();
  clockPulse();
  sdaLo();
  i2cDelay();
}

uint8_t SoftWire::doStartWriteAckStop(uint8_t data_byte) {
  start();
  if (writeByte(data_byte)) {
    return 1;
  }
  stop();
  return 0;
}

uint8_t SoftWire::doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length) {
  start();
  for (uint8_t i = 0; i < data_length; i++) {
    if(writeByte(data_bytes[i])) {
      stop();
      return 1;
    }
  }
  stop();
  return 0;
}

void SoftWire::beginTransmission(uint8_t address) {
  current_rw_address = address;
  transmitting = true;
}

void SoftWire::beginTransmission(int address) {
  beginTransmission((uint8_t)address);
}

uint8_t SoftWire::endTransmission(void) {
  return endTransmission(true);
}

uint8_t SoftWire::endTransmission(uint8_t shouldStop) {
  start();
  if (writeByte(((current_rw_address << 1) | 0) & 0xFF)) {
    stop();
    return 1;
  }
  for (int i=0; i<txBufferIndex; i++) {
    if (writeByte(txBuffer[i])) {
      transmitting = false;
      txBufferIndex = 0;
      stop();
      return 1;
    }
  }
  txBufferIndex = 0;
  transmitting = false;
  stop();
  return 0;
}

uint8_t SoftWire::requestFrom(uint8_t address, uint8_t numBytes) {
  start();
  current_rw_address = address;
  if (writeByte(((current_rw_address << 1) | 1) & 0xFF)) {
    stop();
    return 0;
  }

  if (numBytes > BufferLength) {
    numBytes = BufferLength;
  }

  for (int i=0; i<numBytes; i++) {
    rxBuffer[i] = readByte();
    if (i != numBytes - 1) {
      sendAck();
    } else {
      sendNack();
    }
  }

  rxBufferIndex = 0;
  stop();
  return numBytes;
}

uint8_t SoftWire::requestFrom(int address, int numBytes) {
  return requestFrom((uint8_t)address, (uint8_t)numBytes);
}

int SoftWire::available() {
  return !transmitting;
}

int SoftWire::write(uint8_t value) {
  if (txBufferIndex == BufferLength) {
    return 0;
  }
  txBuffer[txBufferIndex] = value;
  txBufferIndex += 1;
}

int SoftWire::write(int value) {
  return write((uint8_t)value);
}

int SoftWire::read() {
  if (rxBufferIndex > BufferLength) {
    return -1;
  }
  int value = rxBuffer[rxBufferIndex];
  rxBufferIndex += 1;
  return value;
}

SoftWire SWire = SoftWire();
