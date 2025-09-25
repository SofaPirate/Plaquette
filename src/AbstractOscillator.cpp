/*
 * AbstractOscillator.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "AbstractOscillator.h"
#include "pq_random32.h"
#include "pq_map.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

AbstractOscillator::AbstractOscillator(float period_)
: Timeable(),
  _period(period_+1), // makes sure period is different and will be adjusted
#if PQ_OPTIMIZE_FOR_CPU
  _frequency(FLT_MAX),
#endif
  _phaseShiftOrRandomFrequencyRatio(0),
  _overflowed(false), _isRunning(false), _isForward(true), _valueNeedsUpdate(true), _randomness(0) {
  period(period_);
}

#define RANDOM_MAX_PERIOD_RATIO 32.0f
constexpr float RANDOM_MIN_PERIOD_RATIO = 1.0f / RANDOM_MAX_PERIOD_RATIO;

void AbstractOscillator::_randomPickNext() {
  // Pick random period ratio for this interval using Poisson distribution.
  const float u = max(randomFloat(), FLT_MIN); // (0,1]
  // Precise –ln(u). If log1pf exists it’s slightly better for tiny arguments.
#if defined(log1pf)
  float periodRatio = -log1pf(u - 1.0f);
#else
  float periodRatio = -logf(u);
#endif

  // clamping to avoid extreme ratios (rare outliers).
  periodRatio = constrain(periodRatio, RANDOM_MIN_PERIOD_RATIO, RANDOM_MAX_PERIOD_RATIO);

  // Apply random mixing factor between periodRatio and 1.0
  // = 1 / ( (1-r) * 1 + r x ratio )
  _phaseShiftOrRandomFrequencyRatio = 1.0f / (1 + randomness() * (periodRatio - 1));
}

#define RANDOMNESS_MAX 15
constexpr float INV_RANDOMNESS_MAX = 1.0f / RANDOMNESS_MAX;
float AbstractOscillator::randomness() const {
  return fixedToFloatInv(_randomness, INV_RANDOMNESS_MAX);
}

void AbstractOscillator::randomize(float randomness) {
  bool wasRandom = (_randomness != 0);
  _randomness = floatToFixed(randomness, RANDOMNESS_MAX);

  // If randomness is larger than zero (even slightly), set random level to at least 1.
  if (randomness > 0)
    _randomness = max(_randomness, 1);

  // If we are switching mode, reset phase-shift / frequency ratio to zero.
  if (wasRandom ^ (_randomness != 0))
    _phaseShiftOrRandomFrequencyRatio = 0;
}

void AbstractOscillator::_stepPhase(float deltaTimeSecondsTimesFixed32Max) {

  if (!isRunning()) {
    _overflowed = false;
  }

  else if (!_randomness) {
    // Deterministic path (unchanged)
    _overflowed = phase32UpdateFixed32(_phase32, frequency(),
                                       deltaTimeSecondsTimesFixed32Max, _isForward);
  }

  // Running scheduled random path.
  else {

      // if (_randScaleQ12 == 0) _randomPickNext();

    // const float k = (float)_randScaleQ12 * (1.0f / 1024.0f);
    // const float feff = frequency() * k;

    // _overflowed = phase32UpdateFixed32(_phase32, _randomFrequencyMultiplier,
    //                                    deltaTimeSecondsTimesFixed32Max, _isForward);

    // if (_overflowed) _randomPickNext();

    // --- Stochastic "speed-driven" path ---
    // Ensure we have an effective frequency for the current interval
  //  _randomFrequencyMultiplier = (float)_randScaleQ12 * KINV;
    if (!_phaseShiftOrRandomFrequencyRatio) {
      _randomPickNext();
    }

    // Advance phase using the instantaneous frequency for THIS interval
    _overflowed = phase32UpdateFixed32(_phase32, _phaseShiftOrRandomFrequencyRatio*frequency(),
                                       deltaTimeSecondsTimesFixed32Max, _isForward);

    // Overflowed: schedule next.
    if (_overflowed) {
      _phaseShiftOrRandomFrequencyRatio = 0.0f;
    }
  }
}


void AbstractOscillator::period(float period) {
  // Assign period.
  if (_period != period) {
    _period = max(period, 0.0f); // Make sure period is positive.

#if PQ_OPTIMIZE_FOR_CPU
    // Assign frequency.
    _frequency = periodToFrequency(_period);
#endif
  }
}

void AbstractOscillator::frequency(float frequency) {
#if PQ_OPTIMIZE_FOR_CPU
  // Assign period.
  if (_frequency != frequency) {
    _frequency = max(frequency, 0.0f); // Make sure frequency is positive.

    // Assign period.
    _period = frequencyToPeriod(_frequency);
  }
#else
  period( frequencyToPeriod(frequency) );
#endif
}

void AbstractOscillator::bpm(float bpm) {
  frequency(bpm * BPM_TO_HZ);
}

void AbstractOscillator::phase(float phase) {
  _setPhase32(floatToPhase32(phase));
}

void AbstractOscillator::phaseShift(float phaseShift) {
  if (!_randomness && _phaseShiftOrRandomFrequencyRatio != phaseShift) {
    // Need to readjust phase time.
    _setPhase32(phase32AddPhase(_phase32, _phaseShiftOrRandomFrequencyRatio - phaseShift));
    _phaseShiftOrRandomFrequencyRatio = phaseShift;
  }
}

float AbstractOscillator::phaseShift() const {
  return (_randomness ? 0 : _phaseShiftOrRandomFrequencyRatio);
}

float AbstractOscillator::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

void AbstractOscillator::setTime(float time) {
  if (!_randomness) {
    // Reset phase time to beginning.
    _setPhase32( phase32AddTime(_phaseShiftOrRandomFrequencyRatio, _period, time) );
  }
}

void AbstractOscillator::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (!_randomness && time > 0)
    _setPhase32( phase32AddTime(_phase32, _period, time) );
}

void AbstractOscillator::_setPhase32(q0_32u_t phase32) {
  _phase32 = phase32;
  _valueNeedsUpdate = true;
}

void AbstractOscillator::_setRunning(bool isRunning)
{
  _isRunning = isRunning;
}

}
