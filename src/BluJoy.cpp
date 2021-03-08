#include <Arduino.h>

#include "BluJoy.h"

BluJoy::BluJoy(std::string name, std::string manufacturer, float deadzone)
: ble_gamepad(name, manufacturer, 100) {

  this->name = name;
  this->manufacturer = name;
  this->deadzone = dead