
Debian
====================
This directory contains files used to package uraniumxd/uraniumx-qt
for Debian-based Linux systems. If you compile uraniumxd/uraniumx-qt yourself, there are some useful files here.

## uraniumx: URI support ##


uraniumx-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install uraniumx-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your uraniumx-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

uraniumx-qt.protocol (KDE)
