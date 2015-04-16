#!/bin/sh
set -ex
wget https://github.com/libuv/libuv/archive/v0.10.36.tar.gz
tar -xzvf v0.10.36.tar.gz
cd v0.10.36 && ./configure --prefix=/usr && make && sudo make install