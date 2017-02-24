/*
 * pq_random.h
 *
 * Provides methods for generating uniform random numbers.
 *
 * (c) 2008 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This class was adapted from the files Random.{h,cc} in the Torch
 * library (http://torch.ch)
 * Copyright (C) 2003--2004 Ronan Collobert (collober@idiap.ch)
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
#ifndef RANDOM_H_
#define RANDOM_H_

#include <limits.h>
#include <stdlib.h>

// Floating point random numbers.

/// Generates a uniform random number on [0,1[.
float randomUniform();

/// Generates a uniform random number on [0,h[.
float randomUniform(float h);

/// Generates a uniform random number on [a,b[ (b>a).
float randomUniform(float a, float b);

#endif
