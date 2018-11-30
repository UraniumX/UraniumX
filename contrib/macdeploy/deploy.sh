#!/bin/bash

version="1.0.1"
builder=`depends/config.guess`
site="$PWD/depends/${builder}/share/config.site"

if [ ! -f "$site" ]; then
    echo "Config site not found for OSX"
    echo "$site"
    exit 1
fi

CONFIG_SITE="${site}" CXXFLAGS="-fvisibility=hidden" ./configure --prefix="$HOME/.local" --disable-tests

make clean && make -j8 && make deploy
set -e
pkg=dist/uraniumx-${version}-osx
rm -f ${pkg}.asc
mv UraniumX-Qt.dmg ${pkg}.dmg
rm -rf ./UraniumX*.app

if [ ! -z "$GPG_KEY" ]; then
    gpg -u "$GPG_KEY" --output "${pkg}.asc" --detach-sig "${pkg}.dmg"
    gpg --verify ${pkg}.asc ${pkg}.dmg
fi
