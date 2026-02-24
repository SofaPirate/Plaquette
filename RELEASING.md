# Releasing Plaquette

This document describes the process for publishing a new release.

## Prerequisites

- [`gh`](https://cli.github.com/) CLI installed and authenticated
- Node.js and npm installed (`npm install` to install `auto-changelog`)

## Workflow

### 1. Prepare the release

Make sure all intended commits are on `develop` and the working tree is clean:

```bash
git status   # should show nothing to commit
```

### 2. Bump version, update changelog, commit and tag

Run `npm version` with the new version number:

```bash
npm version 0.9.4 --message "Bump to version %s."
```

Or use a bump type:

```bash
npm version patch --message "Bump to version %s."   # 0.9.3 → 0.9.4
npm version minor --message "Bump to version %s."   # 0.9.3 → 0.10.0
npm version major --message "Bump to version %s."   # 0.9.3 → 1.0.0
```

This automatically:
- Bumps the version in `package.json`, `library.properties`, and `docs/conf.py`
- Regenerates `CHANGELOG.md`
- Creates a git commit with all version-bumped files
- Creates the git tag (e.g. `v0.9.4`)

### 3. Push

```bash
git push && git push --tags
```

### 4. Create the GitHub release

```bash
./tools/release.sh
```

This extracts the release notes for the latest tag from `CHANGELOG.md` and creates the GitHub release. To target a specific tag:

```bash
./tools/release.sh v0.9.4
```
