/*
 * pq_math.h
 *
 * Mathematical functions - replacements of Arduino macros.
 *
 * Arduino’s min/max/abs/etc. are unsafe macros: they re-evaluate arguments,
 * lack type safety, and collide with <algorithm>/<cmath>. This header provides
 * constexpr function alternatives in namespace pq (min, max, abs, constrain,
 * round, radians, degrees, sq). Legacy macro names are redefined to call the
 * safe pq:: functions for backwards compatibility.
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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
#ifndef PQ_MATH_H_
#define PQ_MATH_H_

#include <Arduino.h>

// --- Neutralize Arduino's macros so they don't collide ---
#ifdef min
#  undef min
#endif
#ifdef max
#  undef max
#endif
#ifdef abs
#  undef abs
#endif
#ifdef constrain
#  undef constrain
#endif
#ifdef round
#  undef round
#endif
#ifdef radians
#  undef radians
#endif
#ifdef degrees
#  undef degrees
#endif
#ifdef sq
#  undef sq
#endif

namespace pq {

// --- Common type machinery ---------------------------------
// Ensures math functions work safely when mixing types (e.g. int and float).
#if defined(__has_include) && __has_include(<type_traits>)

#include <type_traits>
template <typename A, typename B>
  using common_pair_t = typename std::common_type<A,B>::type;

template <typename A, typename B, typename C>
  using common_triple_t = typename std::common_type<A,B,C>::type;

#else

// Minimal fallback: works for most arithmetic types
template <typename A, typename B>
  struct common_pair_type {
    using type = decltype(true ? A{} : B{});
  };
template <typename A, typename B>
  using common_pair_t = typename common_pair_type<A,B>::type;

template <typename A, typename B, typename C>
  struct common_triple_type {
    using type = decltype(true ? true ? A{} : B{} : C{});
  };
template <typename A, typename B, typename C>
  using common_triple_t = typename common_triple_type<A,B,C>::type;

#endif

// --- Math functions ---------------------------------------

// -------- min / max --------
template <class A, class B>
constexpr common_pair_t<A,B> min(A a, B b) {
  typedef common_pair_t<A,B> R;
  return (static_cast<R>(a) < static_cast<R>(b)) ?
            static_cast<R>(a) :
            static_cast<R>(b);
}

template <class A, class B>
constexpr common_pair_t<A,B> max(A a, B b) {
  typedef common_pair_t<A,B> R;
  return (static_cast<R>(a) > static_cast<R>(b)) ?
            static_cast<R>(a) :
            static_cast<R>(b);
}

// -------- abs --------
template <typename T>
constexpr T abs(T x) {
    return (x < T{0}) ? -x : x;
}


// -------- constrain --------
template <class A, class L, class H>
constexpr common_triple_t<A,L,H>
constrain(A amt, L low, H high) {
  typedef common_triple_t<A,L,H> R;
    return (static_cast<R>(amt) < static_cast<R>(low) ? static_cast<R>(low) :
            static_cast<R>(amt) > static_cast<R>(high) ? static_cast<R>(high) :
            static_cast<R>(amt));
}

// -------- round (Arduino semantics: return long) --------
template <typename Float>
constexpr long round(Float x) {
    return (x >= Float{0})
         ? static_cast<long>(x + Float{0.5})
         : static_cast<long>(x - Float{0.5});
}

// -------- radians / degrees --------
template <typename T>
constexpr double radians(T deg) {
    return deg * DEG_TO_RAD;
}

template <typename T>
constexpr double degrees(T rad) {
    return rad * RAD_TO_DEG;
}

// -------- sq --------
template <typename T>
constexpr T sq(T x) { return x * x; }

} // namespace pq

// --- Legacy Arduino names for backwards compatibility -----
#define min(a,b)         (::pq::min((a),(b)))
#define max(a,b)         (::pq::max((a),(b)))
#define abs(x)           (::pq::abs((x)))
#define constrain(x,l,h) (::pq::constrain((x),(l),(h)))
#define round(x)         (::pq::round((x)))
#define radians(d)       (::pq::radians((d)))
#define degrees(r)       (::pq::degrees((r)))
#define sq(x)            (::pq::sq((x)))

#endif
