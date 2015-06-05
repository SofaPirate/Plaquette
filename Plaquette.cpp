/*
 * Plaquette.cpp
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
#include "Plaquette.h"

PqComponent::PqComponent() {
  Pq.add(this);
}

Plaquette::Plaquette() : _nComponents(0) { }

void Plaquette::update() {
  static bool isSettedUp = false;
  // Setup on first call.
  if (!isSettedUp) {
    setup();
    isSettedUp = true;
  }
  // Update every component.
  for (uint8_t i=0; i<_nComponents; i++)
    _components[i]->update();
}
  
void Plaquette::add(PqComponent* component) {
  if (_nComponents < PLAQUETTE_MAX_COMPONENTS) {
    _components[_nComponents++] = component;
  }
}

void Plaquette::setup() {
  for (uint8_t i=0; i<_nComponents; i++)
    _components[i]->setup();
}

Plaquette Pq;


LED::LED(uint8_t pin) {
	
	 LED(pin, SOURCE);
	 
}

LED::LED(uint8_t pin, outputMode mode){
	 this->pin = pin;
	 this->mode = mode;
	 pinMode(pin,OUTPUT);
}


void LED::on(){
	if ( mode == SOURCE) digitalWrite(pin,HIGH);
	else digitalWrite(pin,LOW);
}
void LED::off(){
	if ( mode == SOURCE) digitalWrite(pin,LOW);
	else digitalWrite(pin,HIGH);
}
	
	
	

Switch::Switch(uint8_t pin) {
	Switch(pin,INTERNAL_PULL_UP);
}
Switch::Switch(uint8_t pin, inputMode mode){
	 this->pin = pin;
	this->mode = mode;
	pinMode(pin,INPUT);
	// INTERNAL_PULL_UP, EXTERNAL_PULL_UP, EXTERNAL_PULL_DOWN 
	if ( mode == INTERNAL_PULL_UP) {
		digitalWrite(pin,OUTPUT);
	}
}

uint8_t Switch::isPressed() {
	if ( mode == INTERNAL_PULL_UP || mode == EXTERNAL_PULL_UP) {
		return !digitalRead(pin);
	} else {
		return digitalRead(pin);
	}
}
		

