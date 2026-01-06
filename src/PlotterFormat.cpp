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
  _writeTemplate(out, element, index, label, value, digits, keyFallback);

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

void RowFormat::_writeKey(Print& out,
                          LabelView label,
                          uint16_t index,
                          const char* fallback) {
  // If we have a real label, write it as raw bytes (not necessarily null-terminated).
  if (!label.empty()) {
    out.write(reinterpret_cast<const uint8_t*>(label.ptr), label.len);
  }
  // Otherwise print fallback (if any).
  else if (fallback && *fallback) {
    _writeTemplate(out, fallback, index, label, 0, 0);
  }
}

void RowFormat::_writeTemplate(Print& out,
                               const char* templ,
                               uint16_t index,
                               LabelView label,
                               float value,
                               uint8_t digits,
                               const char* fallback)
{
  if (!templ) return;

  const char* p = templ;

  while (*p) {
    const char c = *p++;

    // Backslash escape: "\$" prints '$', "\X" prints 'X' (existing behavior).
    if (c == '\\' && *p) {
      out.write(static_cast<uint8_t>(*p++));
      continue;
    }

    // Regular characters.
    if (c != '$') {
      out.write(static_cast<uint8_t>(c));
      continue;
    }

    // '$' introduces a token.
    if (!*p) {
      // Trailing '$' -> print literally.
      out.write(static_cast<uint8_t>('$'));
      break;
    }

    const char t = *p++;

    if (t == 'v') {
      // Value (float)
      out.print(static_cast<double>(value), digits);
    }
    else if (t == 'i') {
      // Index (0-based)
      out.print(static_cast<unsigned long>(index));
    }
    else if (t == 'k') {
      // Key/label with fallback
      if (fallback)
        _writeKey(out, label, index, fallback);
      else
        out.print("$k");
    }
    else {
      // Unknown token -> print literally as "$<t>"
      out.write(static_cast<uint8_t>('$'));
      out.write(static_cast<uint8_t>(t));
    }
  }
}


} // namespace pq
