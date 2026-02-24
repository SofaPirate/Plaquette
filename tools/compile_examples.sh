#!/usr/bin/env bash
set -euo pipefail

# -----------------------------
# Configuration (defaults)
# -----------------------------
# List of default board targets (FQBNs) to compile against when -b is not provided.
# Example FQBN format: vendor:architecture:board
DEFAULT_BOARDS=(
  "teensy:avr:teensy31"
  # "arduino:avr:uno"
  # "arduino:avr:micro"
  # "esp32:esp32:feathers3"
  # "teensy:avr:teensy40"
)

# -----------------------------
# Helpers
# -----------------------------
usage() {
  cat <<'USAGE'
Usage:
  ./compile_examples.sh                Compile all examples for default boards
  ./compile_examples.sh -p <path>      Compile only one example (sketch directory)
  ./compile_examples.sh -b <list>      Compile for a comma-separated list of FQBNs
  ./compile_examples.sh -p <path> -b <list>

Options:
  -p <path>   Path to a sketch directory. May be:
              - absolute, or
              - relative to repo root, or
              - relative to repo_root/examples

  -b <list>   Comma-separated list of board FQBN targets.
              Example: teensy:avr:teensy31,arduino:avr:uno

  -h          Show this help

Notes:
  - If -b is omitted, DEFAULT_BOARDS at the top of the script is used.
  - This script expects ./bin/arduino-cli to exist relative to the script.
USAGE
}

die() {
  echo "Error: $*" 1>&2
  exit 1
}

# Resolve repo root as the directory that contains this script.
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

ARDUINO_CLI="$ROOT_DIR/bin/arduino-cli"
[[ -x "$ARDUINO_CLI" ]] || die "arduino-cli not found or not executable at: $ARDUINO_CLI"

# Keep the original behavior: install Servo.
"$ARDUINO_CLI" lib install Servo >/dev/null

# Build the compile command (keep original libraries relative path).
# NOTE: this is evaluated at runtime in the sketch directory; preserve original ../../..
CMD="$ARDUINO_CLI compile --libraries ../../.."

normalize_sketch_path() {
  local in_path="$1"

  # Absolute path
  if [[ "$in_path" = /* ]]; then
    echo "$in_path"
    return 0
  fi

  # Relative to repo root
  if [[ -d "$ROOT_DIR/$in_path" ]]; then
    echo "$ROOT_DIR/$in_path"
    return 0
  fi

  # Relative to examples/
  if [[ -d "$ROOT_DIR/examples/$in_path" ]]; then
    echo "$ROOT_DIR/examples/$in_path"
    return 0
  fi

  return 1
}

compile_one() {
  local sketch_dir="$1"
  shift
  local -a boards=("$@")

  [[ -d "$sketch_dir" ]] || die "Sketch directory does not exist: $sketch_dir"

  # Make output easier to scan
  echo "== $(realpath --relative-to="$ROOT_DIR" "$sketch_dir" 2>/dev/null || echo "$sketch_dir") =="

  pushd "$sketch_dir" >/dev/null
  for b in "${boards[@]}"; do
    echo "-- board: $b"
    $CMD -b "$b" .
  done
  popd >/dev/null
}

# -----------------------------
# CLI parsing
# -----------------------------
SKETCH_PATH=""
BOARDS_CSV=""

while getopts ":p:b:h" opt; do
  case "$opt" in
    p) SKETCH_PATH="$OPTARG" ;;
    b) BOARDS_CSV="$OPTARG" ;;
    h) usage; exit 0 ;;
    :) die "Option -$OPTARG requires an argument" ;;
    \?) die "Unknown option: -$OPTARG (use -h for help)" ;;
  esac
done

# Boards selection
declare -a BOARDS
if [[ -n "$BOARDS_CSV" ]]; then
  IFS=',' read -r -a BOARDS <<< "$BOARDS_CSV"
  # Trim whitespace around entries (defensive)
  for i in "${!BOARDS[@]}"; do
    BOARDS[$i]="${BOARDS[$i]//[[:space:]]/}"
  done
else
  BOARDS=("${DEFAULT_BOARDS[@]}")
fi

[[ ${#BOARDS[@]} -gt 0 ]] || die "No boards specified. Set DEFAULT_BOARDS or provide -b." 

# Single-sketch mode
if [[ -n "$SKETCH_PATH" ]]; then
  if ! SKETCH_DIR="$(normalize_sketch_path "$SKETCH_PATH")"; then
    die "Could not resolve sketch path: $SKETCH_PATH"
  fi
  compile_one "$SKETCH_DIR" "${BOARDS[@]}"
  exit 0
fi

# -----------------------------
# Compile all examples
# -----------------------------
EXAMPLES_DIR="$ROOT_DIR/examples"
[[ -d "$EXAMPLES_DIR" ]] || die "examples/ directory not found at: $EXAMPLES_DIR"

# Match your existing layout: examples/<category>/<example>/
shopt -s nullglob
for category_dir in "$EXAMPLES_DIR"/*/; do
  for example_dir in "$category_dir"*/; do
    compile_one "$example_dir" "${BOARDS[@]}"
  done
done
