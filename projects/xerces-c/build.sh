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

cp $SRC/xml.dict $OUT/sax_parse_fuzzer.dict
cp $SRC/xml.dict $OUT/dom_parse_fuzzer.dict

./reconf
./configure --disable-shared --with-pic=yes --prefix="$WORK"
make -j$(nproc)
make install

# build fuzzers
# e.g.
$CXX $CXXFLAGS -std=c++11 -I$WORK/include \
    $SRC/sax_parse_fuzzer.cc -o $OUT/sax_parse_fuzzer \
    -lFuzzingEngine $WORK/lib/libxerces-c.a

$CXX $CXXFLAGS -std=c++11 -I$WORK/include \
    $SRC/dom_parse_fuzzer.cc -o $OUT/dom_parse_fuzzer \
    -lFuzzingEngine $WORK/lib/libxerces-c.a
