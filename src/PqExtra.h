/*
 * PqExtra.h
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

#ifndef PQ_EXTRA_H_
#define PQ_EXTRA_H_

// General.
#include "PqCore.h"
#include "MovingAverage.h"
#include "MovingStats.h"

// Filters.
//#include "Thresholder.h"

// Normalization.
#include "MinMaxScaler.h"
#include "Normalizer.h"
#include "Smoother.h"

// Stream.
#include "StreamIn.h"
#include "StreamOut.h"
#include "OscilloscopeOut.h"

// Timing.
#include "Chrono.h"
#include "Metro.h"
#include "Timer.h"

// Generators.
#include "Ramp.h"
#include "SineOsc.h"
#include "SquareOsc.h"
#include "TriOsc.h"

#endif
