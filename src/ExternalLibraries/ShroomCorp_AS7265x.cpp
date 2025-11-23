#include "ShroomCorp_AS7265x.h"

Adafruit_AS7265x::Adafruit_AS7265x(int8_t addr) { _i2caddr = addr; }
Adafruit_AS7265x::~Adafruit_AS7265x(void) { if (i2c_dev) delete i2c_dev; }

bool Adafruit_AS7265x::begin(TwoWire *theWire) {
  if (i2c_dev) delete i2c_dev;
  i2c_dev = new Adafruit_I2CDevice(_i2caddr, theWire);
  if (!i2c_dev->begin()) return false;

  delay(500);
  uint8_t version = virtualRead(AS726X_HW_VERSION);

  // Detect master/slave sensors (should be 0x40 if master initialized)
  if (version != 0x40 && version != 0x4F) {
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
    const uint8_t READ_BASE = 0x08;
    const uint8_t MAX_RETRIES = 5;
    const uint8_t INVALID_BYTE = 0x3F; // invalid-marker observed

    // Read all three sensors (AS72651, AS72652, AS72653)
    for (uint8_t sensor = 1; sensor <= 3; sensor++) {
        // Select sensor bank (virtual register 0x01)
        virtualWrite(0x01, sensor);
        delay(300); // slightly longer settle after bank switch

        uint8_t offset = (sensor - 1) * 6; // sensor1->0, sensor2->6, sensor3->12

        for (uint8_t i = 0; i < 6; i++) {
            uint8_t regHigh = READ_BASE + (i * 2);
            uint8_t high = 0, low = 0;
            bool ok = false;

            for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++) {
                if (attempt) { // re-select bank on retry
                    virtualWrite(0x01, sensor);
                    delay(8);
                }

                high = virtualRead(regHigh);
                delay(300); // allow internal update
                low  = virtualRead(regHigh + 1);

                // Accept if both bytes aren't the invalid marker
                if (high != INVALID_BYTE && low != INVALID_BYTE) {
                    ok = true;
                    break;
                }

                // small backoff before retry
                delay(300);
            }

            uint16_t val = 0;
            if (ok) {
                val = (uint16_t(high) << 8) | uint16_t(low);
            } else {
                // final attempt and verbose debug
                virtualWrite(0x01, sensor);
                delay(10);
                high = virtualRead(regHigh);
                delayMicroseconds(120);
                low = virtualRead(regHigh + 1);
                val = (uint16_t(high) << 8) | uint16_t(low);

                if (((high == INVALID_BYTE) && (low == 0x00)) || (high == INVALID_BYTE && low == INVALID_BYTE)) {
                    Serial.print(" [AS7265x] WARN: invalid read (high=0x");
                    Serial.print(high, HEX);
                    Serial.print(" low=0x");
                    Serial.print(low, HEX);
                    Serial.print(") sensor ");
                    Serial.print(sensor);
                    Serial.print(" reg 0x");
                    Serial.print(regHigh, HEX);
                    Serial.println(" -> set 0");
                    val = 0;
                } else {
                    // log the strange but non-0x3F result (helps debug)
                    Serial.print(" [AS7265x] NOTE: unusual read sensor ");
                    Serial.print(sensor);
                    Serial.print(" reg 0x");
                    Serial.print(regHigh, HEX);
                    Serial.print(" high=0x");
                    Serial.print(high, HEX);
                    Serial.print(" low=0x");
                    Serial.println(low, HEX);
                }
            }

            buf[offset + i] = val;

            // Debug output (can be removed later)
            Serial.print(" AS7265");
            Serial.print(sensor);
            Serial.print(" Channel ");
            Serial.print(offset + i);
            Serial.print(": ");
            Serial.println(val);

            // small spacing between successive reads to help bus/stable timing
            delayMicroseconds(60);
        }
    }

    // Reset to first sensor and allow settle
    virtualWrite(0x01, 0x01);
    delay(10);
}

void Adafruit_AS7265x::startMeasurement() {
    // Ensure conversion type set to ONE_SHOT
    setConversionType(ONE_SHOT);
    delay(5);

    // Start measurement on all three sensors (write ONE_SHOT per bank with small delays)
    for (uint8_t sensor = 1; sensor <= 3; sensor++) {
        virtualWrite(0x01, sensor);
        delay(5);
        uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
        control &= ~(0b11 << 2);
        control |= (ONE_SHOT << 2);
        virtualWrite(AS726X_CONTROL_SETUP, control);
        delay(10); // give the sensor time to latch the command
    }

    // Reset to first sensor
    virtualWrite(0x01, 0x01);
    delay(5);
}

bool Adafruit_AS7265x::dataReady() {
    // Check all three sensors for DATA_RDY (CONTROL_SETUP bit1)
    for (uint8_t sensor = 1; sensor <= 3; sensor++) {
        virtualWrite(0x01, sensor);
        delay(4);
        uint8_t status = virtualRead(AS726X_CONTROL_SETUP);
        // bit1 (0x02) is DATA_RDY per datasheet
        if (!(status & 0x02)) {
            // Reset to first sensor before returning
            virtualWrite(0x01, 0x01);
            return false;
        }
    }
    virtualWrite(0x01, 0x01);
    return true;
}

uint8_t Adafruit_AS7265x::readTemperature() {
  return virtualRead(AS726X_DEVICE_TEMP);
}

// --- Data Reading ---

void Adafruit_AS7265x::readCalibratedValues(float *buf) {
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
    Serial.print(" Reading calibrated value for channel ");
    Serial.println(i);
    buf[i] = readCalibratedChannel(i);
    Serial.print(" Calibrated value: ");
    Serial.println(buf[i]);
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
