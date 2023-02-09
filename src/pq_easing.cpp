/*
 * pq_easing.cpp
 *
 * Easing functions.
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2022 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "pq_easing.h"
#include "pq_fastmath.h"
#include "pq_trig8.h"

namespace pq {

float easeNone(float t) { return t; }

float easeInSine(float t) {
	return fastSin( HALF_PI * t );
}

float easeOutSine(float t) {
    t--;
	return 1 + fastSin( HALF_PI * t );
}

float easeInOutSine(float t) {
	return 0.5f * (1 + fastSin( PI * (t - 0.5f) ) );
}

float easeInQuad(float t) {
    return t * t;
}

float easeOutQuad(float t) { 
    return t * (2 - t);
}

float easeInOutQuad(float t) {
    return t < 0.5f ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float easeInCubic(float t) {
    return t * t * t;
}

float easeOutCubic(float t) {
    t--;
    return 1 + t * t * t;
}

float easeInOutCubic(float t) {
    if (t < 0.5f)
        return  4 * t * t * t;
    else {
        t = 2*(t - 1);
        return 0.5f * t * t * t;
    }
}

float easeInQuart(float t) {
    t *= t;
    return t * t;
}

float easeOutQuart(float t) {
    t--;
    t *= t;
    return 1 - t * t;
}

float easeInOutQuart(float t) {
    if (t < 0.5f) {
        t *= t;
        return 8 * t * t;
    } else {
        t--;
        t *= t;
        return 1 - 8 * t * t;
    }
}

float easeInQuint(float t) {
    float t2 = t * t;
    return t * t2 * t2;
}

float easeOutQuint(float t) {
    t--;
    float t2 = t * t;
    return 1 + t * t2 * t2;
}

float easeInOutQuint(float t) {
    if (t < 0.5f) {
        float t2 = t * t;
        return 16 * t * t2 * t2;
    } else {
        t--;
        float t2 = t * t;
        return 1 + 16 * t * t2 * t2;
    }
}

float easeInExpo(float t) {
    return (fastPow( 2, 8 * t ) - 1) / 255;
}

float easeOutExpo(float t) {
    return 1 - fastPow( 2, -8 * t );
}

float easeInOutExpo(float t) {
    if( t < 0.5f ) {
        return (fastPow( 2, 16 * t ) - 1) / 510;
    } else {
        return 1 - 0.5f * fastPow( 2, -16 * (t - 0.5f) );
    }
}

float easeInCirc(float t) {
    return 1 - fastSqrt( 1 - t );
}

float easeOutCirc(float t) {
    return fastSqrt( t );
}

float easeInOutCirc(float t) {
    if( t < 0.5f ) {
        return (1 - fastSqrt( 1 - 2 * t )) * 0.5f;
    } else {
        return (1 + fastSqrt( 2 * t - 1 )) * 0.5f;
    }
}

float easeInBack(float t) {
    return t * t * (2.70158f * t - 1.70158f);
}

float easeOutBack(float t) {
    t--;
    return 1 + t * t * (2.70158f * t + 1.70158f);
}

float easeInOutBack(float t) {
    if( t < 0.5f ) {
        return t * t * (7 * t - 2.5f) * 2;
    } else {
        t--;
        return 1 + t * t * 2 * (7 * t + 2.5f);
    }
}

float easeInElastic(float t) {
    float t2 = t * t;
    return t2 * t2 * fastSin( t * PI * 4.5f);
}

float easeOutElastic(float t) {
    float t2 = (t - 1);
    t2 *= t2;
    return 1 - t2 * t2 * fastCos( t * PI * 4.5f );
}

float easeInOutElastic(float t) {
    if( t < 0.45f ) {
        float t2 = t * t;
        return 8 * t2 * t2 * fastSin( t * PI * 9 );
    } else if( t < 0.55f ) {
        return 0.5f + 0.75f * fastSin( t * PI * 4 );
    } else {
        float t2 = (t - 1);
        t2 *= t2;
        return 1 - 8 * t2 * t2 * fastSin( t * PI * 9 );
    }
}

float easeInBounce(float t) {
    return fastPow( 2, 6 * (t - 1) ) * abs( fastSin( t * PI * 3.5f ) );
}

float easeOutBounce(float t) {
    return 1 - fastPow( 2, -6 * t ) * abs( fastSin( t * PI * 3.5 ) );
}

float easeInOutBounce(float t) {
    if( t < 0.5f ) {
        return 8 * fastPow( 2, 8 * (t - 1) ) * abs( fastSin( t * PI * 7 ) );
    } else {
        return 1 - 8 * fastPow( 2, -8 * t ) * abs( fastSin( t * PI * 7 ) );
    }
}

}