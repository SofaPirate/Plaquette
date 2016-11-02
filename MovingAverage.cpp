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

MovingAverage::MovingAverage(float alphaOrN) : _value(0.5f) {
  setAlphaOrN(alphaOrN);
  reset();
}

MovingAverage::MovingAverage(float alphaOrN, float startValue) : _value(startValue) {
  setAlphaOrN(alphaOrN);
}

void MovingAverage::setAlphaOrN(float alphaOrN)
{
  alphaOrN =  max(alphaOrN, 0); // make sure factor >= 0
  _alpha = (alphaOrN > 1 ?
      2 / (alphaOrN + 1) :
      alphaOrN);
}

void MovingAverage::reset() {
  // Note: the sign of alpha is preserved as an indication of whether the MA has actually
  // started or not.
  if (_alpha >= 0)
    _alpha = -_alpha;
}

void MovingAverage::reset(float startValue) {
  _value = startValue;
  if (_alpha < 0)
    _alpha = -_alpha;
}

float MovingAverage::update(float v) {
  if (!isStarted()) {
    _value = v;
    _alpha = -_alpha;
    return _value;
  }
  else
    return (_value -= _alpha * (_value - v));
}

bool MovingAverage::isStarted() const {
  return _alpha >= 0;
}
