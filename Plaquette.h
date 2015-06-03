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

#define PLAQUETTE_MAX_COMPONENTS 32

/// Main class for components (sensors, actuators) to be added to Plaquette.
class PqComponent {
protected:
  PqComponent();
  virtual ~PqComponent() {}

public:
  virtual void setup() {}
  virtual void update() {}
};

/// The main 
class Plaquette {
private:
  PqComponent* _components[PLAQUETTE_MAX_COMPONENTS];
  uint8_t _nComponents;

public:
  Plaquette();

  /// Needs to be called regularly.
  void update();
  
  /// Adds a component to Plaquette.
  void register(PqComponent* component);

  /// Returns the current number of components.
  uint8_t nComponents() const { return _nComponents; }

private:
  void setup();
};

extern Plaquette Pq;

#endif
