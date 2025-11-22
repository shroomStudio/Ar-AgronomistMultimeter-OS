/*!
 * @file Adafruit_AS7265x.h
 * @brief Modified library to support AS7265x (18-channel spectrometer)
 * Based on Adafruit_AS726x library by Dean Miller.
 */

#ifndef LIB_ADAFRUIT_AS7265X
#define LIB_ADAFRUIT_AS7265X

#include "Arduino.h"
#include "ExternalLibraries/Adafruit_I2CDevice.h"

// Default I2C address
#define AS7265X_ADDRESS (0x49)

// AS7265x Virtual Registers
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
  CH_410NM = 0,
  CH_435NM,
  CH_460NM,
  CH_485NM,
  CH_510NM,
  CH_535NM,
  CH_560NM,
  CH_585NM,
  CH_610NM,
  CH_645NM,
  CH_680NM,
  CH_705NM,
  CH_730NM,
  CH_760NM,
  CH_810NM,
  CH_860NM,
  CH_900NM,
  CH_940NM
};

#define AS7265X_NUM_CHANNELS 18
#define AS726X_INTEGRATION_TIME_MULT 2.8

// Conversion modes
enum conversion_types {
  MODE_0 = 0b00,
  MODE_1 = 0b01,
  MODE_2 = 0b10,
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

class Adafruit_AS7265x {
public:
  Adafruit_AS7265x(int8_t addr = AS7265X_ADDRESS);
  ~Adafruit_AS7265x(void);

  bool begin(TwoWire *theWire = &Wire);

  void setIntegrationTime(uint8_t time);
  void setGain(uint8_t gain);
  void setConversionType(uint8_t type);
  void enableInterrupt();
  void disableInterrupt();

  void drvOn();
  void drvOff();
  void setDrvCurrent(uint8_t current);

  void startMeasurement();
  bool dataReady();
  uint8_t readTemperature();

  void readRawValues(uint16_t *buf);
  void readCalibratedValues(float *buf);

  float readCalibratedChannel(uint8_t channel);

private:
  Adafruit_I2CDevice *i2c_dev = NULL;
  uint8_t _i2caddr;

  void write8(uint8_t reg, uint8_t value);
  uint8_t read8(uint8_t reg);
  uint8_t virtualRead(uint8_t addr);
  void virtualWrite(uint8_t addr, uint8_t value);
  void read(uint8_t reg, uint8_t *buf, uint8_t num);
  void write(uint8_t reg, uint8_t *buf, uint8_t num);
};

#endif
