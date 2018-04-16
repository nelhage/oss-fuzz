#!/bin/bash -eu
# Copyright 2018 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

# build zlib
pushd "$SRC/zlib"
./configure --static --prefix="$WORK"
make -j$(nproc) CFLAGS="$CFLAGS -fPIC"
make install
popd

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$WORK -DBUILD_SHARED_LIBS=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j$(nproc)
make install

# The libs being linked here need to become static but right now the cmake build process isn't generating static libs so oh well
$CXX $CXXFLAGS -std=c++11 -I$WORK/include -L$WORK/lib -rpath $WORK/lib/ \
    $SRC/fuzzmain.cc -o $OUT/fuzzmain \
    -lFuzzingEngine -lgvc -lcgraph -lcdt
