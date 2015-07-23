#include "PqSensors.h"

PqSingleSensor::PqSingleSensor(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}

bool PqSingleSensor::isOn() const
{
  return (get() > 0.5f);
}

float PqSingleSensor::get() const
{
  int rawValue = getRaw();
  return (isDigital() ? rawValue : rawValue / 1023.0f);
}

int PqSingleSensor::getRaw() const
{
  if (isDigital())
  {
    int rawValue = digitalRead(_pin) == HIGH ? 1 : 0;
    return (isInverted() ? 1-rawValue : rawValue);
  }
  else
  {
    int rawValue = analogRead(_pin);
    return (isInverted() ? 1023-rawValue : rawValue);
  }
}
		
void PqSingleSensor::setup() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
}

