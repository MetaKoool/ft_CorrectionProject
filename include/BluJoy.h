#ifndef BLUJOY_H
#define BLUJOY_H

#include <Arduino.h>
#include <BleGamepad.h>
#include <Preferences.h>

class BluJoy {
  private:
    const float SLOPE = 0.414214;

    uint16_t ble_axis_min = 0;
    uint16_t ble_axis_max = 4095;

    BleGamepadConfiguration ble_gamepad_cfg;
    BleGamepad ble_gamepad;

    std::string name;
    std::string manufacturer;

    int16_t map_axis_range_ble(float state);

    byte button_states[11] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
    byte dpad_state = DPAD_CENTERED;

    const float axis_states_raised[6] = { 0.0, 0.0, 0.0, 0.0, -1.0, -1.0 };

    float axis_states[6] = { 0.0, 0.0, 0.0, 0.0, -1.0, -1.0 };
    float