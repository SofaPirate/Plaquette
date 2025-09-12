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
#include "pq_map.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {


  // 32-bit uniform in (0,1]; avoid 0 exactly.
static inline float uniform01_open32() {
  // Build 32 random bits even if rand() is only 15–16 bits.
  // uint32_t r = ((uint32_t)rand() << 16) ^ (uint32_t)rand();
  uint32_t r = random32();
  r |= 1u;                            // ensure nonzero => U > 0
  constexpr float TWO_NEG_32 = 1.0f / 4294967296.0f;  // 2^-32
  return (r + 1u) * TWO_NEG_32;       // (0,1]
}

  // Precise: -ln(U) with protection against U=0
static inline float negLogU_precise() {
  // U ∈ (0,1]; avoid 0 exactly
  const float u = uniform01_open32(); // (0,1]
  // Prefer log1p for tiny arguments when available; fall back to logf.
  // -ln(u) = -log1p(u-1)
#if defined(__GNUC__) && !defined(ARDUINO_ARCH_AVR)
  return -log1pf(u - 1.0f);
#else
  return -logf(u);
#endif
}
// U ∈ (0,1]; fast approx of -ln(U). Good enough for scheduling.
static inline float fastNegLogU() {
  const float u = (rand() + 1.0f) / (RAND_MAX + 1.0f);
  union { float f; uint32_t i; } v{u};
  const int e = int((v.i >> 23) & 0xFF) - 127;
  v.i = (v.i & 0x7FFFFF) | (127u << 23);   // m∈[1,2)
  const float m = v.f * 0.5f;              // m∈[0.5,1)
  const float t = 1.0f - m;
  const float t2 = t*t, t3 = t2*t;
  const float neglnm = t + 0.346607f*t2 + 0.239282f*t3; // small fit
  const float LN2 = 0.69314718056f;
  return neglnm - e * LN2;                 // ≈ -ln(u)
}

// Draw X ~ Exp(1) truncated to [0, K] cycles (K = 8 or 16).
// Avoids exp/log by clamping the cheap draw.
static inline float sampleTruncatedExpCycles(float K) {
  float x = negLogU_precise();
  if (x > K) x = K;        // truncate tail; P[X>K]=e^{-K}
  return x;
}

AbstractOscillator::AbstractOscillator(float period_)
: Timeable(),
  _period(period_+1), // makes sure period is different and will be adjusted
#if PQ_OPTIMIZE_FOR_CPU
  _frequency(FLT_MAX),
#endif
  _phaseShift(0),
  _overflowed(false), _isRunning(false), _isForward(true), _valueNeedsUpdate(true), _isRandom(true) {
  period(period_);
  setRandom(false);
}

void AbstractOscillator::start() {
  Timeable::start();
  _overflowed = false;
}

#define K 32.0f
#define KMULT (65535.0f/K)
#define KINV (K/65535.0f)
void AbstractOscillator::_randomPickNext() {
  const float lam = frequency();            // nominal rate (Hz)

  if (lam <= 0.0f) { _randEffHz = 0.0f; return; }

  const float u = uniform01_open32();
  // Precise –ln(u). If log1pf exists it’s slightly better for tiny arguments.
#if defined(log1pf)
  const float negLogU = -log1pf(u - 1.0f);
#else
  const float negLogU = -logf(u);
#endif

  float k = 1.0f / negLogU;    // k = 1 / ( -ln u )
//  Serial.println(k);

  // Clamp tails to keep speeds within reasonable bounds:
  k = constrain(k, 1.0f/K, K);

//  _randScaleQ12 = (uint16_t)(k * KMULT);

  // Piecewise-constant instantaneous frequency for this interval
  _randEffHz = 1 / negLogU;
}

void AbstractOscillator::setRandom(bool isRandom) {
  if (_isRandom != isRandom) {
    _isRandom = isRandom;

    if (_isRandom)
      _randEffHz = 0;
  }
}

void AbstractOscillator::_stepPhase(float deltaTimeSecondsTimesFixed32Max) {

  if (!isRunning()) {
    _overflowed = false;
  }

  else if (!isRandom()) {
    // Deterministic path (unchanged)
    _overflowed = phase32UpdateFixed32(_phase32, frequency(),
                                       deltaTimeSecondsTimesFixed32Max, _isForward);
  }

  // Running scheduled random path.
  else {
  // Adjust phase time.
  // NOTE (optimization trick): If isRunning() is false, phase32UpdateFixed32() will not be called.
  _overflowed = (isRunning() &&
                 phase32UpdateFixed32(_phase32, frequency(), deltaTimeSecondsTimesFixed32Max, _isForward));
}

    // --- Stochastic "speed-driven" path ---
    // Ensure we have an effective frequency for the current interval
  //  _randEffHz = (float)_randScaleQ12 * KINV;
    if (!_randEffHz) {
      _randomPickNext();
    }

    // Advance phase using the instantaneous frequency for THIS interval
    _overflowed = phase32UpdateFixed32(_phase32, _randEffHz*frequency(),
                                       deltaTimeSecondsTimesFixed32Max, _isForward);

    // Overflowed: schedule next.
    if (_overflowed) {
      _randEffHz = 0.0f;
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
  if (_phaseShift != phaseShift) {
    // Need to readjust phase time.
    _setPhase32(phase32AddPhase(_phase32, _phaseShift - phaseShift));
    _phaseShift = phaseShift;
  }
}

float AbstractOscillator::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

void AbstractOscillator::setTime(float time) {
  // Reset phase time to beginning.
  _setPhase32( phase32AddTime(_phaseShift, _period, time) );
}

void AbstractOscillator::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (time > 0)
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
