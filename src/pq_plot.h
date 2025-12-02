/*
 * pq_plot.h
 *
 * Print functions for Plaquette to use with the Serial plotter.
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

#ifndef PQ_PLOT_H_
#define PQ_PLOT_H_

#include "pq_print.h"

#ifndef PLOT_TOKEN_SIZE
#define PLOT_TOKEN_SIZE 4
#endif

namespace pq {

enum PlotToken {
  LABEL = 0,
  ITEM = 1,
  BEGIN = 2,
  END = 3
};

/// Sets the Token used for a given plot boundary.
void plotToken(PlotToken type, const char* token);

/// Returns the Token for the given plot boundary.
const char* plotToken(PlotToken type);

/// Sets the numeric precision for printed values.
void plotPrecision(int digits);

/// Returns the current numeric precision.
int plotPrecision();

/// Prints an unlabeled value in the current plot frame.
size_t plot(double value);

/// Prints a labeled value (C-string label).
size_t plot(const char label[], double value);

/// Prints a labeled value stored in program memory.
size_t plot(const __FlashStringHelper * label, double value);

/// Prints a labeled value using an Arduino String.
size_t plot(const String &label, double value);

/// Ends and flushes the current plot frame (newline).
size_t plotln();

} // namespace pq

#endif
