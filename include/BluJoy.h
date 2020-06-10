#ifndef BLUJOY_H
#define BLUJOY_H

#include <Arduino.h>
#include <BleGamepad.h>
#include <Preferences.h>

class BluJoy {
  private:
    const float SLOPE = 0.414214;

    uint16_t ble_axis_min = 0;
    uint16_t