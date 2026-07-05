#!/usr/bin/env bash
# Point the PROS extension's "Create New PROS Project" command at the CLOCKWORK
# scaffold generator. Re-run this after the PROS extension updates (an update
# reinstalls into a new sigbots.pros-<version> folder and reverts the patch).

set -euo pipefail

SRC="${BASH_SOURCE[0]}"
while [[ -h "$SRC" ]]; do
	DIR="$(cd -P "$(dirname "$SRC")" && pwd)"; SRC="$(readlink "$SRC")"
	[[ "$SRC" != /* ]] && SRC="$DIR/$SRC"
done
SCRIPT_DIR="$(cd -P "$(dirname "$SRC")" && pwd)"
GEN="$SCRIPT_DIR/new-project.sh"

# Newest installed PROS extension for Cursor and/or VS Code.
shopt -s nullglob
EXT_DIRS=( "$HOME"/.cursor/extensions/sigbots.pros-* "$HOME"/.vscode/extensions/sigbots.pros-* )
if [[ ${#EXT_DIRS[@]} -eq 0 ]]; then
	echo "error: no sigbots.pros extension found" >&2
	exit 1
fi

patched=0
for EXT in "${EXT_DIRS[@]}"; do
	F="$EXT/out/commands/create-project.js"
	[[ -f "$F" ]] || continue
	[[ -f "$F.orig" ]] || cp "$F" "$F.orig"

	GEN="$GEN" python3 - "$F" <<'PY'
import os, re, sys
path = sys.argv[1]
gen = os.environ["GEN"].replace("\\", "\\\\").replace('"', '\\"')
s = open(path).read()

if "CLOCKWORK-SCAFFOLD" in s:
    print(f"  already patched: {path}")
    sys.exit(0)

block = re.compile(
    r'    let target = yield.*?successMessage: "Project Created Successfully",\n    \};',
    re.DOTALL,
)
repl = (
    '    // CLOCKWORK-SCAFFOLD: generate from the multi-file template.\n'
    '    const projectPath = path.join(dir, name);\n'
    f'    const scaffoldScript = "{gen}";\n'
    '    const createProjectCommandOptions = {\n'
    '        command: "bash",\n'
    '        args: [scaffoldScript, name, projectPath],\n'
    '        message: "Creating CLOCKWORK project",\n'
    '        requiresProsProject: false,\n'
    '        successMessage: "Project Created Successfully",\n'
    '    };'
)
new, n = block.subn(repl, s, count=1)
if n == 0:
    print(f"  WARN could not find stock block (extension layout changed?): {path}")
    sys.exit(2)
open(path, "w").write(new)
print(f"  patched: {path}")
PY
	patched=1
done

[[ $patched -eq 1 ]] || { echo "error: no create-project.js found" >&2; exit 1; }
echo "Done. Reload the editor window (Developer: Reload Window) for it to take effect."
