#!/usr/bin/env bash
# CLOCKWORK build/upload helper.
# Uses the PROS CLI + ARM toolchain bundled with the Cursor/VS Code PROS extension,
# so you don't need pros installed on your PATH.
#
#   ./clock.sh           build (make)
#   ./clock.sh upload    build + upload to a connected V5 brain
#   ./clock.sh clean     clean build artifacts
#   ./clock.sh <other>   passed straight through to `pros`
set -euo pipefail

PROS_INSTALL="$HOME/Library/Application Support/Cursor/User/globalStorage/sigbots.pros/install"
export PROS_TOOLCHAIN="$PROS_INSTALL/pros-toolchain-macos"
export PATH="$PROS_TOOLCHAIN/bin:$PATH"
CLI="$PROS_INSTALL/pros-cli-macos/pros"

cd "$(dirname "$0")"

case "${1:-build}" in
  build)  "$CLI" make ;;
  upload) "$CLI" mu ;;          # make + upload
  clean)  "$CLI" make clean ;;
  *)      "$CLI" "$@" ;;
esac
