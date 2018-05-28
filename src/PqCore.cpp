/*
 * PqCore.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "PqCore.h"

uint8_t Plaquette::_nComponents = 0;
PqUnit* Plaquette::_components[PLAQUETTE_MAX_COMPONENTS];

void Plaquette::step() {
  // Update every component.
  for (uint8_t i=0; i<_nComponents; i++)
    _components[i]->step();
}

void Plaquette::setup() {
  // Initialize serial.
  Serial.begin(PLAQUETTE_SERIAL_BAUD_RATE);
  // Initialize all components.
  for (uint8_t i=0; i<_nComponents; i++) {
    _components[i]->setup();
	}
}

void Plaquette::add(PqUnit* component) {
  for (uint8_t i=0; i<_nComponents; i++) {
		if (_components[i] == component)
			return; // do not add existing component
	}
  if (_nComponents < PLAQUETTE_MAX_COMPONENTS) {
    _components[_nComponents++] = component;
  }
}

PqUnit::PqUnit() {
  Plaquette::add(this);
}

bool  PqUnit::analogToDigital(float f) { return (f >= 0.5); }
float PqUnit::digitalToAnalog(bool  b) { return (b ? 1.0f : 0.0f); }
