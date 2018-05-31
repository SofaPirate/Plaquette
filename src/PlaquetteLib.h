/*
 * PlaquetteLib.h
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

 #ifndef PLAQUETTE_LIB_H_
 #define PLAQUETTE_LIB_H_

 #if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
 #else
 #include "WProgram.h"
 #endif

 // Plaquette builtin functions.
 #include "pq_lerp.h"
 #include "pq_map_real.h"
 #include "pq_print.h"
 #include "pq_random.h"
 #include "pq_time.h"

 // Core files.
 #include "PqCore.h"
 #include "PqInputs.h"
 #include "PqOutputs.h"

 // Basic set of extra components.
 #include "PqExtra.h"

 #endif
