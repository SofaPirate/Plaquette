/*
 * AbstractField.h
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

#ifndef ABTRACT_FIELD_H_
#define ABTRACT_FIELD_H_

#include "PqCore.h"

namespace pq {

class AbstractField : public Unit {
public:
  AbstractField() {}
  virtual ~AbstractField() {}

public:
  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float at(float proportion) = 0;

  /**
   * Fills an array with values from this field.
   * @param array the array to read into
   * @param size the size of the array
   */
  template <typename T>
  void populate(T* array, size_t size) {
    if (size) {
      float step = 1.0f / size;
      float p = step * 0.5f; // start in the middle of
      for (size_t i = 0; i < size; i++, p += step) {
        _write(array[i], at(p));
      }
    }
  }

private:
  // Write a float value into a Chainable type.
  template <typename T>
  inline void _write(T& dst, float src) { src >> dst; }

  // Write a float value to another float.
  inline void _write(float& dst, float src) { dst = src; }
};

};
#endif
