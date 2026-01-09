/*
 * PlotterFormat.h
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
 * @brief Formatting specification used by Plotter to render values and (optionally) a header.
 *
 * This is intentionally minimal:
 * - The row and the header share the same framing (begin/end/separator).
 * - The only difference is which template is used:
 *     - valueTemplate: used for normal rows
 *     - keyTemplate:   used for header row (keys)
 *
 * Template mini-language:
 *   - $v : numeric value (float) printed with the provided precision
 *   - $i : element index (0-based)
 *   - $k : key/label (LabelView) if available, otherwise keyFallback
 *
 * Escaping:
 *   - Backslash escapes the next character: "\$" prints '$'
 *   - "\X" prints 'X' for any character X
 *
 * Unknown $-tokens are printed literally (e.g., "$q" prints "$q").
 */
struct PlotterFormat {
  // Row framing (shared by values and header, if any).
  const char* plotBegin = "";
  const char* plotEnd   = "";
  const char* rowBegin  = "";
  const char* rowEnd    = "\r\n";
  const char* separator = ",";

  // Templates:
  // - valueTemplate is expanded for each value in normal rows.
  // - keyTemplate is expanded for each key in the header row (when enabled).
  const char* valueTemplate = "$v";
  const char* keyTemplate   = "$k";

  // If true, Plotter may emit a header row (typically only when labels exist).
  bool headerEnabled = false;

  // If true, prints separator after every element (including the last).
  // If false, prints separator only between elements.
  bool trailingSeparator = false;
  bool trailingRowEnd = true;

  // Fallback string used when $k is requested but no label exists for this index
  // (labels missing entirely, labels shorter than value count, empty token, etc.).
  // If nullptr or empty, $k prints nothing in those cases.
  const char* keyFallback = "value_$i";

  // ---- Convenience helpers used by Plotter (short and inline) ----

  static void printString(Print& out, const char* str) {
    if (str && *str) out.print(str);
  }

  void beginPlot(Print& out) const { printString(out, plotBegin); }
  void endPlot(Print& out) const {
    printString(out, plotEnd);
  }
  void beginRow(Print& out) const { printString(out, rowBegin); }
  void endRow(Print& out)   const {
    printString(out, rowEnd);
  }
  void sep(Print& out)      const { printString(out, separator); }

  /**
   * @brief Print one element for a normal (value) row.
   *
   * @param out    Output device
   * @param index  Element index (0-based)
   * @param label  Label for this index (may be empty)
   * @param value  Value for this element
   * @param digits Digits after decimal point (Arduino Print convention)
   * @param isFirst Whether this is the first element in the row
   */
  void printValueElement(Print& out,
                         uint16_t index,
                         LabelView label,
                         float value,
                         uint8_t digits,
                         bool isFirst) const;

  /**
   * @brief Print one element for a header (key) row.
   *
   * Typically Plotter will call this only when labels exist, but this function
   * also applies keyFallback if label is missing for the index.
   */
  void printKeyElement(Print& out,
                       uint16_t index,
                       LabelView label,
                       uint8_t digits,
                       bool isFirst) const;

private:
  // Core template expansion: implemented in .cpp to keep the header light.
  static void _writeTemplate(Print& out,
                             const char* templ,
                             uint16_t index,
                             LabelView label,
                             float value,
                             uint8_t digits,
                             const char* keyFallback);

  static void _writeKey(Print& out,
                        LabelView label,
                        uint16_t index,
                        const char* keyFallback);
};

} // namespace pq

#endif // PQ_PLOTTER_FORMAT_H_
