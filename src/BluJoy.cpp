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
  ble_gamepad_cfg.setIncludeRzAxis(true);

  ble_gamepad_cfg.setIncludeRxAxis(true);
  ble_gamepad_cfg.setIncludeRyAxis(true);

  ble_gamepad_cfg.setAxesMin(ble_axis_min); // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  ble_gamepad_cfg.setAxesMax(ble_axis_max); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
}

int16_t BluJoy::map_axis_range_ble (float state) {
  return map_range(state, -1.0, 1.0, ble_axis_min, ble_axis_max);
}

float BluJoy::clean_axis_value (byte axis, float state) {
  axis_states_raw[axis] = state;

  float state_calibrated = 0.0;

  if (state > axis_mid[axis]) {
    state_calibrated = map_range(state, axis_mid[axis], axis_max[axis], 0.0, 1.0);
  } else if (state < axis_mid[axis]) {
    state_calibrated = map_range(state, axis_min[axis], axis_mid[axis], -1.0, 0.0);
  }

  return dz_scaled_radial(state_calibrated);
}

void BluJoy::set_axis_state (byte axis, float state) {
  axis_states[axis] = clean_axis_value(axis, state);
}

float BluJoy::get_axis_state (byte axis) {
  return axis_stat