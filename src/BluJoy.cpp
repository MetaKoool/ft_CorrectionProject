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

float BluJoy::map_range (float value, float old_min, float old_max, float new_min, float new_max) {
  if (value < old_min) value = old_min;
  if (value > old_max) value = old_max;

  return (new_min + (new_max - new_min) * (value - old_min) / (old_max - old_min));
}

// https://github.com/Minimuino/thumbstick-deadzones

float BluJoy::dz_scaled_radial (float input) {
  const float input_abs = abs(input);
  if (input_abs < deadzone) {
    return 0.0;
  }

  const float sign = input / input_abs;

  return sign * map_range(input_abs, deadzone, 1.0, 0.0, 1.0);
}

void BluJoy::configure_gamepad () {
  ble_gamepad_cfg.setAutoReport(false);
  ble_gamepad_cfg.setControllerType(CONTROLLER_TYPE_GAMEPAD);

  ble_gamepad_cfg.setHatSwitchCount(1);

  ble_gamepad_cfg.setIncludeStart(true);
  ble_gamepad_cfg.setIncludeSelect(true);
  ble_gamepad_cfg.setIncludeHome(true);

  ble_gamepad_cfg.setButtonCount(BUTTON_COUNT);

  ble_gamepad_cfg.setWhichAxes(false, false, false, false, false, false, false, false);

  ble_gamepad_cfg.setIncludeXAxis(true);
  ble_gamepad_cfg.setIncludeYAxis(true);
  ble_gamepad_cfg.setIncludeZAxis(true);
  bl