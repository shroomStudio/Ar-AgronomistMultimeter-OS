#include "Adafruit_AS7265x.h"

Adafruit_AS7265x::Adafruit_AS7265x(int8_t addr) { _i2caddr = addr; }
Adafruit_AS7265x::~Adafruit_AS7265x(void) { if (i2c_dev) delete i2c_dev; }

bool Adafruit_AS7265x::begin(TwoWire *theWire) {
  if (i2c_dev) delete i2c_dev;
  i2c_dev = new Adafruit_I2CDevice(_i2caddr, theWire);
  if (!i2c_dev->begin()) return false;

  delay(500);
  uint8_t version = virtualRead(AS726X_HW_VERSION);
  Serial.print("AS7265x HW Version: 0x");
  Serial.println(version, HEX);

  // Detect master/slave sensors (should be 0x40 if master initialized)
  if (version != 0x40 && version != 0x4F) {
    Serial.println("⚠️ Unknown AS726x device");
    return false;
  }

  setIntegrationTime(50);
  setGain(GAIN_64X);
  setConversionType(ONE_SHOT);
  drvOff();
  return true;
}

void Adafruit_AS7265x::setIntegrationTime(uint8_t time) {
  virtualWrite(AS726X_INT_T, time);
}

void Adafruit_AS7265x::setGain(uint8_t gain) {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 4);
  control |= (gain << 4);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void Adafruit_AS7265x::setConversionType(uint8_t type) {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 2);
  control |= (type << 2);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void Adafruit_AS7265x::enableInterrupt() {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control |= (1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void Adafruit_AS7265x::disableInterrupt() {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void Adafruit_AS7265x::drvOn() {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led |= (1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

void Adafruit_AS7265x::drvOff() {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

void Adafruit_AS7265x::setDrvCurrent(uint8_t current) {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(0b11 << 4);
  led |= (current << 4);
  virtualWrite(AS726X_LED_CONTROL, led);
}

void Adafruit_AS7265x::readRawValues(uint16_t *buf) {
    // Need to read from all three sensors
    Serial.println(F("[AS7265x] Reading all sensors..."));

    // Select first sensor (AS72651)
    virtualWrite(0x01, 0x01);
    delay(10);
    
    // Read channels 0-5
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t regHigh = 0x08 + (i * 2);
        uint8_t high = virtualRead(regHigh);
        uint8_t low = virtualRead(regHigh + 1);
        buf[i] = (high << 8) | low;
        Serial.print(F("[AS7265x] AS72651 Channel ")); 
        Serial.print(i);
        Serial.print(F(": 0x"));
        Serial.println(buf[i], HEX);
    }

    // Select second sensor (AS72652)
    virtualWrite(0x01, 0x02);
    delay(10);
    
    // Read channels 6-11
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t regHigh = 0x08 + (i * 2);
        uint8_t high = virtualRead(regHigh);
        uint8_t low = virtualRead(regHigh + 1);
        buf[i + 6] = (high << 8) | low;
        Serial.print(F("[AS7265x] AS72652 Channel ")); 
        Serial.print(i + 6);
        Serial.print(F(": 0x"));
        Serial.println(buf[i + 6], HEX);
    }

    // Select third sensor (AS72653)
    virtualWrite(0x01, 0x03);
    delay(10);
    
    // Read channels 12-17
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t regHigh = 0x08 + (i * 2);
        uint8_t high = virtualRead(regHigh);
        uint8_t low = virtualRead(regHigh + 1);
        buf[i + 12] = (high << 8) | low;
        Serial.print(F("[AS7265x] AS72653 Channel ")); 
        Serial.print(i + 12);
        Serial.print(F(": 0x"));
        Serial.println(buf[i + 12], HEX);
    }

    // Reset to first sensor
    virtualWrite(0x01, 0x01);
}

void Adafruit_AS7265x::startMeasurement() {
    // Start measurement on all three sensors
    virtualWrite(0x01, 0x01); // Select first sensor
    uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
    control &= ~(0b11 << 2);
    control |= (ONE_SHOT << 2);
    virtualWrite(AS726X_CONTROL_SETUP, control);

    virtualWrite(0x01, 0x02); // Select second sensor
    control = virtualRead(AS726X_CONTROL_SETUP);
    control &= ~(0b11 << 2);
    control |= (ONE_SHOT << 2);
    virtualWrite(AS726X_CONTROL_SETUP, control);

    virtualWrite(0x01, 0x03); // Select third sensor
    control = virtualRead(AS726X_CONTROL_SETUP);
    control &= ~(0b11 << 2);
    control |= (ONE_SHOT << 2);
    virtualWrite(AS726X_CONTROL_SETUP, control);

    // Reset to first sensor
    virtualWrite(0x01, 0x01);
}

bool Adafruit_AS7265x::dataReady() {
    bool ready = true;
    
    // Check all three sensors
    for (uint8_t sensor = 1; sensor <= 3; sensor++) {
        virtualWrite(0x01, sensor);
        uint8_t status = virtualRead(AS726X_CONTROL_SETUP);
        Serial.print(F("[AS7265x] Sensor "));
        Serial.print(sensor);
        Serial.print(F(" status: 0x"));
        Serial.println(status, HEX);
        
        if (!(status & 0x02)) {
            ready = false;
            break;
        }
    }
    
    // Reset to first sensor
    virtualWrite(0x01, 0x01);
    return ready;
}

uint8_t Adafruit_AS7265x::readTemperature() {
  return virtualRead(AS726X_DEVICE_TEMP);
}

// --- Data Reading ---

void Adafruit_AS7265x::readCalibratedValues(float *buf) {
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
    buf[i] = readCalibratedChannel(i);
  }
}

float Adafruit_AS7265x::readCalibratedChannel(uint8_t channel) {
  uint8_t base = 0x14 + (channel * 4);
  uint32_t val = ((uint32_t)virtualRead(base) << 24) |
                 ((uint32_t)virtualRead(base + 1) << 16) |
                 ((uint32_t)virtualRead(base + 2) << 8) |
                 (uint32_t)virtualRead(base + 3);
  float ret;
  memcpy(&ret, &val, 4);
  return ret;
}

// --- I2C Layer ---

void Adafruit_AS7265x::write8(uint8_t reg, uint8_t value) {
  uint8_t buf[2] = {reg, value};
  i2c_dev->write(buf, 2);
}

uint8_t Adafruit_AS7265x::read8(uint8_t reg) {
  uint8_t val;
  i2c_dev->write_then_read(&reg, 1, &val, 1);
  return val;
}

uint8_t Adafruit_AS7265x::virtualRead(uint8_t addr) {
  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID);
  write8(AS726X_SLAVE_WRITE_REG, addr);
  while (!(read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_RX_VALID));
  return read8(AS726X_SLAVE_READ_REG);
}

void Adafruit_AS7265x::virtualWrite(uint8_t addr, uint8_t value) {
  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID);
  write8(AS726X_SLAVE_WRITE_REG, addr | 0x80);
  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID);
  write8(AS726X_SLAVE_WRITE_REG, value);
}

void Adafruit_AS7265x::read(uint8_t reg, uint8_t *buf, uint8_t num) {
  i2c_dev->write_then_read(&reg, 1, buf, num);
}

void Adafruit_AS7265x::write(uint8_t reg, uint8_t *buf, uint8_t num) {
  uint8_t temp[1] = {reg};
  i2c_dev->write(buf, num, true, temp, 1);
}
