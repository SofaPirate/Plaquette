/*
 * Plotter.cpp
 *
 * (c) 2026 Sofian Audry
 *
 * Allocation-free Plotter implementation for Plaquette.
 */

#include "Plotter.h"

namespace pq {

static PlotterFormat defaultPlotterFormat() {
  // Default: Arduino Serial Plotter friendly: space-separated values, newline rows.
  PlotterFormat fmt;
  fmt.row = RowFormat("", "\r\n", ",", "{value}", false);

  // Header uses same separator by default; disabled unless enabled by user.
  fmt.header = RowFormat("", "\r\n", " ", "{label}", false);
  fmt.useHeader = false;

  return fmt;
}

Plotter::Plotter(unsigned long baudRate, const char* labels, Engine& engine)
  : Unit(engine)
{
  _out = &Serial;
  _isSerial = true;
  _baudRate = baudRate;

  _format = defaultPlotterFormat();
  _labels = labels;
}

Plotter::Plotter(SerialType& serial, unsigned long baudRate,
                 const char* labels, Engine& engine)
  : Unit(engine)
{
  _out = &serial;
  _isSerial = true;
  _baudRate = baudRate;

  _format = defaultPlotterFormat();
  _labels = labels;
}

Plotter::Plotter(Print& out, const char* labels, Engine& engine)
  : Unit(engine)
{
  _out = &out;
  _isSerial = false;
  _baudRate = 0;

  _format = defaultPlotterFormat();
  _labels = labels;
}

void Plotter::begin() {
  if (_begun) return;
  if (!_out) { _out = &Serial; _isSerial = true; }

  if (_isSerial) {
    // Caller guarantees _out points to a PlaquetteSerialType if _isSerial == true.
    SerialType* serial = static_cast<SerialType*>(_out);

    serial->flush();
    if (_baudRate > 0) serial->begin(_baudRate);
    while (serial->available()) serial->read();
  }

  _begun = true;
}

void Plotter::step() {
  endRow();
}

void Plotter::format(const PlotterFormat& fmt) {
  _format = fmt;
}

PlotterFormat& Plotter::format() {
  return _format;
}

void Plotter::precision(uint8_t digits) {
  _digits = digits;
}

uint8_t Plotter::precision() const {
  return _digits;
}

void Plotter::labels(const char* labels_) {
  _labels = labels_;
  _headerPrinted = false;  // schema change => allow header again
}

const char* Plotter::labels() const {
  return _labels;
}

void Plotter::headerPrinted(bool printed) {
  _headerPrinted = printed;
}

bool Plotter::headerPrinted() const {
  return _headerPrinted;
}

bool Plotter::rowDirty() const {
  return _rowDirty;
}

float Plotter::put(float value) {
  _ensureHeader();
  _openRowIfNeeded();

  // Separator policy is handled by RowFormat::elementOut; we must know isLast.
  // In streaming/append mode, we do NOT know the last element at put-time.
  // Therefore we treat put() as "not last", and endRow() will print the row end.
  // To preserve correct "between elements" separators, RowFormat should emit sep
  // BEFORE elements (i>0) or we stream with internal rule.
  //
  // For v1 we implement "separator-before" ourselves, then call elementOut
  // with isLast=true (so it doesn't emit sep). This avoids needing lookahead.
  //
  // Concretely: we print separator if valueIndex > 0, then print the element.

  if (_valueIndex > 0) {
    _format.row.sep(*_out);
  }

  const LabelView lab = _labelAt(_valueIndex);
  // elementOut with isLast=true so it does not print separators
  _format.row.elementOut(*_out, _valueIndex, lab, value, _digits, /*isLast=*/true);

  _rowDirty = true;
  _valueIndex++;

  return value;
}

void Plotter::endRow() {
  if (!_rowDirty) {
    // Do not emit an empty line by default.
    clearRow();
    return;
  }
  _format.row.endRow(*_out);
  clearRow();
}

void Plotter::endRowForce() {
  _format.row.endRow(*_out);
  clearRow();
}

void Plotter::clearRow() {
  _rowOpen = false;
  _rowDirty = false;
  _valueIndex = 0;
}

void Plotter::_ensureHeader() {
  if (!_format.useHeader) return;
  if (_headerPrinted) return;
  if (!_labels || !*_labels) return;

  // Emit header once.
  _format.header.beginRow(*_out);

  // Print all labels found in schema (unknown count => iterate until empty).
  // We stop when we cannot find the next label.
  uint16_t i = 0;
  while (true) {
    const LabelView lab = _labelAt(i);
    if (lab.empty()) break;

    if (i > 0) _format.header.sep(*_out);
    _format.header.elementOut(*_out, i, lab, 0.0f, 0, /*isLast=*/true);
    i++;
  }

  _format.header.endRow(*_out);
  _headerPrinted = true;
}

void Plotter::_openRowIfNeeded() {
  if (_rowOpen) return;
  _format.row.beginRow(*_out);
  _rowOpen = true;
}

bool Plotter::_isDelim(char c) {
  return (c == ',' || c == ';' || c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

LabelView Plotter::_labelAt(uint16_t index) const {
  LabelView none;

  if (!_labels || !*_labels) return none;

  const char* s = _labels;

  // Skip leading delimiters
  while (*s && _isDelim(*s)) s++;

  uint16_t current = 0;

  while (*s) {
    // s points at start of a token
    const char* start = s;
    while (*s && !_isDelim(*s)) s++;
    const char* end = s;

    if (current == index) {
      LabelView lab;
      lab.ptr = start;
      lab.len = static_cast<uint16_t>(end - start);
      return lab;
    }

    // Skip delimiters to next token
    while (*s && _isDelim(*s)) s++;
    current++;
  }

  return none;
}

} // namespace pq
