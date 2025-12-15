/*
 * TimeWindowable.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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

 #include "TimeWindowable.h"

 namespace pq {

TimeWindowable::TimeWindowable() {
  infiniteTimeWindow();
}

TimeWindowable::TimeWindowable(float timeWindow_) {
  timeWindow(timeWindow_);
}

void TimeWindowable::infiniteTimeWindow() {
  _timeWindow = INFINITE_TIME_WINDOW;
}

void TimeWindowable::noTimeWindow() {
  _timeWindow = NO_TIME_WINDOW;
}

void TimeWindowable::timeWindow(float seconds) {
  if (seconds < 0)
    infiniteTimeWindow();
  else
    _timeWindow = seconds;
}

void TimeWindowable::cutoff(float hz) {
  if (hz <= 0)
    infiniteTimeWindow();
  else
    timeWindow(1.0f/hz);
}

float TimeWindowable::cutoff() const {
  return (timeWindowIsInfinite() ? 0 : 1.0f/timeWindow());
}

}
