#include "ShroomCorp_AS7265x.h"
#include <Wire.h>

// Constants and static variables
static bool s_measurementComplete[3] = { false, false, false };

// Channel metadata arrays (index = CH_* enum values)
static const uint16_t CHANNEL_WAVELENGTHS[AS7265X_NUM_CHANNELS] = {
  410, 435, 460, 485, 510, 535,   // CH_410NM .. CH_535NM
  560, 585, 610, 645, 680, 705,   // CH_560NM .. CH_705NM
  730, 760, 810, 860, 900, 940    // CH_730NM .. CH_940NM
};

static const char* CHANNEL_NAMES[AS7265X_NUM_CHANNELS] = {
  "A","B","C","D","E","F",   // AS72653 (second slave) A..F -> 410..535
  "G","H","I","J","K","L",   // AS72652 (first  slave) G..L -> 560..705,900,940 order preserved by mapping
  "R","S","T","U","V","W"    // AS72651 (master) R..W -> 610..860
};

// internal per-selector mapping used by the library (keeps legacy mapping)
static const uint8_t MAP_MASTER[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
static const uint8_t MAP_FIRST[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
static const uint8_t MAP_SECOND[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

//////////////////////////////////////////////////////////////////////////////
// Public: readAllChannels
// Reads all three internal devices, performs two snapshots per-selector (bank
// snapshots) and merges them into the provided buf[] (size AS7265X_NUM_CHANNELS).
// Merge rule: prefer non-zero; if both non-zero prefer larger (safer vs truncation).
void ShroomCorp_AS7265x::readAllChannels(uint16_t *buf)
{
  // initialize output
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) buf[i] = 0;

  const uint8_t READ_BASE = 0x08;
  const unsigned long perSensorTimeout = 1200UL; // ms, adjust if your integration time is longer

  struct {
    uint8_t selector;
    const uint8_t *map;
  } sensors[3] = {
    { AS7265X_MASTER_DATA,     MAP_MASTER },
    { AS7265X_FIRST_SLAVE_DATA,MAP_FIRST  },
    { AS7265X_SECOND_SLAVE_DATA,MAP_SECOND}
  };

  // For each internal sensor select & take two banked conversions then merge
  for (uint8_t s = 0; s < 3; s++) {
    uint8_t sel = sensors[s].selector;

    // Ensure we run two conversions so device provides both banks.
    // 1) Trigger conversion + wait, then read snapshot A
    measureSensorAndWait(sel, perSensorTimeout);
    write8(AS7265X_DEV_SELECTOR_REG, sel);
    delay(10);

    uint16_t bankA[6] = {0,0,0,0,0,0};
    for (uint8_t i = 0; i < 6; i++) {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(3);
      uint8_t low  = virtualRead(regHigh + 1);
      bankA[i] = ((uint16_t)high << 8) | (uint16_t)low;
      delay(3);
    }

    // 2) Trigger conversion again (device will produce the other bank) and read snapshot B
    measureSensorAndWait(sel, perSensorTimeout);
    write8(AS7265X_DEV_SELECTOR_REG, sel);
    delay(10);

    uint16_t bankB[6] = {0,0,0,0,0,0};
    for (uint8_t i = 0; i < 6; i++) {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(3);
      uint8_t low  = virtualRead(regHigh + 1);
      bankB[i] = ((uint16_t)high << 8) | (uint16_t)low;
      delay(3);
    }

    // Merge bankA and bankB using datasheet-valid strategy:
    // prefer non-zero values from either bank; if both non-zero prefer larger.
    uint16_t merged[6];
    for (uint8_t i = 0; i < 6; i++) {
      uint16_t a = bankA[i];
      uint16_t b = bankB[i];
      uint16_t val = 0;
      if (a == 0 && b == 0) val = 0;
      else if (a == 0) val = b;
      else if (b == 0) val = a;
      else val = (a > b) ? a : b;
      merged[i] = val;
    }

    // If merged still contains zeros for any channel, attempt one extra conversion/read to try and fill missing
    bool missing = false;
    for (uint8_t i = 0; i < 6; i++) if (merged[i] == 0) { missing = true; break; }

    if (missing) {
      // trigger one more conversion and read snapshot C, then fill missing merged slots
      measureSensorAndWait(sel, perSensorTimeout);
      write8(AS7265X_DEV_SELECTOR_REG, sel);
      delay(8);

      for (uint8_t i = 0; i < 6; i++) {
        uint8_t regHigh = READ_BASE + (i * 2);
        uint8_t high = virtualRead(regHigh);
        delay(3);
        uint8_t low  = virtualRead(regHigh + 1);
        uint16_t c = ((uint16_t)high << 8) | (uint16_t)low;
        if (merged[i] == 0 && c != 0) merged[i] = c;
        delay(3);
      }
    }

    // Map merged values into global buffer using selector-specific map
    for (uint8_t i = 0; i < 6; i++) {
      uint8_t globalIndex = sensors[s].map[i];
      if (globalIndex < AS7265X_NUM_CHANNELS) buf[globalIndex] = merged[i];
    }

    delay(6);
  }

  // ensure selector restored to master when done
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(5);
}

//////////////////////////////////////////////////////////////////////////////
// Helper: read a single channel by wavelength (returns 0xFFFF if not found)
// Reads all channels first and returns the requested channel value.
uint16_t ShroomCorp_AS7265x::readChannelByWavelength(uint16_t wavelength)
{
  // find channel index for the wavelength
  int8_t idx = -1;
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
    if (CHANNEL_WAVELENGTHS[i] == wavelength) { idx = i; break; }
  }
  if (idx < 0) return 0xFFFF;

  uint16_t buf[AS7265X_NUM_CHANNELS];
  readAllChannels(buf);
  return buf[idx];
}

//////////////////////////////////////////////////////////////////////////////
// Helper: read a single channel by photodiode name (A..W). returns 0xFFFF if not found
uint16_t ShroomCorp_AS7265x::readChannelByName(char pdName)
{
  int8_t idx = -1;
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
    if (CHANNEL_NAMES[i][0] == pdName) { idx = i; break; }
  }
  if (idx < 0) return 0xFFFF;

  uint16_t buf[AS7265X_NUM_CHANNELS];
  readAllChannels(buf);
  return buf[idx];
}

//////////////////////////////////////////////////////////////////////////////
// Metadata accessors
uint16_t ShroomCorp_AS7265x::getChannelWavelength(uint8_t channelIndex)
{
  if (channelIndex >= AS7265X_NUM_CHANNELS) return 0;
  return CHANNEL_WAVELENGTHS[channelIndex];
}

const char* ShroomCorp_AS7265x::getChannelName(uint8_t channelIndex)
{
  if (channelIndex >= AS7265X_NUM_CHANNELS) return NULL;
  return CHANNEL_NAMES[channelIndex];
}

///////////////////////////Constructor////////////////////////////////////////////////////
ShroomCorp_AS7265x::ShroomCorp_AS7265x(int8_t addr) 
{ 
  _i2caddr = addr; 
}
///////////////////////////Destructor////////////////////////////////////////////////////
ShroomCorp_AS7265x::~ShroomCorp_AS7265x(void) 
{ 
  if (i2c_dev) delete i2c_dev; 
}

//////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::setIntegrationTime(uint8_t time) 
{
  virtualWrite(AS726X_INT_T, time);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::setGain(uint8_t gain) 
{
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 4);
  control |= (gain << 4);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::setConversionType(uint8_t type) 
{
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(0b11 << 2);
  control |= (type << 2);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}
//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::enableInterrupt() 
{
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control |= (1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::disableInterrupt() 
{
  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);
  control &= ~(1 << 6);
  virtualWrite(AS726X_CONTROL_SETUP, control);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::drvOn() 
{
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led |= (1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::drvOff() 
{
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(1 << 3);
  virtualWrite(AS726X_LED_CONTROL, led);
}

//////////////////////////////////////////////////////////////////////////////
// Programable current of 12.5mA, 25mA, 50mA or 100mA
void ShroomCorp_AS7265x::setDrvCurrent(uint8_t current) 
{
  uint8_t led = virtualRead(AS726X_LED_CONTROL);
  led &= ~(0b11 << 4);
  led |= (current << 4);
  virtualWrite(AS726X_LED_CONTROL, led);
}

//////////////////////////////////////////////////////////////////////////////
bool ShroomCorp_AS7265x::measureSensorAndWait(uint8_t selector, unsigned long timeoutMs) {
  // Select the internal device via physical selector register
  write8(AS7265X_DEV_SELECTOR_REG, selector);
  delay(30); // allow selector settle

  // Ensure integration time/gain/conversion bits are set for this selector
  virtualWrite(AS726X_INT_T, 50);
  delay(5);

  uint8_t control = virtualRead(AS726X_CONTROL_SETUP);

  // set conversion mode to ONE_SHOT (bits [3:2]) per datasheet so Mode2 one-shot runs
  control &= ~(0b11 << 2);
  control |= (ONE_SHOT << 2);
  virtualWrite(AS726X_CONTROL_SETUP, control);
  delay(8);

  // set START bit (bit0) to begin conversion
  control |= 0x01;
  virtualWrite(AS726X_CONTROL_SETUP, control);

  unsigned long start = millis();
  while ((millis() - start) < timeoutMs) {
    uint8_t st = virtualRead(AS726X_CONTROL_SETUP);
    // DATA_RDY indicated by bit1
    if (st & 0x02) return true;
    delay(20);
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////
bool ShroomCorp_AS7265x::dataReady() 
{
  bool all = true;
  for (uint8_t i = 0; i < 3; i++) 
  {
    if (!s_measurementComplete[i]) { all = false; }
  }

  return all;
}

                                /// --- Data Reading ---///
//////////////////////////////////////////////////////////////////////////////
uint8_t ShroomCorp_AS7265x::readTemperature() 
{
  return virtualRead(AS726X_DEVICE_TEMP);
}

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::readCalibratedValues(float *buf) 
{
  // Map internal sensor order -> global channel indices (same as raw mapping)
  const uint8_t map_master[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
  const uint8_t map_first[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
  const uint8_t map_second[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

  struct 
  {
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

//////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::write8(uint8_t reg, uint8_t value) 
{
  Wire.beginTransmission((uint8_t)_i2caddr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
  delay(2);
}

//////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::virtualWrite(uint8_t addr, uint8_t value) 
{
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

//////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////
void ShroomCorp_AS7265x::readRawValuesSequential(uint16_t *buf, unsigned long perSensorTimeoutMs) 
{
  // clear buffer
  for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) buf[i] = 0;

  const uint8_t READ_BASE = 8;
  const uint8_t map_master[6] = { CH_610NM, CH_680NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM };
  const uint8_t map_first[6]  = { CH_560NM, CH_585NM, CH_645NM, CH_705NM, CH_900NM, CH_940NM };
  const uint8_t map_second[6] = { CH_410NM, CH_435NM, CH_460NM, CH_485NM, CH_510NM, CH_535NM };

  struct 
  {
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
    uint8_t sel = sensors[s].selector;

    // If a prior startMeasurement() ran and set the per-selector flag, we still need
    // two conversions to cover both banks. So run up to two conversions as needed.
    // First conversion if not already done
    if (!s_measurementComplete[s]) {
      measureSensorAndWait(sel, perSensorTimeoutMs);
      s_measurementComplete[s] = true;
    }

    // After the first conversion there may still be missing channels (bank complement),
    // so perform a second conversion to get the other bank.
    measureSensorAndWait(sel, perSensorTimeoutMs);

    // select and read 6 raw channels immediately for this selector
    write8(AS7265X_DEV_SELECTOR_REG, sel);
    delay(20);

    // Read two snapshots (they should represent both banks after two conversions)
    uint16_t bankA[6] = {0,0,0,0,0,0};
    uint16_t bankB[6] = {0,0,0,0,0,0};

    for (uint8_t i = 0; i < 6; i++) {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(4);
      uint8_t low  = virtualRead(regHigh + 1);
      bankA[i] = ((uint16_t)high << 8) | (uint16_t)low;
      delay(4);
    }
    delay(8);

    for (uint8_t i = 0; i < 6; i++) {
      uint8_t regHigh = READ_BASE + (i * 2);
      uint8_t high = virtualRead(regHigh);
      delay(4);
      uint8_t low  = virtualRead(regHigh + 1);
      bankB[i] = ((uint16_t)high << 8) | (uint16_t)low;
      delay(4);
    }

    // Merge with same rules, then fallback to a third read if needed
    uint16_t merged[6];
    bool missing = false;
    for (uint8_t i = 0; i < 6; i++) {
      uint16_t a = bankA[i];
      uint16_t b = bankB[i];
      uint16_t val = 0;
      if (a == 0 && b == 0) val = 0;
      else if (a == 0) val = b;
      else if (b == 0) val = a;
      else val = (a > b) ? a : b;
      merged[i] = val;
      if (val == 0) missing = true;
    }

    if (missing) {
      // one more conversion + read to try fill missing slots
      measureSensorAndWait(sel, perSensorTimeoutMs);
      write8(AS7265X_DEV_SELECTOR_REG, sel);
      delay(8);
      for (uint8_t i = 0; i < 6; i++) {
        uint8_t regHigh = READ_BASE + (i * 2);
        uint8_t high = virtualRead(regHigh);
        delay(3);
        uint8_t low  = virtualRead(regHigh + 1);
        uint16_t c = ((uint16_t)high << 8) | (uint16_t)low;
        if (merged[i] == 0 && c != 0) merged[i] = c;
        delay(3);
      }
    }

    // store merged into global buffer
    for (uint8_t i = 0; i < 6; i++) {
      uint8_t globalIndex = sensors[s].map[i];
      if (globalIndex < AS7265X_NUM_CHANNELS) buf[globalIndex] = merged[i];
    }

    delay(12);
  }

  // restore selector
  write8(AS7265X_DEV_SELECTOR_REG, AS7265X_MASTER_DATA);
  delay(10);

  // clear per-cycle flags
  for (uint8_t f = 0; f < 3; f++) s_measurementComplete[f] = false;
}