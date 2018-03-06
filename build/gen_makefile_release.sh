#!/bin/sh

mkdir -p makefile_release
cd makefile_release
cmake ../.. -DCMAKE_CXX_COMPILER=clang++-4.0 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cd ..
