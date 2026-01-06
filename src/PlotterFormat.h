#ifndef PQ_PLOTTER_FORMAT_H_
#define PQ_PLOTTER_FORMAT_H_

#include "PqCore.h"

namespace pq {

/**
 * @brief Lightweight non-owning view of a label (no allocation).
 *
 * Labels come from a schema string (e.g., "wave,signal") and are typically
 * substrings within that schema. Therefore they are not necessarily
 * null-terminated. LabelView stores pointer + length.
 */
struct LabelView {
  const char* ptr = nullptr;
  uint16_t len = 0;

  /// True if no label is available.
  bool empty() const { return (ptr == nullptr || len == 0); }
};

/**
 * @brief Row formatting rules + printing logic (allocation-free, printf-free).
 *
 * A RowFormat describes how to render a sequence of elements as text.
 * It is used for both data rows and optional header rows.
 *
 * Element formatting is done using a lightweight template expander.
 *
 * Supported template keys:
 *   - {value} : prints the numeric value (float) with given precision (digits)
 *   - {label} : prints the label for the current index (may be empty)
 *   - {index} : prints the numeric index (0-based)
 *
 * Escaping:
 *   - \{  prints '{'
 *   - \}  prints '}'
 *   - \\  prints '\'
 */
class RowFormat {
public:
  /// Printed at the start of the row (e.g., "[" or "{").
  const char* begin = "";

  /// Printed at the end of the row (e.g., "]\n" or "}\n" or "\n").
  const char* end = "\n";

  /// Separator between elements (e.g., " ", ",", "\t").
  const char* separator = " ";

  /// Template for each element (e.g., "{value}" or "\"{label}\":{value}").
  const char* element = "{value}";

  /**
   * If true, prints separator after every element (including the last).
   * If false, prints separator only between elements.
   */
  bool trailingSeparator = false;

public:
  RowFormat() = default;

  RowFormat(const char* begin_,
            const char* end_,
            const char* sep_,
            const char* element_,
            bool trailingSep = false)
    : begin(begin_), end(end_), separator(sep_), element(element_),
      trailingSeparator(trailingSep) {}

  /**
   * @brief Print begin token.
   * Kept inline because it is trivial and called frequently.
   */
  void beginRow(Print& out) const {
    if (begin && *begin) out.print(begin);
  }

  /**
   * @brief Print end token.
   * Kept inline because it is trivial and called frequently.
   */
  void endRow(Print& out) const {
    if (end && *end) out.print(end);
  }

  /**
   * @brief Print separator token.
   * Kept inline because it is trivial and called frequently.
   */
  void sep(Print& out) const {
    if (separator && *separator) out.print(separator);
  }

  /**
   * @brief Print one element of a row.
   *
   * This expands the element template (e.g., "{label}:{value}") and prints
   * a separator either between elements or after each element depending on
   * trailingSeparator.
   *
   * @param out    Output device.
   * @param index  Element index within the row (0-based).
   * @param label  Label slice for this index (may be empty).
   * @param value  Numeric value to print.
   * @param digits Digits after decimal (Arduino Print convention).
   * @param isLast Whether this is the last element of the row.
   */
  void elementOut(Print& out,
                  uint16_t index,
                  LabelView label,
                  float value,
                  uint8_t digits,
                  bool isLast) const;

private:
  // --- Internal helpers (implemented in .cpp to keep header light) ---

  /// Compare a token key with a constant string without requiring null-termination.
  static bool _eq(const char* a, const char* b, uint8_t n);

  /// Print a LabelView as raw bytes (since it is not necessarily null-terminated).
  static void _writeLabel(Print& out, LabelView label);

  /**
   * @brief Expand and print a template.
   *
   * Parses templ left-to-right; for each "{key}" occurrence, emits the
   * corresponding expansion. Unknown keys are printed literally.
   */
  static void _writeTemplate(Print& out,
                             const char* templ,
                             uint16_t index,
                             LabelView label,
                             float value,
                             uint8_t digits);
};

/**
 * @brief Plotter format consisting of a data row and an optional header row.
 *
 * Plotter controls WHEN these are printed; PlotterFormat controls HOW.
 *
 * - row:    formatting for data rows
 * - header: formatting for header rows (labels)
 * - useHeader: whether Plotter should emit the header row (if labels exist)
 */
class PlotterFormat {
public:
  RowFormat row;
  RowFormat header;
  bool useHeader = false;

public:
  PlotterFormat() = default;

  PlotterFormat(const RowFormat& rowFmt,
                const RowFormat& headerFmt,
                bool headerEnabled = false)
    : row(rowFmt), header(headerFmt), useHeader(headerEnabled) {}
};

} // namespace pq

#endif // PQ_PLOTTER_FORMAT_H_
