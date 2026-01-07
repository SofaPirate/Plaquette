/*
 * PlotterFormat.cpp
 *
 * Implements PlotterFormat template expansion without printf and without allocation.
 */

#include "PlotterFormat.h"

namespace pq {

void PlotterFormat::printValueElement(Print& out,
                                      uint16_t index,
                                      LabelView label,
                                      float value,
                                      uint8_t digits,
                                      bool isFirst) const
{
  // Apply separator policy.
  if (!isFirst) {
    sep(out);
  }

  // Expand and print the value template.
  _writeTemplate(out, valueTemplate, index, label, value, digits, keyFallback);
}

void PlotterFormat::printKeyElement(Print& out,
                                    uint16_t index,
                                    LabelView label,
                                    uint8_t digits,
                                    bool isFirst) const
{
  // Apply separator policy.
  if (!isFirst) {
    sep(out);
  }

  // For header printing, "value" is irrelevant; we pass 0.0f.
  _writeTemplate(out, keyTemplate, index, label, 0.0f, digits, keyFallback);
}

void PlotterFormat::_writeKey(Print& out,
                              LabelView label,
                              uint16_t index,
                              const char* keyFallback)
{
  // If we have a real label, write it as raw bytes (not necessarily null-terminated).
  if (!label.empty()) {
    out.write(reinterpret_cast<const uint8_t*>(label.ptr), label.len);
  }

  // Otherwise print fallback (if any).
  else if (keyFallback && *keyFallback) {
    _writeTemplate(out, keyFallback, index, label, 0, 0, nullptr);
  }
}

void PlotterFormat::_writeTemplate(Print& out,
                                   const char* templ,
                                   uint16_t index,
                                   LabelView label,
                                   float value,
                                   uint8_t digits,
                                   const char* keyFallback)
{
  if (!templ) return;

  const char* p = templ;

  while (*p) {
    const char c = *p++;

    // Backslash escape: "\$" prints '$', "\X" prints 'X'.
    if (c == '\\' && *p) {
      out.write(static_cast<uint8_t>(*p++));
      continue;
    }

    // Regular characters pass through.
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
      // Numeric value.
      out.print(static_cast<double>(value), digits);
    }
    else if (t == 'i') {
      // Index (0-based).
      out.print(static_cast<unsigned long>(index));
    }
    else if (t == 'k') {
      // Key/label with fallback.
      if (keyFallback)
        _writeKey(out, label, index, keyFallback);
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
