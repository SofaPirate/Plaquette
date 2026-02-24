#!/bin/bash
# Bump version number across all Plaquette project files.
# Usage: ./bump_version.sh <new_version>
# Example: ./bump_version.sh 0.10.0

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <new_version>"
  echo "Example: $0 0.10.0"
  exit 1
fi

NEW_VERSION="$1"

# Validate semver format (MAJOR.MINOR.PATCH).
if ! echo "$NEW_VERSION" | grep -qE '^[0-9]+\.[0-9]+\.[0-9]+$'; then
  echo "Error: Version must be in MAJOR.MINOR.PATCH format (e.g., 0.10.0)"
  exit 1
fi

# Extract short version (MAJOR.MINOR).
SHORT_VERSION=$(echo "$NEW_VERSION" | sed 's/\.[0-9]*$//')

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Bumping version to $NEW_VERSION (short: $SHORT_VERSION)"

# library.properties
sed -i "s/^version=.*/version=$NEW_VERSION/" "$SCRIPT_DIR/library.properties"
echo "  Updated library.properties"

# package.json
sed -i "s/\"version\": \"[0-9][^\"]*\"/\"version\": \"$NEW_VERSION\"/" "$SCRIPT_DIR/package.json"
echo "  Updated package.json"

# docs/conf.py
sed -i "s/^version = '.*'/version = '$SHORT_VERSION'/" "$SCRIPT_DIR/docs/conf.py"
sed -i "s/^release = '.*'/release = '$NEW_VERSION'/" "$SCRIPT_DIR/docs/conf.py"
echo "  Updated docs/conf.py"

echo "Done. Version bumped to $NEW_VERSION"
