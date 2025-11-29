#include "ShroomCorp_AS7265x.h"
#include <Wire.h>

static bool s_measurementComplete[3] = { false, false, false };

ShroomCorp_AS7265x::ShroomCorp_AS7265x(int8_t addr) { _i2caddr = addr; }
ShroomCorp_AS7265x::~ShroomCorp_AS7265x(void) { if (i2c_dev) delete i2c_dev; }

bool ShroomCorp_AS7265x::begin(TwoWire *theWire) 
{
  if (i2c_dev) delete i2c_dev;
  i2c_dev = new Adafruit_I2CDevice(_i2caddr, theWire);

  if (!i2c_dev->begin()) return false;
  delay(500);

  uint8_t version = virtualRead(AS726X_HW_VERSION);

  // Consider the device missing if HW version read failed (0x00) or returned 0xFF (bus error)
  if (version == 0x00 || version == 0xFF) return false;

  // Configure ALL internal sensors consistently (selector-based)
  const uint8_t selectors[3] = { AS7265X_MASTER_DATA, AS7265X_FIRST_SLAVE_DATA, AS7265X_SECOND_SLAVE_DATA };
  
  for (uint8_t i = 0; i < 3; i++) 
  {
    write8(AS7265X_DEV_SELECTOR_REG, selectors[i]);
    delay(20);
    virtualWrite(AS726X_INT_T, 50);
    delay(5);

    uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
    control &= ~(0b11 << 4); // clear gain bits
    control |= (GAIN_64X << 4);
    control &= ~(0b11 << 2); // clear conv bits
    control |= (ONE_SHOT << 2);
    virtualWrite(AS726X_CONTROL_SETUP, control);
    delay(10);
  }
  // restore master selector
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(10);
  drvOff();
  return true;
}

void ShroomCorp_AS7265x::setIntegrationTime(uint8_t time) {
  virtualWrite(AS726X_INT_T, time);
}

void ShroomCorp_AS7265x::setGain(uint8_t gain) {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 4);
  control |= (gain << 4);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void ShroomCorp_AS7265x::setConversionType(uint8_t type) {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 2);
  control |= (type << 2);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void ShroomCorp_AS7265x::enableInterrupt() {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control |= (1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void ShroomCorp_AS7265x::disableInterrupt() {
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

void ShroomCorp_AS7265x::drvOn() {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led |= (1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

void ShroomCorp_AS7265x::drvOff() {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

// Programable current of 12.5mA, 25mA, 50mA or 100mA
void ShroomCorp_AS7265x::setDrvCurrent(uint8_t current) {
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(0b11 << 4);
  led |= (current << 4);
  virtualWrite(AS726X_LED_CONTROL, led);
}

uint8_t ShroomCorp_AS7265x::readTemperature() {
  return virtualRead(AS726X_DEVICE_TEMP);
}

// --- Data Reading ---

void ShroomCorp_AS7265x::readCalibratedValues(float *buf) {
  // Map internal sensor order -> global channel indices (same as raw mapping)
  const uint8_t map_master[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
  const uint8_t map_first[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
  const uint8_t map_second[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

  struct {
    uint8_t selector;
    const uint8_t *map;
  } sensors[3] = {
    { AS7265X_MASTER_DATA,      map_master },
    { AS7265X_FIRST_SLAVE_DATA, map_first  },
    { AS7265X_SECOND_SLAVE_DATA,map_second }
  };

  // Base of calibrated registers (4 bytes per channel)
  const uint8_t CAL_BASE = 0x14;

  // clear output
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) buf[i] = 0.0f;

  for (uint8_t s = 0; s < 3; s++) {
    // select internal device
    write8(AS7265X_DEV_SELECTOR_REG, sensors[s].selector);
    delay(20);

    for (uint8_t i = 0; i < 6; i++) {
      uint8_t virtualBase = CAL_BASE + (i * 4);
      uint32_t val = ((uint32_t)virtualRead(virtualBase) << 24) |
                     ((uint32_t)virtualRead(virtualBase + 1) << 16) |
                     ((uint32_t)virtualRead(virtualBase + 2) << 8) |
                      (uint32_t)virtualRead(virtualBase + 3);
      float f;
      memcpy(&f, &val, 4);

      uint8_t globalIndex = sensors[s].map[i];
      if (globalIndex < AS7265X_NUM_CHANNELS) buf[globalIndex] = f;
      delay(8);
    }
    delay(15);
  }

  // restore selector
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(10);
}

// I2C Layer basic read/write (instrumented)
uint8_t ShroomCorp_AS7265x::read8(uint8_t reg) 
{
  Wire.beginTransmission((uint8_t)_i2caddr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  uint8_t v = 0;
  
  if (Wire.available()) 
  {
    v = Wire.read();
    return v;
  }
  return 0;
}

void ShroomCorp_AS7265x::write8(uint8_t reg, uint8_t value) 
{
  Wire.beginTransmission((uint8_t)_i2caddr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
  delay(2);
}

// --- I2C virtual register layer (instrumented with timeouts + logs) ---
uint8_t ShroomCorp_AS7265x::virtualRead(uint8_t addr) 
{
  const unsigned long timeout = 200; // ms
  unsigned long start = millis();

  // wait until TX buffer free
  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID) {
    if ((millis() - start) > timeout) return 0xFF;
    delay(1);
  }

  write8(AS726X_SLAVE_WRITE_REG, addr);
  delayMicroseconds(30);

  // wait for RX valid
  start = millis();
  while (!(read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_RX_VALID)) 
  {
    if ((millis() - start) > timeout) return 0xFF;  
    delay(1);
  }

  uint8_t v = read8(AS726X_SLAVE_READ_REG);
  delayMicroseconds(20);

  return v;
}

void ShroomCorp_AS7265x::virtualWrite(uint8_t addr, uint8_t value) {
  const unsigned long timeout = 200; // ms
  unsigned long start = millis();

  // wait until TX buffer free
  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID) 
  {
    if ((millis() - start) > timeout) return;
    delay(1);
  }

  // Write register address with WRITE flag (addr | 0x80) per datasheet
  uint8_t addrWithWrite = addr | 0x80;
  write8(AS726X_SLAVE_WRITE_REG, addrWithWrite);
  delayMicroseconds(30);

  // wait TX clear before sending the data byte
  start = millis();

  while (read8(AS726X_SLAVE_STATUS_REG) & AS726X_SLAVE_TX_VALID) 
  {
    if ((millis() - start) > timeout) 
    {
      return;
    }
    delay(1);
  }

  // Write the value
  write8(AS726X_SLAVE_WRITE_REG, value);
  delayMicroseconds(30);

}

void ShroomCorp_AS7265x::startMeasurement() 
{
  // Reset per-selector completion flags for this round
  for (uint8_t f = 0; f < 3; f++) s_measurementComplete[f] = false;

  // selectors for the three internal sensors
  const uint8_t selectors[3] = { AS7265X_MASTER_DATA, AS7265X_FIRST_SLAVE_DATA, AS7265X_SECOND_SLAVE_DATA };

  // Use a large per-sensor timeout (ms)
  const unsigned long perSensorTimeout = 2000UL;

  for (uint8_t idx = 0; idx < 3; idx++) 
  {
    bool ok = measureSensorAndWait(selectors[idx], perSensorTimeout);
    s_measurementComplete[idx] = ok; // NEW: remember result for dataReady()
    delay(40);
  }

  // Restore selector to master
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(10);
}

bool ShroomCorp_AS7265x::dataReady() 
{
  bool all = true;
  for (uint8_t i = 0; i < 3; i++) 
  {
    if (!s_measurementComplete[i]) { all = false; }
  }

  return all;
}

// --- Data Reading (instrumented) ---
void ShroomCorp_AS7265x::readRawValues(uint16_t *buf) 
{
  // Clear output buffer
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) buf[i] = 0;

  const uint8_t READ_BASE = 0x08;
  const uint8_t map_master[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
  const uint8_t map_first[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
  const uint8_t map_second[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

  struct {
    uint8_t selector;
    const uint8_t *map;
    const char *name;
  } sensors[3] = {
    { AS7265X_MASTER_DATA,     map_master,  "AS72651" },
    { AS7265X_FIRST_SLAVE_DATA,map_first,   "AS72652" },
    { AS7265X_SECOND_SLAVE_DATA,map_second, "AS72653" }
  };

  for (uint8_t s = 0; s < 3; s++) 
  {
    // select which internal device's data to expose via virtual registers
    write8(AS7265X_DEV_SELECTOR_REG, sensors[s].selector);
    delay(30); // allow selector to take effect

    for (uint8_t i = 0; i < 6; i++) 
    {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(10);
      uint8_t low  = virtualRead(regHigh + 1);
      uint16_t val = ((uint16_t)high << 8) | (uint16_t)low;
      uint8_t globalIndex = sensors[s].map[i];

      if (globalIndex < AS7265X_NUM_CHANNELS) 
      {
        buf[globalIndex] = val;
      }
      // short pause between channel reads
      delay(20);
    }

    // small pause between sensors
    delay(40);
  }

  // Reset selector to master for safety
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(20);
}

// --- Measurement control (instrumented) ---
bool ShroomCorp_AS7265x::measureSensorAndWait(uint8_t selector, unsigned long timeoutMs) {
  // Select the internal device via physical selector register
  write8(AS7265X_DEV_SELECTOR_REG, selector);
  delay(30); // allow selector settle

  // Ensure integration time/gain/conversion bits are set for this selector
  // (idempotent - reads current CONTROL_SETUP then modifies conversion bits)
  virtualWrite(AS726X_INT_T, 50);
  delay(5);

  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  
  // set ONE_SHOT in bits[3:2] and preserve gain bits
  control &= ~(0b11 << 2);
  control |= (0x01 << 2); // ONE_SHOT = 1 (ensure macro maps to 1)
  virtualWrite(AS726X_CONTROL_SETUP, control);
  delay(10);

  // set START bit (bit0)
  control |= 0x01;
  virtualWrite(AS726X_CONTROL_SETUP, control);

  

  unsigned long start = millis();

  while ((millis() - start) < timeoutMs) 
  {
    uint8_t st = virtualRead(AS726X_CONTROL_SETUP);

    if (st & 0x02) return true;
    delay(50);
  }

  return false;
}

// ...existing code...
void ShroomCorp_AS7265x::readRawValuesSequential(uint16_t *buf, unsigned long perSensorTimeoutMs) {
  // clear buffer
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) buf[i] = 0;

  const uint8_t READ_BASE = 0x08;
  const uint8_t map_master[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
  const uint8_t map_first[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
  const uint8_t map_second[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

  struct {
    uint8_t selector;
    const uint8_t *map;
    const char *name;
  } sensors[3] = {
    { AS7265X_MASTER_DATA,     map_master,  "AS72651" },
    { AS7265X_FIRST_SLAVE_DATA,map_first,   "AS72652" },
    { AS7265X_SECOND_SLAVE_DATA,map_second, "AS72653" }
  };

  for (uint8_t s = 0; s < 3; s++) {
    uint8_t sel = sensors[s].selector;

    // If a prior startMeasurement() ran and set the per-selector flag, skip retriggering
    if (!s_measurementComplete[s]) 
    {
      measureSensorAndWait(sel, perSensorTimeoutMs);
    } 

    // select and read 6 raw channels immediately for this selector
    write8(AS7265X_DEV_SELECTOR_REG, sel);
    delay(20);

    for (uint8_t i = 0; i < 6; i++) {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(8);
      uint8_t low  = virtualRead(regHigh + 1);
      uint16_t val = ((uint16_t)high << 8) | (uint16_t)low;

      uint8_t globalIndex = sensors[s].map[i];
      if (globalIndex < AS7265X_NUM_CHANNELS) buf[globalIndex] = val;
      delay(12);
    }

    delay(30);
  }

  // restore selector
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(10);

  for (uint8_t f = 0; f < 3; f++) s_measurementComplete[f] = false;
}
