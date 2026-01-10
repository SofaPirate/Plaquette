// PqValue.h (or inside an existing core header)
//
// Typed wrappers for basic values as Chainables.
// Implements: in >> value; value >> out; and convenient C++ value semantics.

#ifndef PQ_VALUE_INC_
#define PQ_VALUE_INC_

#include "PqCore.h"

namespace pq {

// ---------- Conversion traits ----------
template <typename T>
struct ValueCodec;

// float passthrough
template <>
struct ValueCodec<float> {
  static inline float fromFloat(float v) { return v; }
  static inline float toFloat(float v) { return v; }
};

// int32 rounding policy (deterministic, no libm dependency)
template <>
struct ValueCodec<int32_t> {
  static inline int32_t fromFloat(float v) {
    return static_cast<int32_t>(round(v));
  }
  static inline float toFloat(int32_t v) { return static_cast<float>(v); }
};

// bool threshold policy
template <>
struct ValueCodec<bool> {
  static inline bool fromFloat(float v) { return Flowable::analogToDigital(v); }
  static inline float toFloat(bool v) { return Flowable::digitalToAnalog(v); }
};

// ---------- Generic Flowable value wrapper ----------

template <typename T, class Codec = ValueCodec<T>>
class Value : public Flowable {
public:
  using value_type = T;

  Value() : _v(0) {}
  Value(T initial) : _v(initial) {}

  // Flowable API
  float put(float x) override { _v = Codec::fromFloat(x); return x; }
  float get() override { return Codec::toFloat(_v); }

  // Explicit accessors
  inline T value() const { return _v; }
  inline void value(T v) { _v = v; }

  // Implicit conversion to base type (enables: T t = myValue;)
  inline operator T() const { return _v; }

  // Assignment from base type
  inline Value& operator=(T v) { _v = v; return *this; }

  // Prefix ++ / --
  inline Value& operator++() { ++_v; return *this; }
  inline Value& operator--() { --_v; return *this; }

  // Postfix ++ / --  (returns the *old* base value like built-in types)
  inline T operator++(int) { T old = _v; ++_v; return old; }
  inline T operator--(int) { T old = _v; --_v; return old; }

  // Compound assignment with base type
  inline Value& operator+=(T rhs) { _v += rhs; return *this; }
  inline Value& operator-=(T rhs) { _v -= rhs; return *this; }
  inline Value& operator*=(T rhs) { _v *= rhs; return *this; }
  inline Value& operator/=(T rhs) { _v /= rhs; return *this; }

  // Compound assignment with another Value
  inline Value& operator+=(const Value& rhs) { _v += rhs._v; return *this; }
  inline Value& operator-=(const Value& rhs) { _v -= rhs._v; return *this; }
  inline Value& operator*=(const Value& rhs) { _v *= rhs._v; return *this; }
  inline Value& operator/=(const Value& rhs) { _v /= rhs._v; return *this; }

  // Comparisons (Value-to-Value, Value-to-scalar)
  friend inline bool operator==(const Value& a, const Value& b) { return a._v == b._v; }
  friend inline bool operator!=(const Value& a, const Value& b) { return a._v != b._v; }
  friend inline bool operator< (const Value& a, const Value& b) { return a._v <  b._v; }
  friend inline bool operator<=(const Value& a, const Value& b) { return a._v <= b._v; }
  friend inline bool operator> (const Value& a, const Value& b) { return a._v >  b._v; }
  friend inline bool operator>=(const Value& a, const Value& b) { return a._v >= b._v; }

  friend inline bool operator==(const Value& a, T b) { return a._v == b; }
  friend inline bool operator==(T a, const Value& b) { return a == b._v; }
  friend inline bool operator!=(const Value& a, T b) { return a._v != b; }
  friend inline bool operator!=(T a, const Value& b) { return a != b._v; }

  // Basic arithmetic: return base type (or choose float — see notes below)
  friend inline T operator+(const Value& a, const Value& b) { return a._v + b._v; }
  friend inline T operator-(const Value& a, const Value& b) { return a._v - b._v; }
  friend inline T operator*(const Value& a, const Value& b) { return a._v * b._v; }
  friend inline T operator/(const Value& a, const Value& b) { return a._v / b._v; }

  friend inline T operator+(const Value& a, T b) { return a._v + b; }
  friend inline T operator+(T a, const Value& b) { return a + b._v; }
  friend inline T operator-(const Value& a, T b) { return a._v - b; }
  friend inline T operator-(T a, const Value& b) { return a - b._v; }

protected:
  T _v;
};

// ---------- Convenient aliases matching issue #157 ----------
using Float   = Value<float>;
using Integer = Value<int32_t>;
using Boolean = Value<bool>;

} // namespace pq

#endif
