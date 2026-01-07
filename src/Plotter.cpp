#include "Plotter.h"

namespace pq {

static PlotterFormat makePlotterFormat(PlotterMode mode, const char* labelsSchema) {
  static PlotterFormat baseTemplateFmt;

  const bool hasLabels = (labelsSchema != nullptr && *labelsSchema != '\0');

  PlotterFormat fmt = baseTemplateFmt; // copy base format

  switch (mode) {
    case PLOTTER_DEFAULT:
      fmt.valueTemplate = hasLabels ? "$k:$v" : "$v";
      break;

    case PLOTTER_CSV:
      fmt.headerEnabled = hasLabels;  // header only when labels exist
      break;

    case PLOTTER_JSON:
      if (hasLabels) {
        fmt.rowBegin = "{";
        fmt.rowEnd = "}\r\n";
        fmt.valueTemplate = "\"$k\":$v";
      } else {
        fmt.rowBegin = "[";
        fmt.rowEnd = "]\r\n";
        fmt.valueTemplate = "$v";
      }
      break;
  }

  return fmt;
}

// --- Constructors ---

Plotter::Plotter(unsigned long baudRate,
                 const char* labels,
                 PlotterMode mode,
                 Engine& engine)
: Unit(engine),
  _out(&Serial),
  _serial(&Serial),
  _baudRate(baudRate),
  _mode(mode),
  _labels(labels)
{
  _labelCount = _countLabels(_labels);
  _rebuildFormat();
}

Plotter::Plotter(SerialType& serial,
                 unsigned long baudRate,
                 const char* labels,
                 PlotterMode mode,
                 Engine& engine)
: Unit(engine),
  _out(&serial),
  _serial(&serial),
  _baudRate(baudRate),
  _mode(mode),
  _labels(labels)
{
  _labelCount = _countLabels(_labels);
  _rebuildFormat();
}

Plotter::Plotter(Print& out,
                 const char* labels,
                 PlotterMode mode,
                 Engine& engine)
: Unit(engine),
  _out(&out),
  _serial(nullptr),
  _baudRate(0),
  _mode(mode),
  _labels(labels)
{
  _labelCount = _countLabels(_labels);
  _rebuildFormat();
}

// --- Configuration ---

void Plotter::labels(const char* labelsSchema) {
  _labels = labelsSchema;
  _labelCount = _countLabels(_labels);

  _headerPrinted = false;
  _rebuildFormat();
}

void Plotter::mode(PlotterMode mode) {
  _mode = mode;

  _headerPrinted = false;
  _rebuildFormat();
}

void Plotter::endRow() {
  _closeRowIfOpen();
}

// --- Unit lifecycle ---

void Plotter::begin() {
  if (_serial && _baudRate > 0) {
    _serial->begin(_baudRate);
  }

  _headerPrinted = false;
  _rowOpen = false;
  _valueIndex = 0;
}

void Plotter::end() {
  // End-of-engine-step finalization.
  _closeRowIfOpen();
}

// --- Streaming values ---

float Plotter::put(float value) {
  if (!_out) return value;

  _lastValue = value;

  _ensureHeader();
  _openRowIfNeeded();

  const uint16_t idx = _valueIndex;

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
  return value;
}

// --- Internal helpers ---

void Plotter::_rebuildFormat() {
  _format = makePlotterFormat(_mode, _labels);

  // Safety: this Plotter implementation uses prefix separators and expects
  // PlotterFormat not to emit any separators on its own.
  _format.trailingSeparator = false;
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

void Plotter::_closeRowIfOpen() {
  if (!_rowOpen || !_out) return;

  _format.endRow(*_out);

  _rowOpen = false;
  _valueIndex = 0;
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
