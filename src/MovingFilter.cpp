/*
 * MovingFilter.cpp
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "MovingFilter.h"

namespace pq {

MovingFilter::MovingFilter(Engine& engine)
  : AnalogSource(engine),
    _isCalibrating(true),
    _nValuesStep(0)
{
}

void MovingFilter::reset() {
  _isCalibrating = true;
  _isPreInitialized = false;
  _nValuesStep = 0;
  _nSamples = 0;
}

void MovingFilter::reset(float estimatedMeanValue) {
  MovingFilter::reset();
  _value = estimatedMeanValue;
  _isPreInitialized = true;
}

void MovingFilter::reset(float estimatedMinValue, float estimatedMaxValue) {
  reset(0.5f * (estimatedMinValue + estimatedMaxValue));
  _isPreInitialized = true;
}

void MovingFilter::resumeCalibrating() {
  _isCalibrating = true;
}

void MovingFilter::pauseCalibrating() {
  _isCalibrating = false;
}

void MovingFilter::toggleCalibrating() {
  _isCalibrating = !_isCalibrating;
}

bool MovingFilter::isCalibrating() const {
  return _isCalibrating;
}

}
