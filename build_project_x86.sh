#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [ ! -d $DIR/build_x86 ]; then
	mkdir $DIR/build_x86
fi
cd $DIR/build_x86

# To get the rootfs which is required here, use:
# rsync -rl --delete-after --safe-links pi@192.168.1.PI:/{lib,usr} $HOME/rpi/rootfs

cmake  -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make -j

if [ ! -f config ]; then
	cp $DIR/config config
fi
