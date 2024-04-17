/**
 * @file PM01_UBW25LDO.cpp
 * @author HMatsufuji (h-matsufuji@hi-corp.jp)
 * @brief 
 * @version 1.0.2
 * @date 2024-04-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "PM01_UBW25LDO.h"

PM01_UBW25LDO::PM01_UBW25LDO(uint8_t address) {
    _address = address;
}  //Constructor for I-squared-C

bool PM01_UBW25LDO::begin(TwoWire &wirePort) {
    _i2cPort = &wirePort;
    _i2cPort->beginTransmission(_address);
    uint8_t _ret = _i2cPort->endTransmission();
    if (!_ret)
        return true;
    else
        return false;
}

//
// command
//

// Into the power-off delay state.
void PM01_UBW25LDO::power_off() {
  _write_byte(PM01_ADDR_POWER_COMMAND, PM01_POWER_COMMAND_DOWN);
}

// Into the power-on state.
void PM01_UBW25LDO::power_on() {
  _write_byte(PM01_ADDR_POWER_COMMAND, PM01_POWER_COMMAND_UP);
}

// Into the sleep state.
void PM01_UBW25LDO::sleep() {
  _write_byte(PM01_ADDR_POWER_COMMAND, PM01_POWER_COMMAND_SLEEP);
}

// Into the deep sleep state.
void PM01_UBW25LDO::deep_sleep() {
  _write_byte(PM01_ADDR_POWER_COMMAND, PM01_POWER_COMMAND_DEEP_SLEEP);
}

// Cancel to cancel deep sleep state.
void PM01_UBW25LDO::deep_sleep_cancel() {
  _write_byte(PM01_ADDR_POWER_COMMAND, PM01_POWER_COMMAND_DEEP_SLEEP_CANCEL);
}

// Set the delay time to deep sleep.
uint8_t PM01_UBW25LDO::set_delay_time(uint8_t dt) {
    return _write_byte(PM01_ADDR_DEEP_SLEEP_DELAY_TIME, dt);
}

// Set the Vin mode.
// usage:
//  setVinMode(PM01_POWER_VIN_MODE_ENABLE);  ... VIN is always enabled.
//  setVinMode(PM01_POWER_VIN_MODE_AUTO);    ... Automatic detection of enable/disable by VIN threshold voltage. Entering deep sleep mode, the state is fixed.
//  setVinMode(PM01_POWER_VIN_MODE_DISABLE); ... If you use battery only, you can disable VIN to reduce current consumption.
//
uint8_t PM01_UBW25LDO::set_vin_mode(uint8_t mode) {
    return _write_byte(PM01_ADDR_VIN_MODE, mode);
}

// set charge setting
// usage:
//   setChargeSetting(PM01_POWER_BATTERY_CHARGE_ENABLE); ... enable battery charge
//   setChargeSetting(PM01_POWER_BATTERY_CHARGE_DISABLE); ... disable battery charge
// 
uint8_t PM01_UBW25LDO::set_charge_setting(uint8_t setting) {
    return _write_byte(PM01_ADDR_CHARGE_SETTING, setting);
}

// set startup charge setting
// No need to call setChargeSetting function next boot
// usage:
//   setStartupChargeSetting(PM01_POWER_BATTERY_CHARGE_ENABLE); ... enable battery charge
//   setStartupChargeSetting(PM01_POWER_BATTERY_CHARGE_DISABLE); ... disable battery charge
//
uint8_t PM01_UBW25LDO::set_startup_charge_setting(uint8_t setting) {
    return _write_byte(PM01_ADDR_STARTUP_CHARGE_SETTING, setting);
}

// Set the VIN threshold voltage.
uint8_t PM01_UBW25LDO::set_vin_threshold(float vinThreshold) {
    uint16_t VT;
    VT = (uint16_t)((vinThreshold / KVIN) * 1024.0);
    return _write_word(PM01_ADDR_VIN_THRESHOLD_L, VT);
}

// Set the battery detect threshold voltage.
uint8_t PM01_UBW25LDO::set_battery_detect_threshold(float batThreshold) {
    uint16_t VT;
    VT = (uint16_t)((batThreshold / KVBAT) * 1024.0);
    return _write_word(PM01_ADDR_BATTERY_DETECT_THRESHOLD_L, VT);
}

// Changes the I2C address.
uint8_t PM01_UBW25LDO::change_address(uint8_t newAddress) {
    if (newAddress < 0x07 || newAddress > 0x78)  // Range of legal addresses
        return false;
    return _write_byte(PM01_ADDR_I2C_ADDRESS, newAddress);
}

//
// request
//

// Get the power module state.
uint8_t PM01_UBW25LDO::get_state() {
    uint8_t state = _read_byte(PM01_ADDR_POWER_STATE);
    return state;
}

// Get the delay time to deep sleep.
uint8_t PM01_UBW25LDO::get_delay_time() {
    return _read_byte(PM01_ADDR_DEEP_SLEEP_DELAY_TIME);
}

// Get the VIN mode.
uint8_t PM01_UBW25LDO::get_vin_mode() {
    return _read_byte(PM01_ADDR_VIN_MODE);
}

// Get the charge setting
uint8_t PM01_UBW25LDO::get_charge_setting() {
    return _read_byte(PM01_ADDR_CHARGE_SETTING);
}

// Get the startup charge setting
uint8_t PM01_UBW25LDO::get_startup_charge_setting() {
    return _read_byte(PM01_ADDR_STARTUP_CHARGE_SETTING);
}

// Get th VIN state.
uint8_t PM01_UBW25LDO::get_vin_state() {
    return _read_byte(PM01_ADDR_VIN_STATE);
}

// Gget the battery charging state.
uint8_t PM01_UBW25LDO::get_battery_charge_state() {
    return _read_byte(PM01_ADDR_BATTERY_CHARGE_STATE);
}

// Get the battery state.
uint8_t PM01_UBW25LDO::get_battery_state() {
    return _read_byte(PM01_ADDR_BATTERY_STATE);
}

// Get the battery voltage.
float PM01_UBW25LDO::get_vin_voltage() {
    return (float)_read_word(PM01_ADDR_VIN_VOLTAGE_L) / 1024 * KVIN;
}

// Get the battery voltage.
float PM01_UBW25LDO::get_battery_voltage() {
    return (float)_read_word(PM01_ADDR_BATTERY_VOLTAGE_L) / 1024 * KVBAT;
}

// Get the VIN threshold voltage.
float PM01_UBW25LDO::get_vin_threshold() {
    return (float)_read_word(PM01_ADDR_VIN_THRESHOLD_L) / 1024 * KVIN;
}

// Get the battery detect threshold voltage.
float PM01_UBW25LDO::get_battery_detect_threshold() {
    return (float)_read_word(PM01_ADDR_BATTERY_DETECT_THRESHOLD_L) / 1024 * KVBAT;
}

// Get the I2C Address.
uint8_t PM01_UBW25LDO::get_i2c_address() {
    return _read_byte(PM01_ADDR_I2C_ADDRESS);
}

//
// private functions
//

// Set a byte
uint8_t PM01_UBW25LDO::_write_byte(uint8_t _command, uint8_t value) {
    uint8_t status;
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command);
    _i2cPort->write(value);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    status = _i2cPort->read();
    return status;
}

// Set 2 bytes word
uint8_t PM01_UBW25LDO::_write_word(uint8_t _command, uint16_t value) {
    uint8_t status;
    union {
        uint16_t value;
        struct {
            byte L;
            byte H;
        };
    } V;
    V.value = value;

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command);
    _i2cPort->write(V.L);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    status = _i2cPort->read();

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command + 1);
    _i2cPort->write(V.H);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    status = _i2cPort->read();

    return status;
}

// Request a byte
uint8_t PM01_UBW25LDO::_read_byte(uint8_t _command) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    uint8_t status = _i2cPort->read();
    return status;
}

// Request a word
uint16_t PM01_UBW25LDO::_read_word(uint8_t _command) {
    uint8_t status;
    union {
        uint16_t value;
        struct {
            byte L;
            byte H;
        };
    } V;
    
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    V.L = _i2cPort->read();

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(_command+1);
    _i2cPort->endTransmission();

    _i2cPort->requestFrom(_address, 1, 1);
    V.H = _i2cPort->read();

    return V.value;
}
