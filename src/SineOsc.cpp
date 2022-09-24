/*
 * SineOsc.cpp
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

#include "SineOsc.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"
#include "pq_trig8.h"

namespace pq {

#define PQ_SINE_OSC_AMPLITUDE_DIVIDER (-32767.0f)

SineOsc::SineOsc(float period_) : Osc(period_) {}

void SineOsc::_updateValue() {
	_value = 0.5f + 0.5f * (cos16((uint16_t)(_phaseTime >> 16)) * _amplitude / PQ_SINE_OSC_AMPLITUDE_DIVIDER);
}

}
