#!/bin/bash

set -e

version="1.0.1"
PATH="/opt/wine/usr/bin:$PATH"
MAKENSIS="$HOME/.wine/drive_c/Program Files (x86)/NSIS/makensis.exe"

find dist -name "*.exe.bak" -delete
find dist -name "*.dll.bak" -delete
find . -name "uraniumx*setup.exe" -delete
find . -name "uraniumx*setup.asc" -delete

wine "$MAKENSIS" "contrib/windeploy/setup-32.nsi"
wine "$MAKENSIS" "contrib/windeploy/setup-64.nsi"

pkg32="dist/uraniumx-${version}-win32-setup"
pkg64="dist/uraniumx-${version}-win64-setup"

if [ ! -z "$GPG_KEY" ]; then
    gpg -u "$GPG_KEY" --output "${pkg32}.asc" --detach-sig "${pkg32}.exe"
    gpg --verify "${pkg32}.asc" "${pkg32}.exe"
    gpg -u "$GPG_KEY" --output "${pkg64}.asc" --detach-sig "${pkg64}.exe"
    gpg --verify "${pkg64}.asc" "${pkg64}.exe"
fi

exit 0
