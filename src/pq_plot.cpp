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

#include "pq_plot.h"

namespace pq {

#define PLOT_SEPARATOR_SIZE 8

char separators [][PLOT_SEPARATOR_SIZE] = {":", ",", "", "\n"};
int precision = 2;

bool rowStarted = false;

size_t _checkStart() {
  if (!rowStarted) {
    rowStarted = true;
    return print(separators[BEGIN]);
  }
  else
    return print(separators[ITEM]);
}

size_t _plotValue(double value) {
  return print(value, precision);
}

size_t _postLabelPlotValue(double value) {
  size_t n = print(plotSeparator(LABEL));
  n += _plotValue(value);
  return n;
}

void plotSeparator(PlotSeparator type, const char *separator) {
  memcpy(separators[(int)type], separator, PLOT_SEPARATOR_SIZE);
}

const char* plotSeparator(PlotSeparator type) {
  return separators[(int)type];
}

void plotPrecision(int digits) {
  precision = digits;
}

int plotPrecision() {
  return precision;
}


size_t plot(double value)
{
  size_t n = _checkStart();
  n += _plotValue(value);
  return n;
}

size_t plot(const char label[], double value)
{
  size_t n = _checkStart();
  n += print(label);
  n += _postLabelPlotValue(value);
  return n;
}

size_t plot(const __FlashStringHelper *label,  double value) {
  size_t n = _checkStart();
  n += print(label);
  n += _postLabelPlotValue(value);
  return n;
}

size_t plot(const String &label, double value) {
  size_t n = _checkStart();
  n += print(label);
  n += _postLabelPlotValue(value);
  return n;
}

size_t plotln() {
  rowStarted = false;
  return print(plotSeparator(END));
}

}
