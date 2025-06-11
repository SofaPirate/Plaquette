#ifndef TIME_MAP_H
#define TIME_MAP_H
/*
 * Map.h
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "PqCore.h"
#include "AbstractMap.h"

namespace pq
{
    template <size_t COUNT>
    class TimeMap : public AbstractMap
    {
    protected:
        int _buffer[COUNT];
        size_t _count;
        //size_t _maxIndex;
        size_t _index;
        size_t _previousIndex;
        unsigned long _chrono;   // microseconds
        unsigned long _interval; // microseconds
        bool _full;

        void _reset() {
            _previousIndex = 0;
            _full = false;
            _chrono = micros();
        }

    public:
        TimeMap(float period) : _count(COUNT), _full(false)
        {
            //_maxIndex = COUNT - 1;
            _interval = floor(period * 1000000.0); // convet to microseconds
            _reset();
        }

        float read(float percent) override
        {
            percent = constrain(percent, 0.0, 1.0);
            int index = floor(percent * (float)_maxIndex);
            return _buffer[index];
        }

        float triggered()
        {
            return _full;
        }

        void step() override
        {
            if ( _full ) reset();

            unsigned long duration = micros() - _chrono; // microseconds
            _index = (duration * COUNT / _interval);
            if ( _index >= COUNT ) {
                _full = true;
                _index = COUNT-1;
            }
        }

    protected:
        float _read() override
        {
            return _full;
        }

        float _write(float value) override
        {
            if (_full)
                return _full;

            // fill missing data
            if (_previousIndex < _index)
            {
                for (int i = _previousIndex + 1; i <= _index; ++i)
                {
                    _buffer[i] = value;
                }
                _previousIndex = _index;
            }
            else
            {
                _buffer[_index] = value;
            }
            

            return _full;
        }
    };

};
#endif