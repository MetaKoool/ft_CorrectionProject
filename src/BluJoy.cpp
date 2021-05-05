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
    axis_max[axis] = preferences.getFloat(axis_max_names[axis], 1.0);
  }
}

void BluJoy::write_calibration_data () {
  for (byte axis = 0; axis < BluJoy::AXIS_COUNT; axis++) {
    preferences.putFloat(axis_min_names[axis], axis_min[axis]);
    preferences.putFloat(axis_mid_names[axis], axis_mid[axis]);
    preferences.putFloat(axis_max_names[axis], axis_max[axis]);
  }
}

// https://github.com/Minimuino/thumbstick-deadzones

float BluJoy::map_range (float value, float old_min, float old_max, f