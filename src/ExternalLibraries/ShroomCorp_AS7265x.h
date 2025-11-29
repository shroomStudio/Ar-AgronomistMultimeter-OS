/*!
 * @file ShroomCorp_AS7265x.h
 * @briefThe maximum sensitivity value depends on the integration
time. For every 2.78ms of integration time, the maximum
sensitivity value increases by 1024 counts. This means that to
be able to reach the full sensitivity scale, the sensitivity has to
be at least 64*2.78ms.
 */

#ifndef LIB_ADAFRUIT_AS7265X
#define LIB_ADAFRUIT_AS7265X

#include "Arduino.h"
#include "ExternalLibraries/Adafruit_I2CDevice.h"

// Default I2C address
#define AS7265X_ADDRESS (0x49)
#define AS7265X_DEV_SELECTOR_REG 0x4F


enum{
  AS7265X_MASTER_DATA = 0x00,
  AS7265X_FIRST_SLAVE_DATA = 0x01,
  AS7265X_SECOND_SLAVE_DATA = 0x02
};

// AS7265x Virtual Registers
enum {
  AS726X_RAW_VALUE_RGA_H = 0x08,
  AS726X_RAW_VALUE_RGA_L = 0x09,
  AS726X_RAW_VALUE_SHB_H = 0x0A,
  AS726X_RAW_VALUE_SHB_L = 0x0B,
  AS726X_RAW_VALUE_TIC_H = 0x0C,
  AS726X_RAW_VALUE_TIC_L = 0x0D,
  AS726X_RAW_VALUE_UJB_H = 0x0E,
  AS726X_RAW_VALUE_UJB_L = 0x0F,
  AS726X_RAW_VALUE_VKE_H = 0x10,
  AS726X_RAW_VALUE_VKE_L = 0x11,
  AS726X_RAW_VALUE_WLF_H = 0x12,
  AS726X_RAW_VALUE_WLF_L = 0x13
};

enum {
  AS726X_CALIBRATE_VALUE_RGA_0 = 0x14,
  AS726X_CALIBRATE_VALUE_RGA_1 = 0x15,
  AS726X_CALIBRATE_VALUE_RGA_2 = 0x16,
  AS726X_CALIBRATE_VALUE_RGA_3 = 0x17,
  AS726X_CALIBRATE_VALUE_SHB_0 = 0x18,
  AS726X_CALIBRATE_VALUE_SHB_1 = 0x19,
  AS726X_CALIBRATE_VALUE_SHB_2 = 0x1A,
  AS726X_CALIBRATE_VALUE_SHB_3 = 0x1B,
  AS726X_CALIBRATE_VALUE_TIC_0 = 0x1C,
  AS726X_CALIBRATE_VALUE_TIC_1 = 0x1D,
  AS726X_CALIBRATE_VALUE_TIC_2 = 0x1E,
  AS726X_CALIBRATE_VALUE_TIC_3 = 0x1F,
  AS726X_CALIBRATE_VALUE_UJB_0 = 0x20,
  AS726X_CALIBRATE_VALUE_UJB_1 = 0x21,
  AS726X_CALIBRATE_VALUE_UJB_2 = 0x22,
  AS726X_CALIBRATE_VALUE_UJB_3 = 0x23,
  AS726X_CALIBRATE_VALUE_VKE_0 = 0x24,
  AS726X_CALIBRATE_VALUE_VKE_1 = 0x25,
  AS726X_CALIBRATE_VALUE_VKE_2 = 0x26,
  AS726X_CALIBRATE_VALUE_VKE_3 = 0x27,
  AS726X_CALIBRATE_VALUE_WLF_0 = 0x28,
  AS726X_CALIBRATE_VALUE_WLF_1 = 0x29,
  AS726X_CALIBRATE_VALUE_WLF_2 = 0x2A,
  AS726X_CALIBRATE_VALUE_WLF_3 = 0x2B
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
  void readRawValues(uint16_t *buf);
  void readCalibratedValues(float *buf);
  void readRawValuesSequential(uint16_t *buf, unsigned long perSensorTimeoutMs);

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
