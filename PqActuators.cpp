#include "PqActuators.h"

PqSingleActuator::PqSingleActuator(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}
  
void PqSingleActuator::setup() {
  pinMode(_pin, OUTPUT);
}

void PqSingleActuator::on() {
  digitalWrite( _pin, isInverted() ? LOW : HIGH );
}

void PqSingleActuator::off() {
  digitalWrite( _pin, isInverted() ? HIGH : LOW );
}

void PqSingleActuator::set(float value) {
  setRaw(round(value * 255));
}

void PqSingleActuator::setRaw(int value) {
  value = constrain(value, 0, 255);
  analogWrite( _pin, isInverted() ? 255-value : value );
}

LED::LED(uint8_t pin, uint8_t mode) : PqSingleActuator(pin, mode) {}

