#!/bin/bash
# Create a GitHub release for a given tag.
# Usage: ./release.sh [tag]
# If no tag is given, defaults to the latest git tag.
# Example: ./release.sh v0.9.4

set -euo pipefail

TAG="${1:-$(git describe --tags --abbrev=0)}"
[[ "$TAG" == v* ]] || TAG="v$TAG"

NOTES_DIR="release-notes"
mkdir -p "$NOTES_DIR"

# Extract the matching section from CHANGELOG.md
RELEASE_TAG="$TAG" python3 - <<'PY'
import re, os
from pathlib import Path

tag = os.environ['RELEASE_TAG']
text = Path("CHANGELOG.md").read_text(encoding="utf-8")

hdr = re.compile(r'^#### \[(v\d+\.\d+\.\d+)\].*$', re.M)
matches = list(hdr.finditer(text))
sections = {}
for i, m in enumerate(matches):
    start = m.start()
    end = matches[i+1].start() if i+1 < len(matches) else len(text)
    sections[m.group(1)] = text[start:end].strip()

if tag not in sections:
    raise SystemExit(f"ERROR: Could not find section for {tag} in CHANGELOG.md")

lines = sections[tag].splitlines()
if lines and lines[0].startswith("#### [v"):
    lines = lines[1:]
while lines and not lines[0].strip():
    lines.pop(0)
body = "\n".join(lines).strip() + "\n"

out_dir = Path(os.environ.get('NOTES_DIR', 'release-notes'))
(out_dir / f"{tag}.md").write_text(f"## Changelog\n{body}", encoding="utf-8")
print(f"Wrote release notes for {tag}")
PY

ver="${TAG#v}"
gh release create "$TAG" \
  --title "Version ${ver}" \
  --notes-file "${NOTES_DIR}/${TAG}.md"
