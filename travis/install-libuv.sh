#!/bin/sh
set -ex
wget https://github.com/libuv/libuv/archive/v1.4.2.tar.gz
tar -xzvf v1.4.2.tar.gz
cd libuv-1.4.2 && ./autogen.sh && ./configure --prefix=/usr && make && sudo make install