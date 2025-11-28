/*
 * MovingAverage.cpp
 *
 * Tool for moving averages.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * Inspired by code by Karsten Kutza
 * http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
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

#include "MovingAverage.h"

namespace pq {

MovingAverage::MovingAverage() {
  reset();
}

void MovingAverage::reset() {
  _value = 0;
}

void MovingAverage::reset(float initialValue) {
  _value = initialValue;
}

float MovingAverage::update(float v, float alpha) {
  return applyMovingAverageUpdate(_value, v, alpha);
}

float MovingAverage::amend(float previousValue, float newValue, float alpha) {
  return amendMovingAverageUpdate(_value, previousValue, newValue, alpha);
}

float MovingAverage::delta(float d) {
  return applyMovingAverageDelta(_value, d);
}

} // namespace pq
