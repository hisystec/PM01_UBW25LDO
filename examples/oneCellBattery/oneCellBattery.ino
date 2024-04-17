#include <PM01_UBW25LDO.h>

#define PowerModuleAddress 0x08

PM01_UBW25LDO PM01(PowerModuleAddress); 

void setup() {
  Wire.begin();

  // connect to PM01_ power module
  if(PM01.begin()) {

    // set VIN mode auto
    PM01.set_vin_mode(PM01_VIN_MODE_AUTO);

    // set Battery charge enable (default disabled)
    PM01.set_charge_setting(PM01_CHARGE_ENABLE);

    // put your code here:




    // If the battery is charging, stop it to reduce charging noise.
    if (PM01.get_battery_state() == PM01_BATTERY_CHARGING) {
      PM01.set_charge_setting(PM01_CHARGE_DISABLE);
    }

    // put your analog measurement code here:




    // set Battery charge enable
    PM01.set_charge_setting(PM01_CHARGE_ENABLE);

    // into the sleep 
    //PM01.sleep();
    PM01.deep_sleep();
  }
}

void loop() {

}
