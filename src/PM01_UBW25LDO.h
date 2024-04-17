#ifndef __PM01_UBW25LDO__
#define __PM01_UBW25LDO__

#include <Wire.h>
#include <Arduino.h>

enum ST_PM01_POWER_MODULE_STATE {
    PM01_POWER_STATE_FIRST_POWER_ON     = 0x00,
    PM01_POWER_STATE_POWER_ON           = 0x01,
    PM01_POWER_STATE_POWER_OFF          = 0x02,
    PM01_POWER_STATE_SLEEP              = 0x03,
    PM01_POWER_STATE_WAKEUP             = 0x04,
    PM01_POWER_STATE_DEEP_SLEEP_DELAY   = 0x05,
    PM01_POWER_STATE_DEEP_SLEEP         = 0x06,
    PM01_POWER_STATE_DEEP_WAKEUP        = 0x07,
    PM01_POWER_STATE_OFFLINE            = 0xff
};

enum ST_I2C_ADDR {
    PM01_ADDR_POWER_STATE	                = 0x00,
    PM01_ADDR_POWER_COMMAND                 = 0x01,
    PM01_ADDR_DEEP_SLEEP_DELAY_TIME         = 0x03,
    PM01_ADDR_VIN_MODE                      = 0x04,
    PM01_ADDR_CHARGE_SETTING                = 0x06,
    PM01_ADDR_STARTUP_CHARGE_SETTING        = 0x07,
    PM01_ADDR_VIN_STATE                     = 0x08,
    PM01_ADDR_BATTERY_CHARGE_STATE          = 0x09,
    PM01_ADDR_BATTERY_STATE                 = 0x0a,
    PM01_ADDR_VIN_VOLTAGE_L                 = 0x10,
    PM01_ADDR_VIN_VOLTAGE_H                 = 0x11,
    PM01_ADDR_BATTERY_VOLTAGE_L             = 0x12,
    PM01_ADDR_BATTERY_VOLTAGE_H             = 0x13,
    PM01_ADDR_VIN_THRESHOLD_L               = 0x14,
    PM01_ADDR_VIN_THRESHOLD_H               = 0x15,
    PM01_ADDR_BATTERY_DETECT_THRESHOLD_L    = 0x16,
    PM01_ADDR_BATTERY_DETECT_THRESHOLD_H    = 0x17,
    PM01_ADDR_I2C_ADDRESS                   = 0x1f
};

enum ST_POWER_COMMAND {
    PM01_POWER_COMMAND_DOWN                 = 0x00,
    PM01_POWER_COMMAND_UP                   = 0x01,
    PM01_POWER_COMMAND_SLEEP	              = 0x02,
    PM01_POWER_COMMAND_DEEP_SLEEP	          = 0x03,
    PM01_POWER_COMMAND_DEEP_SLEEP_CANCEL	  = 0x04
};

enum ST_BATTERY_CHARGE_STATE {
    PM01_BATTERY_STATE_CHARGE     = 0x00,
    PM01_BATTERY_STATE_DISCHARGE  = 0x01
};

enum ST_BATTERY_STATE {
    PM01_BATTERY_DISCONNECTED    = 0x00,
    PM01_BATTERY_CHARGING        = 0x01,
    PM01_BATTERY_DISCHARGING     = 0x02
};

enum ST_BATTERY_CHARGE_SETTING {
    PM01_BATTERY_CHARGE_DISABLE   = 0x00,
    PM01_BATTERY_CHARGE_ENABLE    = 0x01
};

enum ST_VIN_MODE {
    PM01_VIN_MODE_DISABLE = 0x00,
    PM01_VIN_MODE_AUTO    = 0x01,
    PM01_VIN_MODE_ENABLE  = 0x02
};

enum ST_VIN_STATE {
    PM01_VIN_STATE_OFF   = 0x00,
    PM01_VIN_STATE_ON    = 0x01
};

#define DEFAULT_ADDRESS 0x08
#define INCORR_PARAM 0xFF
#define KVBAT 6.6
#define KVIN 26.11

class PM01_UBW25LDO {
public:
    //
    PM01_UBW25LDO(uint8_t address);
    bool begin(TwoWire &wirePort = Wire);  // begin I2C process
    //
    // command
    //
    void power_off();
    void power_on();
    void sleep();
    void deep_sleep();
    void deep_sleep_cancel();
    //
    uint8_t set_delay_time(uint8_t setting);
    uint8_t set_vin_mode(uint8_t setting);
    uint8_t set_charge_setting(uint8_t setting);
    uint8_t set_startup_charge_setting(uint8_t setting);
    uint8_t set_vin_threshold(float vin_threshold);
    uint8_t set_battery_detect_threshold(float bat_threshold);
    uint8_t change_address(uint8_t new_address);
    //
    // request
    //
    uint8_t get_state();
    uint8_t get_delay_time();
    uint8_t get_vin_mode();
    uint8_t get_charge_setting();
    uint8_t get_startup_charge_setting();
    uint8_t get_vin_state();
    uint8_t get_battery_charge_state();
    uint8_t get_battery_state();
    float get_vin_voltage();
    float get_battery_voltage();
    float get_vin_threshold();
    float get_battery_detect_threshold();
    uint8_t get_i2c_address();

private:

  int _address;
  uint8_t _state;
  uint8_t _write_byte(uint8_t _command, uint8_t value);
  uint8_t _write_word(uint8_t _command, uint16_t value);
  uint8_t _read_byte(uint8_t _command);
  uint16_t _read_word(uint8_t _command);
  TwoWire *_i2cPort;
};

#endif
