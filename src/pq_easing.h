/*
 * pq_easing.h
 *
 * Easing functions.
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2022 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * Source: https://github.com/nicolausYes/easing-functions/blob/master/src/easing.cpp
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

#ifndef PQ_EASING_H_
#define PQ_EASING_H_

namespace pq {

typedef float (*easing_function)(float);

/// Linear easing function f(t) = t.
inline float easeNone(float t) { return t; }

float easeInSine(float t);

float easeOutSine(float t);

float easeInOutSine(float t);

float easeInQuad(float t);

float easeOutQuad(float t);

float easeInOutQuad(float t);

float easeInCubic(float t);

float easeOutCubic(float t);

float easeInOutCubic(float t);

float easeInQuart(float t);

float easeOutQuart(float t);

float easeInOutQuart(float t);

float easeInQuint(float t);

float easeOutQuint(float t);

float easeInOutQuint(float t);

float easeInExpo(float t);

float easeOutExpo(float t);

float easeInOutExpo(float t);

float easeInCirc(float t);

float easeOutCirc(float t);

float easeInOutCirc(float t);

float easeInBack(float t);

float easeOutBack(float t);

float easeInOutBack(float t);

float easeInElastic(float t);

float easeOutElastic(float t);

float easeInOutElastic(float t);

float easeInBounce(float t);

float easeOutBounce(float t);

float easeInOutBounce(float t);

}

#endif
