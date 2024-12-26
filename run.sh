#!/bin/bash

ROOT_DIR=$(pwd)
BUILD_DIR=$ROOT_DIR/build
BIN_DIR=$ROOT_DIR/bin

# Check if libssl-dev is installed or not
if dpkg -l | grep -q libssl-dev; then
	echo "libssl-dev is already installed"
	echo "Proceeding to build"
	
	# Check if directories exist, if not create it
	if [ ! -d "$BUILD_DIR" ]; then 
	    echo "Creating build/ directory $BUILD_DIR"
	    mkdir -p "$BUILD_DIR"
	fi
	
	cd $BUILD_DIR
	cmake ..
	make -j 4
	cd $ROOT_DIR
	
	# Run the binary
	$BIN_DIR/merkle > out.log

else
	echo "Please install libssl-dev"
	echo "To install: sudo apt update && sudo apt install -y libssl-dev"
fi
