#!/bin/bash

bash /bitcoin/contrib/vagrant/build-linux.sh && \
    make clean && bash /bitcoin/contrib/vagrant/build-win32.sh && \
    make clean && bash /bitcoin/contrib/vagrant/build-win64.sh && \
    make clean
