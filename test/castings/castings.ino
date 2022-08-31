#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

using namespace pq;
using namespace aunit;

AnalogIn   aIn(A0);
AnalogOut  aOut(9);
DigitalIn  dIn(0);
DigitalOut dOut(10);
SquareOsc  square(0.5);
TriOsc     tri(0.5);
Metro      metro(0.5);

#define N_UNITS 7
Node* units[N_UNITS] = {
  (Node*)&aIn,  (Node*)&aOut,
  (Node*)&dIn,  (Node*)&dOut,
  (Node*)&tri,  (Node*)&square,
  (Node*)&metro
};

testing(valuesIn01) {
  static unsigned long startTime = millis();

  for (int i=0; i<N_UNITS; i++) {
    Node* unit = units[i];
    float value = unit->get();
    assertMoreOrEqual(value, 0.0f);
    assertLessOrEqual(value, 1.0f);
  }
  if (millis() - startTime > 1000) pass();
}

class TestPiping: public TestAgain {
  protected:
    // optional
    void setup() override {
      TestAgain::setup();
    }

    void teardown() override {
      TestAgain::teardown();
    }

    void testPiping(Node& source, Node& sink) {
      source >> sink;
      assertEqual(source.get(),    sink.get());
      assertEqual((float)source,   (float)sink);
    }

    void testDigitalPiping(DigitalNode& source, DigitalNode& sink) {
      source >> sink;
      testPiping(source, sink);
      assertEqual(source.isOn(),    sink.isOn());
      assertEqual(source.getInt(),  sink.getInt());
      assertEqual((bool)source,  (bool)sink);
      assertEqual((int)source,   (int)sink);
    }

    void testValuePipingTo(Node& node) {
      float value = randomUniform();
      value >> node;
      assertEqual(value, node.get());
    }

    void testValuePipingFrom(Node& node) {
      float value;
      node >> value;
      assertEqual(value, node.get());
    }

    void testValueDigitalPipingTo(DigitalNode& node) {
      1.0f >> node;
      assertEqual(1.0f, node.get());
      assertEqual(true, node.isOn());
      assertEqual(1, node.getInt());

      true >> node;
      assertEqual(1.0f, node.get());
      assertEqual(true, node.isOn());
      assertEqual(1, node.getInt());

      false >> node;
      assertEqual(0.0f, node.get());
      assertEqual(false, node.isOn());
      assertEqual(0, node.getInt());
    }

    void testValueDigitalPipingFrom(DigitalNode& node) {
      float value;
      int valueInt;
      bool valueBool;
      node >> value;
      node >> valueInt;
      node >> valueBool;
      assertEqual(value, node.get());
      assertEqual(valueInt, node.getInt());
      assertEqual(valueBool, node.isOn());
    }

    void testDigital() {
      testDigitalPiping(dIn, dOut);
      testDigitalPiping(dIn, dOut);
    }

    void testAnalog() {
      testPiping(metro, dOut);
      testPiping(aIn, aOut);
      testPiping(square, aOut);
      testPiping(tri, aOut);
      testPiping(metro, aOut);
    }

    void testValues() {
      testValueDigitalPipingTo(dOut);
      dOut.toggle();
      testValueDigitalPipingFrom(dOut);
      bool prev = dOut;
      !dOut >> dOut;
      testValueDigitalPipingFrom(dOut);
      assertEqual(!dOut.isOn(), prev);

      testValuePipingTo(aOut);
      testValuePipingFrom(aOut);
      (aOut + 1) / 2 >> aOut;
      testValuePipingFrom(aOut);
      (aOut * randomUniform()) >> aOut;
      testValuePipingFrom(aOut);

      assertEqual(aOut + 1, aOut.get() + 1);
      assertEqual(aOut * 0.5, aOut.get() / 2);
      assertEqual(aOut + aOut, aOut.get() * 2);
    }

};

testingF(TestPiping, test) {
  static unsigned long startTime = millis();
  testDigital();
  testAnalog();
  testValues();
  if (millis() - startTime > 2000) pass();
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);
}

void loop() {
  TestRunner::run();
}
