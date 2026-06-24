#!/bin/bash

set -e

BUILD_DIR="build"
EXE_NAME="compiler"

mkdir -p $BUILD_DIR
echo "=========================="
echo "Created build directory..."
echo "=========================="

cmake -S . -B $BUILD_DIR
echo "=========================="
echo "Initizializated cmake configuration..."
echo "=========================="

cmake --build $BUILD_DIR
echo "========================="
echo "Building..."
echo "========================="

echo "========================="
echo "Running..."
echo "========================="

./$BUILD_DIR/$EXE_NAME


