/*
 * Normalizer.cpp
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

#include "MovingStats.h"
#include "Normalizer.h"

namespace pq {

Normalizer::Normalizer()
: MovingFilter(),
  MovingStats()
{
  _value = NORMALIZER_DEFAULT_MEAN;
  targetMean(NORMALIZER_DEFAULT_MEAN);
  targetStdDev(NORMALIZER_DEFAULT_STDDEV);
  clamp();
}

Normalizer::Normalizer(float timeWindow)
: MovingFilter(),
  MovingStats(timeWindow)
{
  _value = NORMALIZER_DEFAULT_MEAN;
  targetMean(NORMALIZER_DEFAULT_MEAN);
  targetStdDev(NORMALIZER_DEFAULT_STDDEV);
  clamp();
}

Normalizer::Normalizer(float mean, float stdDev)
	: MovingFilter(),
    MovingStats()
{
  _value = mean;
  targetMean(mean);
  targetStdDev(stdDev);
  clamp();
}

Normalizer::Normalizer(float mean, float stdDev, float timeWindow)
	: MovingFilter(),
    MovingStats(timeWindow)
{
  _value = mean;
  targetMean(mean);
  targetStdDev(stdDev);
  clamp();
}

void Normalizer::infiniteTimeWindow() {
  MovingStats::infiniteTimeWindow();
}

void Normalizer::timeWindow(float seconds) {
  MovingStats::timeWindow(seconds);
}

float Normalizer::timeWindow() const { return MovingStats::timeWindow(); }

bool Normalizer::timeWindowIsInfinite() const {
  return (_timeWindow == MOVING_FILTER_INFINITE_TIME_WINDOW);
}

void Normalizer::reset() {
  MovingStats::reset();
  MovingFilter::reset();
}

float Normalizer::put(float value) {
  // Update only if started.
  _value = isStarted() ? MovingStats::update(value, sampleRate()) : normalize(value);

  // Normalize value to target normal.
  _value = _value * _targetStdDev + _targetMean;
  // Check for clamp.
  if (isClamped())
    _value = _clamp(_value);

  return _value;
}

float Normalizer::lowOutlierThreshold(float nStdDev) const {
  return targetMean() - abs(nStdDev) * targetStdDev();
}

float Normalizer::highOutlierThreshold(float nStdDev) const  {
  return targetMean() + abs(nStdDev) * targetStdDev();
}

bool Normalizer::isClamped() const {
  return (_clampStdDev != NORMALIZER_NO_CLAMP);
}

void Normalizer::clamp(float nStdDev) {
  _clampStdDev = abs(nStdDev);
}

void Normalizer::noClamp() {
  _clampStdDev = NORMALIZER_NO_CLAMP;
}

float Normalizer::_clamp(float value) const {
  float absStdDevOutlier = _clampStdDev * targetStdDev();
  return constrain(value, _targetMean - absStdDevOutlier, _targetMean + absStdDevOutlier);
}

}
