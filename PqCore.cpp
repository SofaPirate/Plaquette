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

PqComponent::PqComponent() {
  Pq.add(this);
}

Plaquette::Plaquette() : _nComponents(0) { }

void Plaquette::update() {
  // Update every component.
  for (uint8_t i=0; i<_nComponents; i++)
    _components[i]->update();
}

void Plaquette::setup() {
  // Initialize serial.
  Serial.begin(PLAQUETTE_SERIAL_BAUD_RATE);
  // Initialize all components.
  for (uint8_t i=0; i<_nComponents; i++)
    _components[i]->setup();
}

void Plaquette::add(PqComponent* component) {
  if (_nComponents < PLAQUETTE_MAX_COMPONENTS) {
    _components[_nComponents++] = component;
  }
}

Plaquette Pq;
