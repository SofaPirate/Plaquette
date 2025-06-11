/*
 * pq_time.h
 *
 * Time functions for Plaquette.
 *
 * (c) 2016 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef PQ_TIME_H_
#define PQ_TIME_H_

#include <stdint.h>

namespace pq {

// Time constants.
#define MILLIS_PER_MICROS    1000
#define MICROS_PER_MILLIS    1000
#define MILLIS_PER_SECOND    1000
#define MICROS_PER_SECOND 1000000UL

#define SECONDS_PER_MINUTE     60
#define SECONDS_PER_HOUR     3600
#define SECONDS_PER_DAY     86400

#define MINUTES_PER_HOUR       60
#define HOURS_PER_DAY          24

#define MILLIS_TO_MICROS      1e3f
#define SECONDS_TO_MILLIS     1e3f
#define SECONDS_TO_MICROS     1e6f

#define MICROS_TO_MILLIS     1e-3f
#define MILLIS_TO_SECONDS    1e-3f
#define MICROS_TO_SECONDS    1e-6f

constexpr float BPM_TO_HZ = 1.0f / SECONDS_PER_MINUTE;

// Special type to store time in microseconds.
// Allows dealing with micros() timer overflow to represent time as a 64-bit value.
typedef union {
  // 64 bits version.
  uint64_t micros64;

  // 32 bits split version.
  struct {
    uint32_t base;
    uint32_t overflows;
  } micros32;
}
micro_seconds_t; // type name

/**
 * Returns time in seconds. Optional parameter allows to ask for reference time (default)
 * which will yield the same value through one iteration of step(), or "real" time which will
 * return the current total running time.
 * @param referenceTime determines whether the function returns the reference time or the real time
 * @return the time in seconds
 */
float seconds(bool referenceTime=true);

/**
 * Returns time in milliseconds. Optional parameter allows to ask for reference time (default)
 * which will yield the same value through one iteration of step(), or "real" time which will
 * return the current total running time.
 * @param referenceTime determines whether the function returns the reference time or the real time
 * @return the time in milliseconds
 */
uint32_t milliSeconds(bool referenceTime=true);

/**
 * Returns time in microseconds. Optional parameter allows to ask for reference time (default)
 * which will yield the same value through one iteration of step(), or "real" time which will
 * return the current total running time.
 * @param referenceTime determines whether the function returns the reference time or the real time
 * @return the time in microseconds
 */
uint64_t microSeconds(bool referenceTime=true);

/**
 * Converts microseconds to milliseconds.
 * @param micros microseconds
 * @return milliseconds
 */
inline uint32_t microsToMillis(uint64_t micros) { return micros / MICROS_PER_MILLIS; }

/**
 * Converts microseconds to seconds.
 * @param micros microseconds
 * @return seconds
 */
inline float microsToSeconds(uint64_t micros) { return micros * MICROS_TO_SECONDS; }

/**
 * Converts milliseconds to microseconds.
 * @param millis milliseconds
 * @return microseconds
 */
inline uint64_t millisToMicros(uint32_t millis) { return static_cast<uint64_t>(millis * MICROS_PER_MILLIS); }

/**
 * Converts milliseconds to seconds.
 * @param millis milliseconds
 * @return seconds
 */
inline float millisToSeconds(uint32_t millis) { return millis * MILLIS_TO_SECONDS; }

/**
 * Converts seconds to microseconds.
 * @param secs seconds
 * @return microseconds
 */
inline uint64_t secondsToMicros(float secs) { return static_cast<uint64_t>(secs * SECONDS_TO_MICROS); }

/**
 * Converts seconds to milliseconds.
 * @param secs seconds
 * @return milliseconds
 */
inline uint32_t secondsToMillis(float secs) { return static_cast<uint32_t>(secs * SECONDS_TO_MILLIS); }

/**
 * Converts minutes to seconds.
 * @param minutes number of minutes
 * @return seconds
 */
inline float minutesToSeconds(float minutes) { return minutes * SECONDS_PER_MINUTE; }

/**
 * Converts hours to seconds.
 * @param hours number of hours
 * @return seconds
 */
inline float hoursToSeconds(float hours) { return hours * SECONDS_PER_HOUR; }

/**
 * Converts days to seconds.
 * @param days number of days
 * @return seconds
 */
inline float daysToSeconds(float days) { return days * SECONDS_PER_DAY; }

} // namespace pq

#endif
