/*
 * pq_traits.h
 *
 * Minimal, AVR-safe type traits for Plaquette.
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef PQ_TRAITS_H_
#define PQ_TRAITS_H_

namespace pq {

// ---------- enable_if ----------

// C++11-compatible enable_if_t.
template <bool B, typename T = void>
struct enable_if { };

template <typename T>
struct enable_if<true, T> { typedef T type; };

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

// ---------- is_integral ----------

#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>

template <typename T>
using is_integral = std::is_integral<T>;

#else
// Minimal is_integral (AVR-safe).
template <typename T> struct is_integral { static const bool value = false; };

template <> struct is_integral<bool> { static const bool value = true; };
template <> struct is_integral<char> { static const bool value = true; };
template <> struct is_integral<signed char> { static const bool value = true; };
template <> struct is_integral<unsigned char> { static const bool value = true; };
template <> struct is_integral<short> { static const bool value = true; };
template <> struct is_integral<unsigned short> { static const bool value = true; };
template <> struct is_integral<int> { static const bool value = true; };
template <> struct is_integral<unsigned int> { static const bool value = true; };
template <> struct is_integral<long> { static const bool value = true; };
template <> struct is_integral<unsigned long> { static const bool value = true; };
template <> struct is_integral<long long> { static const bool value = true; };
template <> struct is_integral<unsigned long long> { static const bool value = true; };

#endif

// ---------- is_same ----------

template <typename T, typename U>
struct is_same { static const bool value = false; };
template <typename T>
struct is_same<T, T> { static const bool value = true; };

// ---------- supports_modulo ----------

// True for integral types excluding bool.
template <typename T>
struct supports_modulo {
  static const bool value = is_integral<T>::value && !is_same<T, bool>::value;
};

// ---------- remove_reference / remove_cv ----------

template <typename T> struct remove_reference      { typedef T type; };
template <typename T> struct remove_reference<T&>  { typedef T type; };
#if __cplusplus >= 201103L
template <typename T> struct remove_reference<T&&> { typedef T type; };
#endif

template <typename T> struct remove_const          { typedef T type; };
template <typename T> struct remove_const<const T> { typedef T type; };

template <typename T> struct remove_volatile               { typedef T type; };
template <typename T> struct remove_volatile<volatile T>   { typedef T type; };

template <typename T>
struct remove_cv {
  typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};

template <typename T>
struct remove_cvref {
  typedef typename remove_cv<typename remove_reference<T>::type>::type type;
};

// ---------- always_false ----------

template <typename>
struct always_false { enum { value = 0 }; };

} // namespace pq

#endif
