#!/bin/bash

version="1.0.1"
pkgname="uraniumx-${version}-linux"

set -e
cd dist

rm -f "uraniumx-*-linux.*"
tar -zcvf "${pkgname}.tar.gz" "${pkgname}"

if [ ! -z "$GPG_KEY" ]; then
    gpg -u "$GPG_KEY" --output "${pkgname}.asc" --detach-sig "${pkgname}.tar.gz"
    gpg --verify "${pkgname}.asc" "${pkgname}.tar.gz"
fi
