#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/uraniumx-win64"

CONFIG_SITE="$PWD/depends/x86_64-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

x86_64-w64-mingw32-strip "src/uraniumxd.exe"
x86_64-w64-mingw32-strip "src/uraniumx-tx.exe"
x86_64-w64-mingw32-strip "src/qt/uraniumx-qt.exe"
x86_64-w64-mingw32-strip "src/uraniumx-cli.exe"
x86_64-w64-mingw32-strip "src/.libs/liburaniumxconsensus-0.dll"

make install DESTDIR="${destdir}"
