#!/bin/bash

prefix="/usr/local"
version="1.0.1"
destdir="/bitcoin/dist/uraniumx-${version}-linux"

CONFIG_SITE="$PWD/depends/x86_64-pc-linux-gnu/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make clean && make -j4

strip "src/uraniumxd"
strip "src/uraniumx-tx"
strip "src/qt/uraniumx-qt"
strip "src/uraniumx-cli"
strip "src/.libs/liburaniumxconsensus.so"

make install DESTDIR="${destdir}"
