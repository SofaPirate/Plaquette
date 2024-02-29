#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>
#include "HybridArrayList.h"

using namespace pq;
using namespace aunit;

#define INITIAL_CAPACITY 4
#define INITIAL_SIZE 8

void initializeHybridArray(HybridArrayList<int, INITIAL_CAPACITY>& hybridArray, size_t size = INITIAL_SIZE) {
  for (int i=0; i<(int)size; i++) {
    hybridArray.add(i);
  }
}

test(basic) {
  HybridArrayList<int, INITIAL_CAPACITY> hybridArray;
  initializeHybridArray(hybridArray);

  for (int i=0; i<INITIAL_SIZE; i++) {
    assertEqual(hybridArray[i], i);
  }
}

test(removeAll) {
  HybridArrayList<int, INITIAL_CAPACITY> hybridArray;
  initializeHybridArray(hybridArray);

  assertEqual(hybridArray.size(), (size_t)INITIAL_SIZE);
  for (int i=0; i<INITIAL_SIZE; i++) {
    assertEqual(hybridArray[i], i);
  }

  hybridArray.removeAll();
  assertEqual(hybridArray.size(), (size_t)0);

  for (int i=0; i<INITIAL_SIZE; i++) {
    hybridArray.add(i);
    assertEqual(hybridArray.size(), (size_t)(i+1));
  }
}


test(removeInsert) {
  for (int removeItem = 0; removeItem < INITIAL_SIZE; removeItem++) {
    HybridArrayList<int, INITIAL_CAPACITY> hybridArray;
    initializeHybridArray(hybridArray);

    // Remove item.
    hybridArray.remove(removeItem);
    for (int i=0; i<INITIAL_SIZE; i++) {
      if (i < removeItem) {
        assertEqual(hybridArray[i], i);
      }
      else if (i > removeItem) {
        assertEqual(hybridArray[i-1], i);
      }
    }

    // Re-insert item.
    hybridArray.insert(removeItem, removeItem);
    for (int i=0; i<INITIAL_SIZE; i++) {
      assertEqual(hybridArray[i], i);
    }
  }
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(10000);
}

void loop() {
  aunit::TestRunner::run();
}
