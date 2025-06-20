#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

using namespace pq;
using namespace aunit;

AnalogIn   analogIn(A0);
AnalogOut  analogOut(9);
DigitalIn  digitalIn(0);
DigitalOut digitalOut(10);
Wave  square(0.5);
Wave tri(TRIANGLE, 0.5);
Metronome      metro(0.5);

#define N_UNITS 7
Unit* units[N_UNITS] = {
  (Unit*)&analogIn,  (Unit*)&analogOut,
  (Unit*)&digitalIn,  (Unit*)&digitalOut,
  (Unit*)&tri,  (Unit*)&square,
  (Unit*)&metro
};

testing(valuesIn01) {
  static unsigned long startTime = millis();
  Plaquette.step();

  for (int i=0; i<N_UNITS; i++) {
    Unit* unit = units[i];
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

    void testPiping(Unit& source, Unit& sink) {
      source >> sink;
      assertEqual(source.get(),    sink.get());
      assertEqual((float)source,   (float)sink);
    }

    void testDigitalPiping(DigitalUnit& source, DigitalUnit& sink) {
      source >> sink;
      testPiping(source, sink);
      assertEqual(source.isOn(),    sink.isOn());
      assertEqual(source.getInt(),  sink.getInt());
      assertEqual((bool)source,  (bool)sink);
      assertEqual((int)source,   (int)sink);
    }

    void testValuePipingTo(Unit& node) {
      float value = randomUniform();
      value >> node;
      assertEqual(value, node.get());
    }

    // void testValuePipingFrom(Unit& node) {
    //   float value;
    //   node >> value;
    //   assertEqual(value, node.get());
    // }

    void testValueDigitalPipingTo(DigitalUnit& node) {
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

    // void testValueDigitalPipingFrom(DigitalUnit& node) {
    //   float value;
    //   int valueInt;
    //   bool valueBool;
    //   node >> value;
    //   node >> valueInt;
    //   node >> valueBool;
    //   assertEqual(value, node.get());
    //   assertEqual(valueInt, node.getInt());
    //   assertEqual(valueBool, node.isOn());
    // }

    void testDigital() {
      testDigitalPiping(digitalIn, digitalOut);
      testDigitalPiping(digitalIn, digitalOut);
    }

    void testAnalog() {
      testPiping(metro, digitalOut);
      testPiping(analogIn, analogOut);
      testPiping(square, analogOut);
      testPiping(tri, analogOut);
      testPiping(metro, analogOut);
    }

    void testValues() {
      testValueDigitalPipingTo(digitalOut);
      digitalOut.toggle();
      // testValueDigitalPipingFrom(digitalOut);
      bool prev = digitalOut;
      !digitalOut >> digitalOut;
      // testValueDigitalPipingFrom(digitalOut);
      assertEqual(!digitalOut.isOn(), prev);

      testValuePipingTo(analogOut);
      // testValuePipingFrom(analogOut);
      // (analogOut + 1) / 2 >> analogOut;
      // testValuePipingFrom(analogOut);
      // (analogOut * randomUniform()) >> analogOut;
      // testValuePipingFrom(analogOut);

      assertEqual(analogOut + 1, analogOut.get() + 1);
      assertEqual(analogOut * 0.5, analogOut.get() / 2);
      assertEqual(analogOut + analogOut, analogOut.get() * 2);
    }

};

testingF(TestPiping, test) {
  static unsigned long startTime = millis();
  Plaquette.step();
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
