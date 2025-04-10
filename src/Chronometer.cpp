/*
 * Chronometer.cpp
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "Chronometer.h"

namespace pq {

Chronometer::Chronometer(Engine& engine) : Unit(engine), AbstractChronometer(), _engine(engine) {
}

float Chronometer::put(float value) {
  set(value);
  return get();
}
  
void Chronometer::begin() {
  stop();
}

void Chronometer::step() {
  update();
}

float Chronometer::_time() const {
  return _engine.seconds();
}

}
