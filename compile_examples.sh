#!/bin/bash
arduino-cli lib install Servo
CMD="$(pwd)/bin/arduino-cli compile --libraries ../../.."
cd examples
for d in */ ; do 
	cd "$d"
	for e in */ ; do 
		echo "== $e =="
		$CMD -b arduino:avr:uno "$e"
		$CMD -b arduino:avr:micro "$e"
		$CMD -b esp32:esp32:feathers3 "$e"
		$CMD -b teensy:avr:teensy31 "$e"
		$CMD -b teensy:avr:teensy40 "$e"
	done
	cd ..
done
cd ..
