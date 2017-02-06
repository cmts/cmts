#!/bin/bash

    if [ ! -d "objs" ]; then
        mkdir objs
    fi

    echo "Installing OpenResty ..."
#    tar zxf 3rdparty/openresty-1.11.2.2.tar.gz -C objs/
#    tar zxf 3rdparty/openssl-1.0.2k.tar.gz -C objs/
    cd objs/openresty-1.11.2.2
#    ./configure --with-openssl=../openssl-1.0.2k --with-cc-opt="-I/usr/local/include" --with-ld-opt="-L/usr/local/lib" -j4
#    make -j4
#    sudo make install
    cd ..

    echo "Installing MySQL Server ..."
    brew install mysql

    echo "Setting MySQL root password ..."
    mysqladmin -u root password

    echo "Installing FFmpeg ..."
    git clone git://source.ffmpeg.org/ffmpeg.git



