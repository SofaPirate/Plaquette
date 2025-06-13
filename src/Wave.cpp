/*
 * Wave.cpp
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

#include "Wave.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"
#include "pq_fixed_trig.h"
// #include <FastLED.h>

namespace pq
{

    fixed_t _PqSquareWave(fixed_t t, fixed_t width)
    {
        return (t <= width) ? FIXED_MAX : 0; // XXX small trick here: we set _onValue at the same time to deal with state changes
    }

    fixed_t _PqTriangleWave(fixed_t t, fixed_t width)
    {
        return (t <= width) ? fixedDivide(t, width) : fixedDivide(FIXED_MAX - t, FIXED_MAX - width);
    }

    // Improved version of Sine Wave.
    fixed_t _PqSineWave(fixed_t t, fixed_t width)
    {
#if defined(PQ_ARCH_32BITS)
        // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
        fixed_t phaseTime;

        // Special case: width == 0.5 (default and most common). More efficient.
        if (width == HALF_FIXED_MAX)
        {
            phaseTime = t;
        }
        // Rising part of sine wave.
        else if (t < width)
        {
            phaseTime = fixedDivide(t, width) / 2;
        }
        // Falling part of sine wave.
        else
        {
            phaseTime = fixedDivide(t - width, FIXED_MAX - width) / 2 + HALF_FIXED_MAX;
        }
        // Serial.print(t); Serial.print(" ");
        // Serial.println(phaseTime);
        // // Peak of sine wave.
        // else { // t == _width
        //   phaseTime = FIXED_MAX / 2;
        // }

        return static_cast<uint32_t>(HALF_FIXED_MAX - cos32(phaseTime));
#else
        // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
        uint16_t phaseTime16;

        // Special case: width == 0.5 (default and most common). More efficient.
        if (width == HALF_FIXED_MAX)
        {
            phaseTime16 = static_cast<uint16_t>(t >> 16);
        }
        // Rising part of sine wave.
        else if (t < width)
        {
            phaseTime16 = static_cast<uint16_t>((static_cast<uint64_t>(t) << 15) / width);
        }
        // Falling part of sine wave.
        else if (t > width)
        {
            phaseTime16 = static_cast<uint16_t>((static_cast<uint64_t>(t - width) << 15) / (FIXED_MAX - width)) + 32768;
        }
        // Peak of sine wave.
        else
        { // t == _width
            phaseTime16 = 32768;
        }

        // Convert to [0, 1] with wave shape similar to triangle wave.
        return static_cast<uint32_t>(static_cast<uint16_t>(32767) - cos16(phaseTime16)) << 16;
#endif
    }

    fixed_t Wave::_getFixed(fixed_t t)
    {
        switch (_shape)
        {
        case Shape::Sine:
            return _PqSineWave(t, _width);
            break;

        case Shape::Square:
            return _PqSquareWave(t, _width);
            break;

        case Shape::Triangle:
            return _PqTriangleWave(t, _width);
            break;

        case Shape::Ramp:
            // TODO width should control the curve
            return t;
            break;

        case Shape::Random:

            if (_overflowed)
            {
                _target = (float)random(1000000) / (float)1000000 ;
            }
            _current = (_target-_current)* 0.001 + _current;
            return floatTofixed(_current);//HALF_FIXED_MAX; //t);
            break;

        default: // SHOULD NOT BE POSSIBLE
            return _PqSineWave(t, _width);
            break;
        };
    }

    Wave::Wave(Shape shape, Engine &engine) : AbstractWave(engine), _shape(shape) {};
    Wave::Wave(Shape shape, float period, Engine &engine) : AbstractWave(period, engine), _shape(shape) {};
    Wave::Wave(Shape shape, float period, float width, Engine &engine) : AbstractWave(period, width, engine), _shape(shape) {};

}
