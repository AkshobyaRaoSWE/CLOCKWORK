#!/usr/bin/env bash
# Create a new PROS (LemLib + titanselect) project pre-organized into the
# CLOCKWORK multi-file layout: robot config, autons/, opcontrol, selector.
#
#   new-vex-project <Name> [dest-dir]
#
# The template's vendored firmware/libs are copied, so the new project builds
# offline with no `pros apply` needed.

set -euo pipefail

# Resolve the template dir from this script's real location (tools/ -> repo root),
# following the symlink if invoked via one. Override with $VEX_TEMPLATE.
SRC="${BASH_SOURCE[0]}"
while [[ -h "$SRC" ]]; do
	DIR="$(cd -P "$(dirname "$SRC")" && pwd)"
	SRC="$(readlink "$SRC")"
	[[ "$SRC" != /* ]] && SRC="$DIR/$SRC"
done
SCRIPT_DIR="$(cd -P "$(dirname "$SRC")" && pwd)"
TEMPLATE="${VEX_TEMPLATE:-$(cd "$SCRIPT_DIR/.." && pwd)}"

NAME="${1:-}"
if [[ -z "$NAME" ]]; then
	echo "usage: new-vex-project <Name> [dest-dir]" >&2
	exit 1
fi
DEST="${2:-$PWD/$NAME}"

if [[ ! -d "$TEMPLATE/tools/skeleton" ]]; then
	echo "error: template not found at: $TEMPLATE" >&2
	echo "set VEX_TEMPLATE to your CLOCKWORK path" >&2
	exit 1
fi
if [[ -e "$DEST" ]]; then
	echo "error: $DEST already exists" >&2
	exit 1
fi

echo "Creating $NAME -> $DEST"

# 1. Copy build scaffolding + vendored libs. Drop the template's own source,
#    build artifacts, editor caches, and git history.
rsync -a \
	--exclude 'bin/' \
	--exclude '.d/' \
	--exclude '.cache/' \
	--exclude '.git/' \
	--exclude 'src/' \
	--exclude 'tools/' \
	--exclude 'include/robot.hpp' \
	--exclude 'include/autons.hpp' \
	--exclude 'compile_commands.json' \
	"$TEMPLATE/" "$DEST/"

# 2. Overlay the clean, structured source skeleton.
cp -R "$TEMPLATE/tools/skeleton/src" "$DEST/src"
cp "$TEMPLATE/tools/skeleton/include/robot.hpp" "$DEST/include/robot.hpp"
cp "$TEMPLATE/tools/skeleton/include/autons.hpp" "$DEST/include/autons.hpp"

# 3. Set the project name in project.pros.
python3 - "$DEST/project.pros" "$NAME" <<'PY'
import json, sys
path, name = sys.argv[1], sys.argv[2]
with open(path) as f:
	data = json.load(f)
data["py/state"]["project_name"] = name
with open(path, "w") as f:
	json.dump(data, f, indent=2)
PY

# 4. Fresh git repo.
git -C "$DEST" init -q
git -C "$DEST" add -A
git -C "$DEST" commit -qm "Scaffold $NAME from CLOCKWORK template" >/dev/null 2>&1 || true

# 5. Regenerate compile_commands if the PROS CLI is available.
if command -v pros >/dev/null 2>&1; then
	( cd "$DEST" && pros build-compile-commands >/dev/null 2>&1 ) || true
fi

echo "Done. Next:"
echo "  cd \"$DEST\""
echo "  pros make        # or build from the PROS extension"
