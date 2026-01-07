/*
 * Monitor.h
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

#ifndef PQ_MONITOR_H_
#define PQ_MONITOR_H_

#include "PqCore.h"
#include "pq_serial.h"

namespace pq {

/**
 * @brief Write-only monitor unit for textual output.
 *
 * Monitor is a Plaquette Unit and a Print proxy. It is intended for
 * human-readable output (debugging, status messages) and deliberately
 * does not expose Stream read functionality to avoid blocking behavior.
 *
 * The backend can be any Print-compatible object (Serial, file, etc.).
 * When bound to a serial device, Monitor can auto-start it at a given
 * baud rate.
 */
class Monitor : public Unit, public Print {
public:
  /**
   * @brief Construct a Monitor using the default Serial device.
   */
  explicit Monitor(unsigned long baudRate,
                   Engine& engine = Engine::primary());

  /**
   * @brief Construct a Monitor using a specific serial device.
   */
  Monitor(SerialType& device, unsigned long baudRate,
          Engine& engine = Engine::primary());

  /**
   * @brief Construct a Monitor using a generic Print backend.
   * No auto-start is performed.
   */
  explicit Monitor(Print& device,
                   Engine& engine = Engine::primary());

  /// @brief Get the current Print backend.
  Print& device() const { return *_device; }

  /**
   * @brief Set the number of digits to print after the decimal point.
   */
  void precision(uint8_t digits);

  /// @brief Get the number of digits to print after the decimal point by default.
  uint8_t precision() const { return _digits; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  float put(float value) override;

  /// Returns value (last value that was put()).
  float get() override { return _value; }

  /**
   * @brief Core Print override.
   *
   * All print()/println() calls funnel through this method.
   */
  virtual size_t write(uint8_t b) override;

  using Print::print;
  using Print::println;

  size_t print(double v);
  size_t println(double v);

  // // This is to support all the Print methods
  // using Print::write;

protected:
  /**
   * @brief Ensure the underlying device is started (idempotent).
   *
   * For non-serial backends, this is a no-op.
   */
  void begin() override;

  Print* _device = nullptr;
  bool   _isSerial   : 1;
  bool   _begun      : 1;
  uint8_t _digits    : 6;

  unsigned long _baudRate = 0;

  float _value;
};

} // namespace pq

#endif // PQ_MONITOR_H_
