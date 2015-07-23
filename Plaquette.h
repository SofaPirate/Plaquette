/*
 * Plaquette.h
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

#ifndef PLAQUETTE_H_
#define PLAQUETTE_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PLAQUETTE_MAX_COMPONENTS 32

enum inputMode { INTERNAL_PULL_UP, EXTERNAL_PULL_UP, EXTERNAL_PULL_DOWN };
enum outputMode { SOURCE , SINK  };

/**
 * Main class for components to be added to Plaquette.
 * Components can be transducers (sensors,actuators) or special integrated circuits.
 */
class PqComponent {
protected:
  PqComponent();
  virtual ~PqComponent() {}

public:
  virtual void setup() {}
  virtual void update() {}
};

/// The main Plaquette singleton class containing all the components.
class Plaquette {
private:
  PqComponent* _components[PLAQUETTE_MAX_COMPONENTS];
  uint8_t _nComponents;

public:
  Plaquette();

  /// Needs to be called regularly.
  void update();
  
  /// Adds a component to Plaquette.
  void add(PqComponent * component);

  /// Returns the current number of components.
  uint8_t nComponents() const { return _nComponents; }

private:
  /*
   * This method will be automatically called on the first call to Plaquette.update()
   * and will call all of the components' setup() method.
   */
  void setup();
};

/// The singleton.
extern Plaquette Pq;


class LED : public PqComponent {
	
	public :
	  LED(uint8_t pin=13, outputMode mode=SOURCE);
    virtual void setup();
		void on();
		void off();

	protected :
		uint8_t _pin;
		outputMode _mode;
};

class Switch : public PqComponent {
	
	public :
	    Switch(uint8_t pin);
		Switch(uint8_t pin, inputMode mode);
		uint8_t isPressed();
	private :
		uint8_t pin;
		inputMode mode;
		
		
};

#endif