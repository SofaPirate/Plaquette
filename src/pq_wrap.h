#include <Arduino.h>

#include <limits.h>

inline float wrap01(float f) {
  while ( f > ULONG_MAX) f -= ULONG_MAX;
  while (-f > ULONG_MAX) f += ULONG_MAX;
  if (f >= 0) {
    return f - (unsigned long)(f);
  }
  else {
    // Untested.
    return 1 + f + (unsigned long)(-f);
  }
// //  f = fmod(f, 1);
//   return (f < 0 ? f+1 : f);
}
