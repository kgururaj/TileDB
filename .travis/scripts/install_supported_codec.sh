#!/bin/bash

INSTALL_DIR=${INSTALL_DIR:-/usr}

if [[ $INSTALL_CODEC == true && $TRAVIS_OS_NAME == linux ]]; then
	# Install Zstandard - Not enabled - Seg faults when Code Coverage is ON
	pushd ~
	wget https://github.com/facebook/zstd/archive/v1.0.0.tar.gz &&
		tar xf v1.0.0.tar.gz &&
		cd zstd-1.0.0 &&
		sudo make install PREFIX=$INSTALL_DIR &&
		export ENABLE_ZSTD=0
	popd

	# Install Blosc
	pushd ~
	git clone https://github.com/Blosc/c-blosc.git &&
		cd c-blosc &&
		mkdir build &&
		cd build &&
		cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR .. &&
		cmake --build . &&
		sudo cmake --build . --target install &&
		export ENABLE_BLOSC=1
	popd

	# Install LZ4 - Not enabled - Travis/Trusty distr has a very old version of lz4
	sudo apt-get -y install liblz4-dev &&
		export ENABLE_LZ4=0
else
	export ENABLE_ZSTD=0
	export ENABLE_BLOSC=0
	export ENABLE_LZ4=0
fi
