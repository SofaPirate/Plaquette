/*
 * AdaptiveNormalizer.cpp
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
#include "AdaptiveNormalizer.h"

namespace pq {
  
AdaptiveNormalizer::AdaptiveNormalizer(float smoothWindow)
  : PqAnalogUnit(0.5f),
    MovingStats(smoothWindow),
    _targetMean(0.5f),
    _targetStddev(0.25f)
{
}

AdaptiveNormalizer::AdaptiveNormalizer(float mean, float stddev, float smoothWindow)
	: PqAnalogUnit(mean),
    MovingStats(smoothWindow),
    _targetMean(mean),
    _targetStddev(abs(stddev))
{
}

float AdaptiveNormalizer::put(float value) {
  return (_value = MovingStats::update(value, sampleRate()) * _targetStddev + _targetMean);
}

}
