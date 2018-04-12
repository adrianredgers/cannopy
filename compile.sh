#!/usr/bin/env bash

#
# Create build dir and make the targets
#
BUILD_DIR=build
mkdir -p $BUILD_DIR
rm -rf $BUILD_DIR/*
pushd $BUILD_DIR
cmake ..
make
popd


