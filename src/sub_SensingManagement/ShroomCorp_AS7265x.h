/*!
 * @file ShroomCorp_AS7265x.h
 * @brief Property of ShroomCompany 
 * The maximum sensitivity value depends on the integration
 * time. For every 2.78ms of integration time, the maximum
 * sensitivity value increases by 1024 counts. This means that to
 * be able to reach the full sensitivity scale, the sensitivity has to
 * be at least 64*2.78ms.
 * The sensor has 6 photodiodes, but it cannot output all 6 readings at the same time in one bank.
 * So it splits them into two “banks” (groups).
Bank 1
Contains real measurement data from 4 photodiodes.
The remaining 2 registers (out of 6 total) are just zeros (not used in this bank).
Bank 2
Contains real measurement data from the other 4 photodiodes.
Again, the remaining 2 registers are filled with zeros.
Why do this?
Because the chip architecture only allows reading 4 real channels per bank, so the chip outputs the data in two sets.
To get all 6 photodiode readings, you must:
Tell the sensor to switch to Bank 1, read the 6 registers (4 real + 2 zero).
Tell it to switch to Bank 2, read the 6 registers (4 different real + 2 zero).
Example (imaginary numbers)
Bank 1: [Ch0, Ch1, Ch2, Ch3, 0, 0]
Bank 2: [Ch4, Ch5, Ch6, Ch7, 0, 0]
Even though the sensor has 6 channels, the registers are arranged as 6 slots, and some get zeroed depending on which bank is active.*/
#ifndef LIB_SHROOMCORP_AS7265X
#define LIB_SHROOMCORP_AS7265X

#include <Arduino.h>
#include <ExternalLibraries/Adafruit_I2CDevice.h>

// Default I2C address
#define AS7265X_ADDRESS (0x49)
#define AS7265X_DEV_SELECTOR_REG 0x4F
#define AS7265X_NUM_CHANNELS 18

enum {
  AS7265X_MASTER_DATA = 0x00,
  AS7265X_FIRST_SLAVE_DATA = 0x01,
  AS7265X_SECOND_SLAVE_DATA = 0x02
};

enum {
  AS726X_HW_VERSION     = 0x00,
  AS726X_FW_VERSION     = 0x02,
  AS726X_CONTROL_SETUP  = 0x04,
  AS726X_INT_T          = 0x05,
  AS726X_DEVICE_TEMP    = 0x06,
  AS726X_LED_CONTROL    = 0x07
};

// AS7265x master/slave communication registers
enum {
  AS726X_SLAVE_STATUS_REG = 0x00,
  AS726X_SLAVE_WRITE_REG  = 0x01,
  AS726X_SLAVE_READ_REG   = 0x02,
  AS726X_SLAVE_TX_VALID   = 0x02,
  AS726X_SLAVE_RX_VALID   = 0x01,
};

// AS7265x Channel Wavelengths (18 channels)
enum {
  CH_410NM = 0, CH_435NM, CH_460NM, CH_485NM, CH_510NM,
  CH_535NM, CH_560NM, CH_585NM, CH_610NM, CH_645NM, CH_680NM,
  CH_705NM, CH_730NM, CH_760NM, CH_810NM, CH_860NM, CH_900NM,
  CH_940NM
};

// Conversion modes
enum conversion_types {
  MODE_0 = 0b00, // S, T, U, V, I, G, H, K, C, A, B, E
  MODE_1 = 0b01, // R, T, U, W, L, G, H, J, F, A, B, D
  MODE_2 = 0b10, // S, T, U, V, I, G, H, K, C, A, B, E // R, T, U, W, L, G, H, J, F, A, B, D
  ONE_SHOT = 0b11
};

// Gain settings
enum channel_gain {
  GAIN_1X = 0b00,
  GAIN_3X7 = 0b01,
  GAIN_16X = 0b10,
  GAIN_64X = 0b11
};

// LED current settings
enum drv_led_current_limits {
  LIMIT_12MA5 = 0b00,
  LIMIT_25MA  = 0b01,
  LIMIT_50MA  = 0b10,
  LIMIT_100MA = 0b11
};

class ShroomCorp_AS7265x {
public:
  ShroomCorp_AS7265x(int8_t addr = AS7265X_ADDRESS);
  ~ShroomCorp_AS7265x(void);

  bool begin(TwoWire *theWire = &Wire);

  void setIntegrationTime(uint8_t time);
  void setGain(uint8_t gain);
  void setConversionType(uint8_t type);

  void drvOn();
  void drvOff();
  void setDrvCurrent(uint8_t current);
  void enableInterrupt();
  void disableInterrupt();

  void startMeasurement();
  bool dataReady();
  uint8_t readTemperature();

  // Read all 18 mapped spectral channels (combines per-device banks)
  // buf must be AS7265X_NUM_CHANNELS length
  void readAllChannels(uint16_t *buf);

  // Backwards-compatible name
  void readRawValues(uint16_t *buf) { readAllChannels(buf); }

  // Read calibrated float channel values (18 channels)
  void readCalibratedValues(float *buf);

  // Sequential raw read that optionally waits per-selector (keeps legacy interface)
  void readRawValuesSequential(uint16_t *buf, unsigned long perSensorTimeoutMs);

  // Helpers to access channel metadata / single-channel reads
  // wavelength in nm (e.g. 410, 435, 460 ...)
  uint16_t readChannelByWavelength(uint16_t wavelength);
  // name is single char A..W as used in datasheet mapping (A..L, etc.)
  uint16_t readChannelByName(char pdName);
  uint16_t getChannelWavelength(uint8_t channelIndex);
  const char* getChannelName(uint8_t channelIndex);

private:
  Adafruit_I2CDevice *i2c_dev = NULL;
  uint8_t _i2caddr;

  uint8_t read8(uint8_t reg);
  void write8(uint8_t reg, uint8_t value);
  uint8_t virtualRead(uint8_t addr);
  void virtualWrite(uint8_t addr, uint8_t value);
  bool measureSensorAndWait(uint8_t selector, unsigned long timeoutMs);
};

#endif
