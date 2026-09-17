# Releasing Plaquette

This document describes the process for publishing a new release.

## Prerequisites

- [`gh`](https://cli.github.com/) CLI installed and authenticated (`gh auth login`)
- Node.js and npm installed (`npm install` to install `auto-changelog`)
- `docs/sphinx_env` set up and working (`cd docs && python3 -m venv sphinx_env && sphinx_env/bin/pip install -r requirements.txt`)
- `doxygen` and `graphviz` installed (`sudo apt install doxygen graphviz`) — needed for the `doxygen/` reference pages (Breathe reads `doxygen`'s XML output, and `graphviz`'s `dot` draws the class inheritance/collaboration diagrams)
- `latexmk` and a LaTeX toolchain installed (`sudo apt install latexmk texlive-full` or equivalent) — needed to build `extras/Plaquette-Manual.pdf`
- A `gh-pages` worktree checked out at `../Plaquette-docs/html` (sibling of this repo):
  ```bash
  git worktree add ../Plaquette-docs/html gh-pages
  ```

## Recommended: interactive script

```bash
./tools/do_release.sh 0.9.4
```

This walks through every step below, pausing for confirmation before each push or commit: local tests, changelog review (shows the generated `CHANGELOG.md` section and lets you hand-edit it before continuing), push, waiting for GitHub Actions to go green on the tag, docs + PDF manual rebuild, gh-pages sync, and GitHub release creation. It's safe to re-run: if the version bump/tag already happened, it picks up from the next step instead of redoing it.

## Manual workflow

If you'd rather run the steps by hand, or the script gets stuck:

### 1. Prepare the release

Make sure all intended commits are on `develop`/`master` and the working tree is clean:

```bash
git status   # should show nothing to commit
```

Run the local test suite:

```bash
cd tests && make clean && make && make test && cd ..
```

### 2. Bump version, update changelog, commit and tag

```bash
bash tools/bump_version.sh 0.9.4      # updates library.properties, package.json, docs/conf.py
node_modules/.bin/auto-changelog -p    # regenerates CHANGELOG.md
```

Review the new section in `CHANGELOG.md`, then:

```bash
git add CHANGELOG.md library.properties package.json docs/conf.py
git commit -m "Bump to version 0.9.4."
git tag v0.9.4
```

(`npm version 0.9.4 --message "Bump to version %s."` also does this in one shot via the `version` script in `package.json`, but it commits/tags immediately with no chance to review the changelog first — prefer the steps above, or the interactive script, if you want to check the changelog.)

### 3. Push

```bash
git push && git push --tags
```

Then check that CI is green for the tag:

```bash
gh run list --commit v0.9.4
```

This repo's `AUnit Tests` / `Compile Examples` workflows only trigger on a branch push when `.ino`/`.cpp`/`.h` files changed, but GitHub ignores path filters on tag pushes — so pushing the tag above always re-triggers them regardless of what the release commit touched. Wait for both to show `success` before continuing.

### 4. Rebuild the docs and PDF manual

```bash
cd docs
source sphinx_env/bin/activate
make clean
make doxygen    # must run before html: generates docs/xml for Breathe, and doxygen/ in the gh-pages worktree
make html       # writes to ../../Plaquette-docs/html (the gh-pages worktree)
make latexpdf   # writes to ../extras/Plaquette-Manual.pdf
cd ..
```

`make clean` wipes the gh-pages worktree's `html/` directory, which deletes `CNAME` (a hand-added GitHub Pages file with no source to regenerate it from — currently `plaquette.org`). Restore it before committing:

```bash
cd ../Plaquette-docs/html && git checkout -- CNAME && cd -
```

Commit the updated manual on `master`:

```bash
git add extras/Plaquette-Manual.pdf
git commit -m "Sync manual doc with 0.9.4."
git push
```

### 5. Sync gh-pages

`make html` writes directly into the `gh-pages` worktree (`../Plaquette-docs/html`), so just commit and push from there:

```bash
cd ../Plaquette-docs/html
git status
git add -A
git commit -m "Sync with 0.9.4."
git push origin gh-pages
cd -
```

### 6. Create the GitHub release

```bash
./tools/release.sh
```

This extracts the release notes for the latest tag from `CHANGELOG.md` and creates the GitHub release. To target a specific tag:

```bash
./tools/release.sh v0.9.4
```
