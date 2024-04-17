#include "PM01_UBW25LDO.h"

#define PowerModuleAddress 0x08

PM01_UBW25LDO PM01(PowerModuleAddress); 

uint8_t state;
uint8_t battery_state;
uint8_t vinMode;

void PM01_print_state() {
  state = PM01.get_state();
  Serial.print("PM01 state: ");
  switch (state) {
    case PM01_POWER_STATE_FIRST_POWER_ON:
      Serial.println("First power on");
      break;
    case PM01_POWER_STATE_POWER_ON:
      Serial.println("Power on");
      break;
    case PM01_POWER_STATE_POWER_OFF:
      Serial.println("Power oFf");
      break;
    case PM01_POWER_STATE_SLEEP:
      Serial.println("Sleep");
      break;
    case PM01_POWER_STATE_WAKEUP:
      Serial.println("Wakeup");
      break;
    case PM01_POWER_STATE_DEEP_SLEEP_DELAY:
      Serial.println("Deep sleep delay");
      break;
    case PM01_POWER_STATE_DEEP_SLEEP:
      Serial.println("Deep sleep");
      break;
    case PM01_POWER_STATE_DEEP_WAKEUP:
      Serial.println("Wakeup form deep sleep");
      break;
    case PM01_POWER_STATE_OFFLINE:
      Serial.println("Off line");
      break;
  }
}

void PM01_print_VIN_mode() {
  vinMode = PM01.get_vin_mode();
  Serial.print("VIN mode: ");
  switch(vinMode) {
    case PM01_VIN_MODE_DISABLE:
      Serial.println("Disable");
      break;
    case PM01_VIN_MODE_AUTO:
      Serial.println("Auto");
      break;
    case PM01_VIN_MODE_ENABLE:
      Serial.println("Enable");
      break;
  }
}

void PM01_print_battery_state () {
  battery_state = PM01.get_battery_state();
  switch(battery_state) {
    case PM01_BATTERY_DISCONNECTED:
      Serial.println("Battery disconnected");
      PM01_print_battery_voltage();
      break;
    case PM01_BATTERY_CHARGING:
      Serial.println("Battery charging");
      PM01_print_battery_voltage();
      break;
   case PM01_BATTERY_DISCHARGING:
      Serial.println("Battery discharging");
      PM01_print_battery_voltage();
      break;
  }
}

void PM01_print_battery_voltage() {
  float VBAT;
  VBAT = PM01.get_battery_voltage();
  Serial.print("Battery voltage: ");
  Serial.print(VBAT);
  Serial.println(" [V]");
}

void PM01_print_charge_setting() {
  uint8_t charge_setting, startup_charge_setting;
  charge_setting = PM01.get_charge_setting();
  startup_charge_setting = PM01.get_startup_charge_setting();
  Serial.print("Charge setting: ");
  if (charge_setting) {
    Serial.println("enable");
  } else {
    Serial.println("disable");
  }
  Serial.print("Startup charge setting: ");
  if (startup_charge_setting) {
    Serial.println("enable");
  } else {
    Serial.println("disable");
  }
}

void PM01_print_VIN_state() {
  uint8_t vinstate;
  vinstate = PM01.get_vin_state();
  Serial.print("VIN state: ");
  switch(vinstate) {
    case PM01_VIN_STATE_OFF:
      Serial.println("OFF");
      break;
    case PM01_VIN_STATE_ON:
      Serial.println("ON");
      PM01_print_VIN_voltage();
      break;
  }
}

void PM01_print_VIN_voltage() {
  float VIN;
  VIN = PM01.get_vin_voltage();
  Serial.print("VIN voltage: ");
  Serial.print(VIN);
  Serial.println(" [V]");
}

void PM01_print_VIN_threshold_voltage() {
  float VINth;
  VINth = PM01.get_vin_threshold();
  Serial.print("VIN threshold voltage: ");
  Serial.print(VINth);
  Serial.println(" [V]");
}

void PM01_print_BATDetect_threshold_voltage() {
  float Vth;
  Vth = PM01.get_battery_detect_threshold();
  Serial.print("Battery detect threshold voltage: ");
  Serial.print(Vth);
  Serial.println(" [V]");
}

void setup()
{

  Wire.begin(); 
  Serial.begin(115200);
  while(!Serial) delay(50);

  if(!PM01.begin()) {
    Serial.println("Check connections to Power module.");
  } else {
    Serial.println("Ready to power controll.");
    PM01_print_VIN_mode();
  }
  delay(1000);

  PM01.set_vin_mode(PM01_VIN_MODE_AUTO);
  PM01.set_charge_setting(PM01_BATTERY_CHARGE_ENABLE);
  PM01.set_startup_charge_setting(PM01_BATTERY_CHARGE_DISABLE);
  PM01.set_vin_threshold(4.5);
  PM01.set_battery_detect_threshold(4.23);
  PM01_print_state();
  PM01_print_battery_state();
  PM01_print_VIN_state();
  PM01_print_VIN_threshold_voltage();
  PM01_print_BATDetect_threshold_voltage();
  PM01_print_VIN_mode();

  delay(5000);
  
  //Serial.println("Deep sleep");
  //PM01.setVinMode(PM01_POWER_VIN_MODE_ENABLE);
  
  // deep sleep
  //PM01.deep_sleep();

  // sleep
  //PM01.sleep();

  // infinity sleep
  //PM01.power_off();
}

void loop()
{
  Serial.println("-----------------------");
  PM01_print_state();
  PM01_print_battery_state();
  PM01_print_VIN_state();
  delay(1000);
}
