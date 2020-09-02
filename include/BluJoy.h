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
    float axis_states_raw[6] = { 0.0, 0.0, 0.0, 0.0, -1.0, -1.0 };

    float axis_min[6] = { -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };
    float axis_mid[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    float axis_max[6] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

  public:
    Preferences preferences;

    static const byte BUTTON_COUNT = 11;
    static const byte AXIS_COUNT = 6;

    static const byte BUTTON_A = 0;
    static const byte BUTTON_B = 1;
    static const byte BUTTON_X = 2;
    static const byte BUTTON_Y = 3;
    static const byte BUTTON_LB = 4;
    static const byte BUTTON_RB = 5;

    static const byte BUTTON_LSB = 6;
    static const byte BUTTON_RSB = 7;

    static const byte BUTTON_START = 8;
    static const byte BUTTON_SELECT = 9;
    static const byte BUTTON_HOME = 10;

    static const byte AXIS_LX = 0;
    static const byte AXIS_LY = 1;
    static const byte AXIS_RX = 2;
    static const byte AXIS_RY = 3;
    static const byte AXIS_LT = 4;
    static const byte AXIS_RT = 5;

    float deadzone = 0.2;

    const char * button_names[11] = { "A", "B", "X", "Y", "LB", "RB", "LSB", "RSB", "START", "SELECT", "HOME" };
    const char * axis_names[6] = { "LX", "LY", "RX", "RY", "LT", "RT" };

    const char * axis_min_names[6] = { "lx-axis-min", "ly-axis-min", "rx-axis-min", "ry-axis-min", "lt-axis-min", "lt-axis-min" };
    const char * axis_mid_names[6] = { "lx-axis-m