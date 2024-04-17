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
    PM01_POWER_COMMAND_SLEEP	            = 0x02,
    PM01_POWER_COMMAND_DEEP_SLEEP	        = 0x03,
    PM01_POWER_COMMAND_DEEP_SLEEP_CANCEL	= 0x04
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

    // power off
    //   DO NOT POWER OFF if I2C Master Processor power supplied by this module.
    void power_off();

    // power on
    void power_on();

    // power sleep
    //   power off and sleep
    void sleep();

    // deep sleep
    //   power off and sleep after delay
    //   wakeup by external interrupt
    //   VIN state doesn't change during sleep.
    void deep_sleep();

    // deep sleep cancel
    //   Deep sleep can be cancelled if the delay is still in progress.
    void deep_sleep_cancel();


    //
    // write
    //

    // set_delay_time
    // usage:
    //   set_delay_time(delaytime); ... Delay {delaytime} [ms] before starting deep sleep.
    //                                  delaytime is 0 up to 255 [ms]
    uint8_t set_delay_time(uint8_t setting);

    // set_vin_mode
    // usage:
    //  set_vin_mode(PM01_VIN_MODE_ENABLE);  ... VIN is always enabled.
    //  set_vin_mode(PM01_VIN_MODE_AUTO);    ... Automatic detection of enable/disable by VIN threshold voltage. Entering deep sleep mode, the state is fixed.
    //  set_vin_mode(PM01_VIN_MODE_DISABLE); ... If you use battery only, you can disable VIN to reduce current consumption.
    uint8_t set_vin_mode(uint8_t setting);

    // set_charge_setting
    // usage:
    //   set_charge_setting(PM01_ATTERY_CHARGE_ENABLE); ... enable battery charge
    //   set_charge_setting(PM01_BATTERY_CHARGE_DISABLE); ... disable battery charge
    uint8_t set_charge_setting(uint8_t setting);

    // No need to call setChargeSetting function next boot
    // usage:
    //   set_startup_charge_setting(PM01_BATTERY_CHARGE_ENABLE); ... enable battery charge
    //   set_startup_charge_setting(PM01_BATTERY_CHARGE_DISABLE); ... disable battery charge
    uint8_t set_startup_charge_setting(uint8_t setting);

    // Voltage threshold for detecting that VIN is being input.
    // Usually, there is no need to change from the default values.
    uint8_t set_vin_threshold(float vin_threshold);

    // Voltage threshold for detecting that LiPo battery is being input.
    // Usually, there is no need to change from the default values.
    uint8_t set_battery_detect_threshold(float bat_threshold);

    // If you need to change I2c address, this function is useful for you.
    // Usage:
    //   change_address(new_address); ... change I2C address to new_address.
    //     The address must be between 0x08 and 0x77.
    uint8_t change_address(uint8_t new_address);



    //
    // request
    //
    
    // get state
    // usage: uint8_t state = get_state();
    //   state: PM01_POWER_STATE_FIRST_POWER_ON     : Initial start-up after power-on.
    //          PM01_POWER_STATE_POWER_ON           : Activation by the power_on() function.
    //          PM01_POWER_STATE_POWER_OFF          : Deactivation by the power_off() function.
    //          PM01_POWER_STATE_SLEEP              : Sleeping by sleep() function.
    //          PM01_POWER_STATE_WAKEUP             : Sleep wake-up via INT terminal.
    //          PM01_POWER_STATE_DEEP_SLEEP_DELAY   : Deep sleep delay in progress.
    //          PM01_POWER_STATE_DEEP_SLEEP         : In deep sleep
    //          PM01_POWER_STATE_DEEP_WAKEUP        : Deep sleep wake-up via INT terminal.
    //          PM01_POWER_STATE_OFFLINE            : offline
    uint8_t get_state();

    // get delay time
    // usage: uint8_t delaytime = get_delay_time();
    uint8_t get_delay_time();

    // get_vin_mode
    // usage: uint8_t vin_mode = get_vin_mode();
    //   vin_mode: PM01_VIN_MODE_DISABLE : VIN is always enabled.
    //             PM01_VIN_MODE_AUTO    : Automatic detection of enable/disable by VIN threshold voltage. Entering deep sleep mode, the state is fixed.
    //             PM01_VIN_MODE_ENABLE  : If you use battery only, you can disable VIN to reduce current consumption.
    uint8_t get_vin_mode();

    // get_charge_setting
    // usage: uint8_t charge_setting = get_charge_setting();
    //   charge_setting: PM01_BATTERY_CHARGE_DISABLE : enabled battery charge
    //                   PM01_BATTERY_CHARGE_ENABLE  : disnabled battery charge
    uint8_t get_charge_setting();

    // get_startup_charge_setting
    // usage: uint8_t startup_charge_setting = get_startup_carhge_setting();
    //   charge_setting: PM01_BATTERY_CHARGE_DISABLE : enabled battery charge
    //                   PM01_BATTERY_CHARGE_ENABLE  : disnabled battery charge
    uint8_t get_startup_charge_setting();

    // get_vin_state
    // usage: uint8_t vin_state = get_vin_state();
    //  vin_state: PM01_VIN_STATE_OFF : VIN mode is disabled or VIN less than VIN_THRESHOLD.
    //             PM01_VIN_STATE_ON  : VIN mode is enabled or VIN over than VIN_THRESHOLD.
    uint8_t get_vin_state();

    // get_battery_charge_state
    // Use the more detailed get_battery_state() function.
    uint8_t get_battery_charge_state();

    // get_battery_state
    // usage: uint8_t battery_state = get_battery_state();
    //   battery_state: PM01_BATTERY_DISCONNECTED : Battery disconnectied. 
    //                  PM01_BATTERY_CHARGING     : Battery is still charging
    //                  PM01_BATTERY_DISCHARGING  : Battery is still discharging
    uint8_t get_battery_state();

    // get_vin_voltage
    // usage: float vin_voltage = get_vin_voltage();
    //   vin_voltage [V]
    float get_vin_voltage();

    // get_battery_volgate
    // usage: float battery_voltage = get_battery_voltage();
    //   battery_voltage [V]
    float get_battery_voltage();

    // get_vin_threshold
    // usage: float vin_threshold = get_vin_threshold();
    //   vin_threshold [V]
    float get_vin_threshold();

    // get_battery_detect_threshold
    // usage: float battery_detect_threshold = get_battery_detect_threshold();
    //   battery_detect_threshold [V]
    float get_battery_detect_threshold();

    // get_i2c_address
    // usate: uint8_t i2c_address = get_i2c_address();
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
