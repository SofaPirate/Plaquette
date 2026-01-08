#ifndef PQ_PLOTTER_H_
#define PQ_PLOTTER_H_

#include "PqCore.h"
#include "PlotterFormat.h"
#include "pq_serial.h"

namespace pq {

enum PlotterMode : uint8_t {
  PLOTTER_DEFAULT = 0,
  PLOTTER_CSV,
  PLOTTER_JSON
};

class Plotter : public Unit {
public:
  Plotter(unsigned long baudRate,
          const char* labels = nullptr,
          PlotterMode mode = PLOTTER_DEFAULT,
          Engine& engine = Engine::primary());

  Plotter(unsigned long baudRate,
          PlotterMode mode,
          Engine& engine = Engine::primary());

  Plotter(SerialType& serial,
          unsigned long baudRate,
          const char* labels = nullptr,
          PlotterMode mode = PLOTTER_DEFAULT,
          Engine& engine = Engine::primary());

  Plotter(SerialType& serial,
          unsigned long baudRate,
          PlotterMode mode,
          Engine& engine = Engine::primary());

  explicit Plotter(Print& out,
                   const char* labels = nullptr,
                   PlotterMode mode = PLOTTER_DEFAULT,
                   Engine& engine = Engine::primary());

  explicit Plotter(Print& out,
                   PlotterMode mode ,
                   Engine& engine = Engine::primary());

  void labels(const char* labelsSchema);
  const char* labels() const { return _labels; }

  void mode(PlotterMode mode);
  PlotterMode mode() const { return _mode; }

  void precision(uint8_t digits) { _digits = digits; }
  uint8_t precision() const { return _digits; }

  /// Returns reference to format so that it can be edited.
  PlotterFormat& format() { return _format; }

  // Unit overrides
  float put(float value) override;
  float get() override { return _lastValue; }

  /// Begins new plot.
  void beginPlot();

  /// Ends current plot.
  void endPlot();

protected:
  void begin() override;
  void step() override;

private:
  // Output backend
  Print* _out = nullptr;

  // If output is serial, we begin it in begin()
  SerialType* _serial = nullptr;
  unsigned long _baudRate = 0;

  // Mode & format
  PlotterMode _mode = PLOTTER_DEFAULT;
  PlotterFormat _format;

  // Labels schema (comma-separated)
  const char* _labels = nullptr;
  uint16_t _labelCount = 0;
  bool _headerPrinted = false;

  // Numeric precision
  uint8_t _digits = PLAQUETTE_PRINT_DEFAULT_DIGITS;

  // Row state
  bool _rowOpen = false;
  uint16_t _valueIndex = 0;
  float _lastValue = 0.0f;

  bool _plotOpen = true;
  bool _scheduleEndRow = false;

private:
  // Manually ends the current row (prints rowEnd if a row is open).
  void _endRow();

  // Internal helpers
  void _rebuildFormat();
  void _ensureHeader();
  void _openRowIfNeeded();
  void _closeRowIfOpen(bool printEndRow);

  void _printValue(float value);

  // Label parsing helpers (comma-separated schema)
  static bool _isSpace(char c);
  static LabelView _trimSpaces(const char* start, const char* end);

  uint16_t _countLabels(const char* schema) const;
  LabelView _labelAt(uint16_t index) const;
};

} // namespace pq

#endif // PQ_PLOTTER_H_
