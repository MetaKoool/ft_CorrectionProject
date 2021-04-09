#include <Arduino.h>

#include "BluJoy.h"

BluJoy::BluJoy(std::string name, std::string manufacturer, float deadzone)
: ble_gamepad(name, manufacturer, 100) {

  this->name = name;
  this->manufacturer = name;
  this->deadzone = deadzone;
};

void BluJoy::connect () {
  configure_gamepad();
  ble_gamepad.begin(&ble_gamepad_cfg);
}

void BluJoy::prefs_init () {
  preferences.begin("blujoy", false);
}

void BluJoy::read_calibration_data () {
  for (byte axis = 0; axis < AXIS_COUNT; axis++) {
    axis_min[axis] = preferences.getFloat(axis_min_names[axis], -1.0);
    axis_mid[axis] = preferences.getFloat(axis_mid_names[axis], 0.0);
    axis_max[a