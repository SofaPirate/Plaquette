/*
 * Plotter.cpp
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

#include "Plotter.h"
#include <math.h>

#define PLOTTER_PADDING_VALUE NAN

namespace pq {

Plotter::Plotter(unsigned long baudRate,
                 const char* labels,
                 Engine& engine)
: Plotter(PLAQUETTE_DEFAULT_SERIAL, baudRate, labels, engine)
{}

Plotter::Plotter(SerialType& serial,
                 unsigned long baudRate,
                 const char* labels,
                 Engine& engine)
: Plotter(static_cast<Print&>(serial), labels, engine)
{
  _serial = &serial;
  _baudRate = baudRate;
}

Plotter::Plotter(Print& out,
                 const char* labels,
                 Engine& engine)
: Unit(engine),
  _out(&out),
  _serial(nullptr),
  _baudRate(0),
  _labels(labels)
{
  _labelCount = _countLabels(_labels);
  format(PLOTTER_DEFAULT);
}

PlotterFormat Plotter::formatFromPreset(PlotterFormatPreset mode, const char* labelsSchema) {
  // Base/default format.
  static PlotterFormat baseTemplateFmt;

  const bool hasLabels = (labelsSchema && *labelsSchema);

  // Copy base format as a starting point.
  PlotterFormat fmt = baseTemplateFmt;

  switch (mode) {
    case PLOTTER_DEFAULT:
      fmt.valueTemplate = hasLabels ? "$k:$v" : "$v";
      break;

    case PLOTTER_CSV:
      fmt.headerEnabled = hasLabels;  // header only when labels exist
      break;

    case PLOTTER_JSON:
      fmt.plotBegin = "[\r\n";
      fmt.trailingRowEnd = false;
      // JSON object.
      if (hasLabels) {
        fmt.plotEnd = "}\r\n]";
        fmt.rowBegin = "{";
        fmt.rowEnd = "},\r\n";
        fmt.valueTemplate = "\"$k\":$v";
      }
      // JSON array.
      else {
        fmt.plotEnd = "]\r\n]";
        fmt.rowBegin = "[";
        fmt.rowEnd = "],\r\n";
        fmt.valueTemplate = "$v";
      }
      break;
  }

  return fmt;
}

void Plotter::format(PlotterFormatPreset preset) {
  format(formatFromPreset(preset, _labels));
  _format.trailingSeparator = false;
}

void Plotter::format(PlotterFormat format) {
  _format = format;
}

void Plotter::beginPlot() {
  _headerPrinted = false;
  _rowOpen = false;
  _valueIndex = 0;

  _plotOpen = true;

  _scheduleBeginPlot = true;
}

void Plotter::endPlot() {
  if (!_plotOpen) return;

  // End-of-engine-step finalization.
  // if (_format.trailingRowEnd)
    // _closeRowIfOpen();
  _closeRowIfOpen(_format.trailingRowEnd);

  _format.endPlot(*_out);
  _plotOpen = false;
}

// --- Unit lifecycle ---

void Plotter::begin() {
  // Safely start serial.
  if (_serial && _baudRate > 0) {
    beginSerial(*_serial, _baudRate);
  }

  beginPlot();
}

void Plotter::step() {
  if (_plotOpen) {
    if (_scheduleBeginPlot) {
      _format.beginPlot(*_out);
      _scheduleBeginPlot = false;
    }

    if (_format.trailingRowEnd) {
      _closeRowIfOpen(_format.trailingRowEnd);
    }
    else {
      _scheduleEndRow = true;
    }
  }
    // _endRow();
}


// --- Streaming values ---

float Plotter::put(float value) {
  if (!_out || !_plotOpen) return value;

  if (_scheduleEndRow) {
    _closeRowIfOpen(true);
    _scheduleEndRow = false;
  }

  _lastValue = value;

  _ensureHeader();
  _openRowIfNeeded();

  _printValue(value);

  return value;
}

void Plotter::_ensureHeader() {
  if (_headerPrinted) return;
  if (!_format.headerEnabled) return;
  if (!_labels || !*_labels) return;
  if (_labelCount == 0) return;
  if (!_out) return;

  _format.beginRow(*_out);

  for (uint16_t i = 0; i < _labelCount; ++i) {
    if (i > 0) {
      _format.sep(*_out);
    }

    const LabelView label = _labelAt(i);

    // Same trick: isLast=true so PlotterFormat does not add separators.
    _format.printKeyElement(*_out,
                            i,
                            label,
                            _digits,
                            /*isLast=*/true);
  }

  _format.endRow(*_out);
  _headerPrinted = true;
}

void Plotter::_openRowIfNeeded() {
  if (_rowOpen) return;
  if (!_out) return;

  _format.beginRow(*_out);

  _rowOpen = true;
  _valueIndex = 0;
}

void Plotter::_closeRowIfOpen(bool printEndRow) {
  if (!_rowOpen || !_out) return;

  // Pad with values.
  while (_valueIndex < _labelCount)
    _printValue(PLOTTER_PADDING_VALUE);

  if (printEndRow)
    _format.endRow(*_out);

  _rowOpen = false;
  _valueIndex = 0;
}

void Plotter::_printValue(float value) {
  const uint16_t idx = _valueIndex;

  // Print if:
  // - headers are not enabled
  // - there are no labels
  // - index is within label counts
  if (!_format.headerEnabled || !_labelCount || idx < _labelCount) {
    // Render element immediately.
    const LabelView label = _labelAt(idx);

    // We pass isLast=true to prevent PlotterFormat from printing separators;
    // separator policy is handled here (prefix).
    _format.printValueElement(*_out,
                              idx,
                              label,
                              value,
                              _digits,
                              idx == 0);
  _valueIndex++;
  }
}

  // --- Label parsing (comma-separated schema) ---

bool Plotter::_isSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

LabelView Plotter::_trimSpaces(const char* start, const char* end) {
  while (start < end && _isSpace(*start)) ++start;
  while (end > start && _isSpace(*(end - 1))) --end;

  LabelView v;
  v.ptr = start;
  v.len = static_cast<uint16_t>(end - start);
  return v;
}

uint16_t Plotter::_countLabels(const char* schema) const {
  if (!schema || !*schema) return 0;

  uint16_t count = 0;
  const char* p = schema;

  while (*p) {
    const char* tokStart = p;
    while (*p && *p != ',') ++p;
    const char* tokEnd = p;

    LabelView v = _trimSpaces(tokStart, tokEnd);
    if (!v.empty()) ++count;

    if (*p == ',') ++p;
  }

  return count;
}

LabelView Plotter::_labelAt(uint16_t index) const {
  LabelView none;
  if (!_labels || !*_labels) return none;

  uint16_t tokenIndex = 0;
  const char* p = _labels;

  while (*p) {
    const char* tokStart = p;
    while (*p && *p != ',') ++p;
    const char* tokEnd = p;

    LabelView v = _trimSpaces(tokStart, tokEnd);
    if (!v.empty()) {
      if (tokenIndex == index) return v;
      tokenIndex++;
    }

    if (*p == ',') ++p;
  }

  return none;
}

} // namespace pq
