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

#define NORMALIZER_DEFAULT_MEAN   0.5f
#define NORMALIZER_DEFAULT_STDDEV 0.15f

Normalizer::Normalizer()
: AnalogSource(NORMALIZER_DEFAULT_MEAN),
  MovingStats(),
  _targetMean(NORMALIZER_DEFAULT_MEAN),
  _targetStdDev(NORMALIZER_DEFAULT_STDDEV)
{
}

Normalizer::Normalizer(float timeWindow)
: AnalogSource(NORMALIZER_DEFAULT_MEAN),
  MovingStats(timeWindow),
  _targetMean(NORMALIZER_DEFAULT_MEAN),
  _targetStdDev(NORMALIZER_DEFAULT_STDDEV)
{
}

Normalizer::Normalizer(float mean, float stddev)
	: AnalogSource(mean),
    MovingStats(),
    _targetMean(mean),
    _targetStdDev(abs(stddev))
{
}

Normalizer::Normalizer(float mean, float stddev, float timeWindow)
	: AnalogSource(mean),
    MovingStats(timeWindow),
    _targetMean(mean),
    _targetStdDev(abs(stddev))
{
}

void Normalizer::infiniteTimeWindow() {
  MovingStats::infiniteTimeWindow();
}

void Normalizer::timeWindow(float seconds) {
  MovingStats::timeWindow(seconds);
}

float Normalizer::timeWindow() const { return MovingStats::timeWindow(); }

float Normalizer::put(float value) {
  return (_value = MovingStats::update(value, sampleRate()) * _targetStdDev + _targetMean);
}

}
