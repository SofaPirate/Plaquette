/**
 * ScalerSpan
 *
 * Shows the usage of the "span" parameter of the Scaler unit.
 * Simulates a noisy signal using a sine wave and adding noise.
 * The signal is then sent to Scaler units with different spans.
 *
 * You can visualize the results using the Arduino's Serial Plotter.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// Controls the amount of noise in the signal.
const float NOISE_LEVEL = 1.0;

// The input signal core wave.
Wave wave(SINE);

// These unit will rescale signal to full range [0, 1].
RobustScaler scaler100;
RobustScaler scaler075;
RobustScaler scaler050;
RobustScaler scaler010;

void begin() {
  // Set different spans for comparison.
  scaler100.span(1.0f);
  scaler075.span(0.75f);
  scaler050.span(0.5f);
  scaler010.span(0.1f);
}

void step() {

  // Create signal by adding noise to the wave.
  float signal = wave + randomFloat(-NOISE_LEVEL/2, NOISE_LEVEL/2);

	// Analog input is rescaled then sent as LED value.
  signal >> scaler100;
  signal >> scaler075;
  signal >> scaler050;
  signal >> scaler010;

  // Prints raw and rescaled values, for comparison using the Serial plotter.
  print("signal:");    print(signal);    print(" ");
  print("scaler100:"); print(scaler100); print(" ");
  print("scaler075:"); print(scaler075); print(" ");
  print("scaler050:"); print(scaler050); print(" ");
  print("scaler010:"); println(scaler010);
}
