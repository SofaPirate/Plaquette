/*
 * PlotterFormat.cpp
 *
 * Implements RowFormat template expansion without printf and without allocation.
 */

#include "PlotterFormat.h"

namespace pq {

void RowFormat::elementOut(Print& out,
                           uint16_t index,
                           LabelView label,
                           float value,
                           uint8_t digits,
                           bool isLast) const
{
  // 1) Expand and print the element template (e.g. "{value}" or "\"{label}\":{value}")
  _writeTemplate(out, element, index, label, value, digits);

  // 2) Separator policy:
  //    - trailingSeparator: always print separator after each element.
  //    - otherwise: print separator only between elements.
  if (trailingSeparator) {
    sep(out);
  }
  else if (!isLast) {
    sep(out);
  }
}

bool RowFormat::_eq(const char* a, const char* b, uint8_t n) {
  // Simple fixed-length comparison (no need for '\0' in `a`).
  for (uint8_t i = 0; i < n; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void RowFormat::_writeLabel(Print& out, LabelView label) {
  // LabelView is not necessarily null-terminated, so we write raw bytes.
  if (label.empty()) return;
  out.write(reinterpret_cast<const uint8_t*>(label.ptr), label.len);
}

void RowFormat::_writeTemplate(Print& out,
                               const char* templ,
                               uint16_t index,
                               LabelView label,
                               float value,
                               uint8_t digits)
{
  // Defensive: null template means "print nothing".
  if (!templ) return;

  // One-pass parser. This keeps code size and RAM usage low.
  const char* p = templ;

  while (*p) {
    const char c = *p++;

    // Backslash escapes:
    //  - \{ prints '{'
    //  - \} prints '}'
    //  - \\ prints '\'
    //  - Any other "\X" prints "X"
    if (c == '\\' && *p) {
      out.write(static_cast<uint8_t>(*p++));
      continue;
    }

    // Regular characters pass through.
    if (c != '{') {
      out.write(static_cast<uint8_t>(c));
      continue;
    }

    // We encountered '{' -> parse a key until '}'.
    const char* keyStart = p;
    while (*p && *p != '}') p++;

    // If no closing brace, treat '{' as a literal.
    if (*p != '}') {
      out.write(static_cast<uint8_t>('{'));
      p = keyStart; // rewind so characters after '{' are processed normally
      continue;
    }

    // Now [keyStart, keyEnd) is the key name (not null-terminated).
    const char* keyEnd = p; // points at '}'
    p++;                    // skip the '}'
    const uint8_t len = static_cast<uint8_t>(keyEnd - keyStart);

    // Recognize known keys by (length, content) to avoid strcmp / allocations.
    if (len == 5 && _eq(keyStart, "value", 5)) {
      // Print numeric value with Arduino Print's float formatting.
      // `digits` is digits after decimal.
      out.print(static_cast<double>(value), digits);
    }
    else if (len == 5 && _eq(keyStart, "label", 5)) {
      // Print label slice (may be empty).
      _writeLabel(out, label);
    }
    else if (len == 5 && _eq(keyStart, "index", 5)) {
      // Print index (0-based).
      out.print(static_cast<unsigned long>(index));
    }
    else {
      // Unknown key -> print literally "{key}" so the user can see the mistake.
      out.write(static_cast<uint8_t>('{'));
      while (keyStart < keyEnd) out.write(static_cast<uint8_t>(*keyStart++));
      out.write(static_cast<uint8_t>('}'));
    }
  }
}

} // namespace pq
