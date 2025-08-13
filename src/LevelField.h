  /*
  * LevelField.h
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
#ifndef LEVEL_FIELD_H_
#define LEVEL_FIELD_H_

#include "AbstractField.h"

namespace pq {

class LevelField : public AbstractField
{
public:
  LevelField() {}
  virtual ~LevelField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float read(float proportion) override
  {
    proportion = constrain01(proportion);
    return (proportion <= _value) ? 1.0 : 0.0;
  }

protected:
  virtual float _read() override
  {
    return _value;
  }

  virtual float _write(float value) override {
    _value = constrain01(value);
    return _value;
  }

protected:
  float _value;
};

};
#endif
