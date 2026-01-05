/*
 * PlaquetteSketch.h
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
#ifndef PLAQUETTE_SKETCH_H_
#define PLAQUETTE_SKETCH_H_

#include "PlaquetteLib.h"

// Declaration of base sketch functions.

// IMPORTANT: These functions are declared with __attribute__((weak)) in the source
// file. This allows the user to override them only optionnaly.
void settings();
void begin();
void step();

#endif
