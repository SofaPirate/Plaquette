#ifndef PQ_PLOTTER_H_
#define PQ_PLOTTER_H_

#include "Monitor.h"
#include "pq_print.h"          // PlaquetteSerialType
#include "PlotterFormat.h"

namespace pq {

/**
 * @brief Plotter unit: consumes floats and emits formatted rows to a Print backend.
 *
 * Typical usage (append style):
 *   Plotter plotter(115200, "wave,signal");
 *   void step() { wave >> plotter; signal >> plotter; plotter.endRow(); }
 *
 * Header can be enabled in the PlotterFormat (CSV-like).
 *
 * Notes:
 * - No allocation.
 * - No printf.
 * - Labels are schema-based and structural (do not flow through >>).
 */
class Plotter : public Unit {
public:
  // Default: Serial
  explicit Plotter(unsigned long baudRate,
                   const char* labels = nullptr,
                   Engine& engine = Engine::primary());

  // Serial device explicitly
  Plotter(SerialType& serial,
          unsigned long baudRate,
          const char* labels = nullptr,
          Engine& engine = Engine::primary());

  // Generic Print (no serial begin)
  explicit Plotter(Print& out,
                   const char* labels = nullptr,
                   Engine& engine = Engine::primary());

  // Backend / lifecycle
  void begin() override;
  void step() override;

  // Formatting
  void format(const PlotterFormat& fmt);
  PlotterFormat& format();

  // Precision (digits after decimal point)
  void precision(uint8_t digits);
  uint8_t precision() const;

  // Labels schema
  void labels(const char* labels);
  const char* labels() const;

  // Header control
  void headerPrinted(bool printed);  // mainly for reset()
  bool headerPrinted() const;

  // Row control
  virtual float put(float value) override;
  void endRow();          // emit row end (and header if needed)
  void endRowForce();     // emit end even if row is empty
  void clearRow();        // reset row state without emitting

  bool rowDirty() const;


private:
  Print* _out = nullptr;
  bool _isSerial = false;
  bool _begun = false;
  unsigned long _baudRate = 0;

  PlotterFormat _format;
  uint8_t _digits = 4;

  const char* _labels = nullptr;
  bool _headerPrinted = false;

  bool _rowOpen = false;
  bool _rowDirty = false;
  uint16_t _valueIndex = 0;

private:
  // Helpers
  void _ensureHeader();
  void _openRowIfNeeded();
  LabelView _labelAt(uint16_t index) const;

  static bool _isDelim(char c);
};

} // namespace pq

#endif // PQ_PLOTTER_H_
