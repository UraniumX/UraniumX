#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/uraniumx-win32"

CONFIG_SITE="$PWD/depends/i686-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

i686-w64-mingw32-strip "src/uraniumxd.exe"
i686-w64-mingw32-strip "src/uraniumx-tx.exe"
i686-w64-mingw32-strip "src/qt/uraniumx-qt.exe"
i686-w64-mingw32-strip "src/uraniumx-cli.exe"
i686-w64-mingw32-strip "src/.libs/liburaniumxconsensus-0.dll"

make install DESTDIR="${destdir}"
