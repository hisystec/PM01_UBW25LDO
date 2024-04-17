#include <PM01_UBW25LDO.h>

#define PowerModuleAddress 0x08

PM01_UBW25LDO PM01(PowerModuleAddress); 

void setup() {
  Wire.begin();

  // connect to PM01 power module
  if(PM01.begin()) {

    // set VIN mode to enable
    PM01.set_vin_mode(PM01_VIN_MODE_ENABLE);

    // set startup battery charge to enable (defautl disbaled)
    // No need to call setChargeSetting function next boot
    PM01.set_startup_charge_setting(PM01_BATTERY_CHARGE_ENABLE);

    // set battery charge to enable (defautl disbaled)
    PM01.set_charge_setting(PM01_BATTERY_CHARGE_ENABLE);
    
    // put your code here:




    // into the sleep 
    PM01.deep_sleep();
  }
}

void loop() {

}
