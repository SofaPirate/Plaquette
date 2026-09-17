#!/bin/bash
# Interactive orchestration for cutting a Plaquette release.
# Walks through version bump, changelog review, push, docs rebuild,
# PDF manual commit, gh-pages sync, and GitHub release creation,
# pausing for confirmation before each irreversible/visible step.
#
# Usage: ./tools/do_release.sh [version]
# Example: ./tools/do_release.sh 0.9.4

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
GHPAGES_DIR="$ROOT_DIR/../Plaquette-docs/html"

cd "$ROOT_DIR"

confirm() {
  read -r -p "$1 [y/N] " reply
  [[ "$reply" =~ ^[Yy]$ ]]
}

echo "=== Plaquette release ==="
echo

# --- Pre-flight ---

if ! command -v node >/dev/null 2>&1 || ! command -v npm >/dev/null 2>&1; then
  echo "Error: node/npm not found on PATH (needed for auto-changelog)."
  exit 1
fi

if [ ! -x node_modules/.bin/auto-changelog ]; then
  echo "Error: node_modules/.bin/auto-changelog not found. Run 'npm install' first."
  exit 1
fi

if ! command -v gh >/dev/null 2>&1; then
  echo "Error: gh CLI not found."
  exit 1
fi

if ! gh auth status >/dev/null 2>&1; then
  echo "Error: gh CLI is not authenticated. Run 'gh auth login' first."
  exit 1
fi

if [ -n "$(git status --porcelain --untracked-files=no)" ]; then
  echo "Error: working tree has uncommitted changes to tracked files. Commit or stash them first."
  git status --short
  exit 1
fi

CURRENT_BRANCH="$(git branch --show-current)"
echo "Branch: $CURRENT_BRANCH"
echo "gh:     $(gh auth status 2>&1 | grep 'Logged in' | sed 's/^ *//')"
echo

# --- Local tests ---

if confirm "Run local AUnit test suite (tests/) before proceeding?"; then
  if ! (cd "$ROOT_DIR/tests" && make clean >/dev/null && make && make test); then
    echo "Error: local tests failed. Fix them before releasing."
    exit 1
  fi
  echo
  echo "Local tests passed."
else
  echo "Skipping local tests."
fi

# --- Version ---

NEW_VERSION="$1"
if [ -z "$NEW_VERSION" ]; then
  read -r -p "New version (MAJOR.MINOR.PATCH): " NEW_VERSION
fi
if ! echo "$NEW_VERSION" | grep -qE '^[0-9]+\.[0-9]+\.[0-9]+$'; then
  echo "Error: version must be in MAJOR.MINOR.PATCH format (e.g., 0.10.0)"
  exit 1
fi
TAG="v$NEW_VERSION"

SKIP_BUMP=0
if git rev-parse -q --verify "refs/tags/$TAG" >/dev/null; then
  TAG_COMMIT="$(git rev-parse "$TAG")"
  HEAD_COMMIT="$(git rev-parse HEAD)"
  if [ "$TAG_COMMIT" = "$HEAD_COMMIT" ]; then
    echo "Tag $TAG already exists and points to HEAD ($HEAD_COMMIT) -- skipping version bump."
    SKIP_BUMP=1
  else
    echo "Error: tag $TAG already exists but points elsewhere ($TAG_COMMIT, HEAD is $HEAD_COMMIT)."
    echo "Resolve manually (e.g. 'git tag -d $TAG') before re-running."
    exit 1
  fi
fi

# --- Bump version, regenerate changelog, review, commit + tag ---

if [ "$SKIP_BUMP" = "0" ]; then
  confirm "Bump version to $NEW_VERSION and regenerate CHANGELOG.md?" || exit 1

  bash "$SCRIPT_DIR/bump_version.sh" "$NEW_VERSION"
  node_modules/.bin/auto-changelog -p >/dev/null

  echo
  echo "--- CHANGELOG.md section for $TAG ---"
  start="$(grep -n "^#### \[$TAG\]" CHANGELOG.md | head -1 | cut -d: -f1)"
  if [ -z "$start" ]; then
    echo "Warning: could not locate a $TAG section in CHANGELOG.md."
  else
    rest="$(tail -n +"$((start + 1))" CHANGELOG.md | grep -n "^#### \[" | head -1 | cut -d: -f1)"
    if [ -n "$rest" ]; then
      end=$((start + rest - 1))
      sed -n "${start},${end}p" CHANGELOG.md
    else
      sed -n "${start},\$p" CHANGELOG.md
    fi
  fi
  echo "--------------------------------------"
  echo
  echo "Edit CHANGELOG.md now in another terminal if you want to adjust it, then come back."
  if ! confirm "Continue and commit the version bump ($TAG)?"; then
    echo "Stopped. library.properties / package.json / docs/conf.py / CHANGELOG.md are left modified for you to fix up or discard."
    exit 1
  fi

  git add CHANGELOG.md library.properties package.json docs/conf.py
  git commit -m "Bump to version $NEW_VERSION."
  git tag "$TAG"

  echo
  echo "Committed and tagged locally:"
  git log -1 --oneline
fi

# --- Push version bump ---

if confirm "Push $CURRENT_BRANCH and tag $TAG to origin?"; then
  git push origin "$CURRENT_BRANCH"

  REMOTE_TAG_COMMIT="$(git ls-remote origin "refs/tags/$TAG" | cut -f1)"
  if [ -z "$REMOTE_TAG_COMMIT" ]; then
    git push origin "$TAG"
  elif [ "$REMOTE_TAG_COMMIT" != "$(git rev-parse "$TAG")" ]; then
    echo "Remote tag $TAG points elsewhere (local changed after an earlier push, e.g. a fix-up commit)."
    if confirm "Force-push local tag $TAG to origin, overwriting it?"; then
      git push origin "$TAG" --force
    else
      echo "Stopped: local and remote tag $TAG disagree. Resolve manually before continuing."
      exit 1
    fi
  fi
else
  echo "Stopped before push. Commit and tag are local only; re-run this script later to continue."
  exit 0
fi

# --- Wait for CI ---
# Note: this repo's AUnit Tests / Compile Examples workflows only trigger on push when
# .ino/.cpp/.h files changed, but GitHub Actions ignores path filters for tag pushes, so a
# tag push (like the one above) always triggers them regardless of what changed.

TAG_SHA="$(git rev-parse "$TAG")"
echo
echo "Waiting for GitHub Actions on $TAG ($TAG_SHA)..."
CI_STATUS=0
python3 - "$TAG_SHA" <<'PY' || CI_STATUS=$?
import json, subprocess, sys, time

sha = sys.argv[1]
max_wait_s = 600
poll_s = 15
elapsed = 0
runs = []

while True:
    out = subprocess.run(
        ["gh", "run", "list", "--commit", sha, "--json", "name,status,conclusion"],
        capture_output=True, text=True
    )
    runs = json.loads(out.stdout or "[]")
    if runs and all(r["status"] == "completed" for r in runs):
        break
    if elapsed >= max_wait_s:
        print(f"Timed out after {max_wait_s}s waiting for CI on {sha}.")
        sys.exit(2)
    time.sleep(poll_s)
    elapsed += poll_s

if not runs:
    print(f"No CI runs found for {sha}.")
    sys.exit(0)

for r in runs:
    print(f"  {r['conclusion']:10} {r['name']}")
failed = [r for r in runs if r["conclusion"] not in ("success", "skipped")]
sys.exit(1 if failed else 0)
PY

if [ "$CI_STATUS" = "1" ]; then
  if ! confirm "Some CI runs did not succeed for $TAG_SHA. Continue anyway?"; then
    exit 1
  fi
elif [ "$CI_STATUS" = "2" ]; then
  confirm "Timed out waiting for CI. Continue without confirmed results?" || exit 1
else
  echo "CI OK."
fi

# --- Docs + PDF manual ---

if confirm "Rebuild HTML docs, doxygen reference and PDF manual now?"; then
  if [ ! -x "$ROOT_DIR/docs/sphinx_env/bin/sphinx-build" ]; then
    echo "Error: docs/sphinx_env not found or broken. Set up the venv first (see RELEASING.md)."
    exit 1
  fi
  if ! command -v doxygen >/dev/null 2>&1; then
    echo "Error: doxygen not found (needed for the doxygen/ reference and Breathe's XML)."
    exit 1
  fi

  CNAME_BACKUP=""
  if [ -f "$GHPAGES_DIR/CNAME" ]; then
    CNAME_BACKUP="$(mktemp)"
    cp "$GHPAGES_DIR/CNAME" "$CNAME_BACKUP"
  fi

  (
    cd "$ROOT_DIR/docs"
    # shellcheck disable=SC1091
    source sphinx_env/bin/activate
    make clean
    make doxygen  # must run before html: generates docs/xml that Breathe reads, and doxygen/ in the gh-pages worktree
    make html
    make latexpdf
  )

  # `make clean` wipes the gh-pages worktree's html/ dir, including CNAME, which has no
  # source to regenerate from -- restore it so it isn't accidentally deleted from gh-pages.
  if [ -n "$CNAME_BACKUP" ] && [ ! -f "$GHPAGES_DIR/CNAME" ]; then
    cp "$CNAME_BACKUP" "$GHPAGES_DIR/CNAME"
    echo "Restored CNAME in gh-pages worktree."
  fi
  [ -n "$CNAME_BACKUP" ] && rm -f "$CNAME_BACKUP"

  if [ -n "$(git status --porcelain -- extras/Plaquette-Manual.pdf)" ]; then
    git add extras/Plaquette-Manual.pdf
    echo
    if confirm "Commit and push updated PDF manual (\"Sync manual doc with $NEW_VERSION.\")?"; then
      git commit -m "Sync manual doc with $NEW_VERSION."
      git push origin "$CURRENT_BRANCH"
    else
      echo "PDF manual left staged, not committed."
    fi
  else
    echo "PDF manual unchanged, nothing to commit."
  fi
else
  echo "Skipping docs rebuild."
fi

# --- gh-pages sync ---

if [ ! -e "$GHPAGES_DIR/.git" ]; then
  echo
  echo "gh-pages worktree not found at $GHPAGES_DIR."
  if confirm "Set it up now (git worktree add $GHPAGES_DIR gh-pages)?"; then
    git worktree add "$GHPAGES_DIR" gh-pages
  fi
fi

if [ -e "$GHPAGES_DIR/.git" ]; then
  echo
  echo "--- gh-pages worktree status ---"
  (cd "$GHPAGES_DIR" && git status --short)
  echo "---------------------------------"
  if [ -n "$(cd "$GHPAGES_DIR" && git status --porcelain)" ]; then
    if confirm "Commit and push gh-pages sync (\"Sync with $NEW_VERSION.\")?"; then
      (
        cd "$GHPAGES_DIR"
        git add -A
        git commit -m "Sync with $NEW_VERSION."
        git push origin gh-pages
      )
    else
      echo "gh-pages worktree left with uncommitted changes."
    fi
  else
    echo "gh-pages worktree already up to date."
  fi
fi

# --- GitHub release ---

echo
if confirm "Create the GitHub release for $TAG now?"; then
  bash "$SCRIPT_DIR/release.sh" "$TAG"
else
  echo "Skipping GitHub release creation. Run './tools/release.sh $TAG' later to finish."
fi

echo
echo "Done."
